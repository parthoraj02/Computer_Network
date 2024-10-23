#include <iostream>
using namespace std;

#define MAX_PKT 1024          // Maximum packet size
#define TOTAL_FRAMES 10      // Total number of frames to send
#define WINDOW_SIZE 4        // Size of the sliding window

enum event_type { frame_arrival, timeout };
typedef unsigned int seq_num;

struct packet {
    char data[MAX_PKT];      // Data contained in the packet
};

struct frame {
    packet info;             // Packet inside the frame
    seq_num seq;             // Sequence number of the frame
    seq_num ack;             // Acknowledgment number
};

void from_network_layer(packet &p) {
    cout << "Fetching packet from network layer\n";
}

void to_network_layer(const packet &p) {
    cout << "Delivering packet to network layer\n";
}

void to_physical_layer(const frame &s) {
    cout << "Sending frame: Seq = " << s.seq << ", Ack = " << s.ack << endl;
}

void from_physical_layer(frame &r) {
    // Simulating received acknowledgment for the last sent frame
    if (r.seq < TOTAL_FRAMES) {
        r.ack = r.seq + 1; // Acknowledgment number is the next expected frame
    } else {
        r.ack = r.seq; // Last frame sent, acknowledge itself
    }
    cout << "Received frame: Seq = " << r.seq << ", Ack = " << r.ack << endl;
}

void wait_for_event(event_type &event) {
    event = frame_arrival; // Simulating a frame arrival event
}

void start_timer(seq_num seq) {
    cout << "Starting timer for frame: " << seq << endl;
}

void stop_timer(seq_num seq) {
    cout << "Stopping timer for frame: " << seq << endl;
}

// Function to toggle between sequence numbers
seq_num toggle(seq_num num) {
    return (num + 1) % (2 * WINDOW_SIZE);
}

// Go-Back-N protocol implementation
void go_back_n_protocol() {
    seq_num next_frame_to_send = 0; // Next frame to send
    seq_num base = 0;                // Base of the window
    frame window[WINDOW_SIZE];       // Sliding window

    for (int i = 0; i < TOTAL_FRAMES; ) {
        // Send frames within the window
        for (int j = 0; j < WINDOW_SIZE && next_frame_to_send < TOTAL_FRAMES; j++) {
            packet buffer;
            from_network_layer(buffer); // Fetch packet from the network layer
            window[next_frame_to_send % WINDOW_SIZE].info = buffer; // Copy to window frame
            window[next_frame_to_send % WINDOW_SIZE].seq = next_frame_to_send; // Set seq
            window[next_frame_to_send % WINDOW_SIZE].ack = base; // Set ack to the base
            to_physical_layer(window[next_frame_to_send % WINDOW_SIZE]); // Send frame
            start_timer(next_frame_to_send); // Start timer for this frame
            next_frame_to_send++; // Increment next frame to send
            i++; // Increment total frames sent
        }

        // Wait for event (frame arrival)
        event_type event;
        wait_for_event(event);

        // Process incoming frames
        if (event == frame_arrival) {
            frame r;
            // Simulate the reception of frames with the sequence number of the last sent frame
            r.seq = base; // Acknowledge the current base frame
            from_physical_layer(r); // Receive frame
            if (r.ack == base + 1) { // If the ack matches the next expected frame
                stop_timer(base); // Stop timer for the base frame
                base++; // Slide the window
            }
        }
    }

    cout << "All frames sent and acknowledged.\n";
}

int main() {
    go_back_n_protocol(); // Start the Go-Bac
    return 0;

    }
