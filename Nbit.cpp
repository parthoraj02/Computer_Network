#include <iostream>
using namespace std;

#define TOTAL_FRAMES 8       // Total number of frames to send
#define WINDOW_SIZE 3        // Sliding window size
#define MAX_PKT 1024         // Maximum packet size

typedef int seq_num;        // Sequence number type

enum event_type { frame_arrival, timeout };

// Structure to represent a packet
struct packet {
    char data[MAX_PKT];      // Data contained in the packet
};


// Structure to represent a frame
struct frame {
    packet info;             // Packet inside the frame
    seq_num seq;             // Sequence number of the frame
    seq_num ack;             // Acknowledgment number
};

// Simulates fetching a packet from the network layer
void from_network_layer(packet &p, int seq) {
    cout << "Fetching packet " << seq << " from the network layer.\n";
}

// Simulates delivering a packet to the network layer
void to_network_layer(packet &p, int seq) {
    cout << "Delivering packet " << seq << " to the network layer.\n";
}

// Simulates sending a frame to the physical layer
void to_physical_layer(frame &f) {
    cout << "Sender sends frame: " << f.seq << endl;
}

// Simulates receiving an acknowledgment from the receiver
void from_physical_layer(seq_num &ack, frame &f) {
    cout << "Receiver acknowledges frame: " << f.seq << endl;
    ack = f.seq;
}

// Simulates starting a timer for a frame
void start_timer(frame &f) {
    cout << "Start timer for frame: " << f.seq << endl;
}

// Simulates stopping a timer for a frame
void stop_timer(frame &f) {
    cout << "Stop timer for frame: " << f.seq << endl;
}

// Simulates an event
void wait_for_event(event_type &event) {
    event = frame_arrival;  // Assuming the event is always a frame arrival for simplicity
}

// Sliding Window Protocol with additional layers
void sliding_window_protocol() {
    int window_start = 0;    // Start of the sliding window
    int next_frame = 0;      // Next frame to send
    int last_ack = -1;       // Last acknowledgment received

    // Arrays for frames and packets
    frame frames[TOTAL_FRAMES];
    packet packets[TOTAL_FRAMES];

    // Initialize packets and frames with sequence numbers
    for (int i = 0; i < TOTAL_FRAMES; ++i) {

        frames[i].info = packets[i];
        frames[i].seq = i;
        frames[i].ack = -1;
    }

    while (last_ack < TOTAL_FRAMES - 1) {
        // Print the current window

        cout << "Current Window: ";
        for (int i = window_start; i < window_start + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {

            cout << frames[i].seq << " ";
        }
        cout << endl;

        // Sender sends frames within the current window
        for (int i = window_start; i < window_start + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            if (frames[i].seq >= next_frame) {
                from_network_layer(packets[i], i);
                frames[i].info = packets[i];        // Assign the packet data to the frame
                frames[i].seq = i;
                frames[i].ack = -1;
            // Ensure only unsent frames are sent
                to_physical_layer(frames[i]);
                start_timer(frames[i]);
                next_frame++;
            }
        }

        // Simulate receiving acknowledgment for the first frame in the window
        if (window_start < TOTAL_FRAMES) {
            event_type event;
            wait_for_event(event);

            if (event == frame_arrival) {
                seq_num ack = -1;
                from_physical_layer(ack, frames[window_start]);
                stop_timer(frames[window_start]);
                to_network_layer(frames[window_start].info, frames[window_start].seq);  // Deliver the packet to the network layer
                last_ack = ack;

                // Slide the window forward
                window_start = last_ack + 1;
            }
        }

        cout << "-------------------------------------\n";
    }

    cout << "All frames sent and acknowledged.\n";
}

// Main function
int main() {
    sliding_window_protocol();
    return 0;
}
