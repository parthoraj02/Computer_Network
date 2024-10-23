#include <iostream>
#include <cstring>

using namespace std;

#define MAX_PKT 1024        // Maximum packet size (arbitrary)
#define TOTAL_FRAMES 5      // Total number of frames to send

typedef unsigned int seq_num;  // Sequence number type (0 or 1)
enum event_type { frame_arrival, timeout };  // Possible events

// Structure representing a packet
struct packet {
    char data[MAX_PKT];    // Data contained in the packet
};

// Structure representing a frame
struct frame {
    packet info;           // Packet inside the frame
    seq_num seq;           // Sequence number of the frame
    seq_num ack;           // Acknowledgment number
};

// Function to simulate fetching a packet from the network layer
void from_network_layer(packet &p) {
    strcpy(p.data, "Packet data");  // Simulating packet data
    cout << "Fetching packet from network layer: " << p.data << endl;
}

// Function to simulate delivering a packet to the network layer
void to_network_layer(const packet &p) {
    cout << "Delivering packet to network layer: " << p.data << endl;
}

// Function to simulate receiving a frame from the physical layer
void from_physical_layer(frame &f) {
    // Simulating correct sequence and acknowledgment for alternating frames
    static seq_num expected_seq = 0;
    f.seq = expected_seq;   // Set the expected sequence number (alternating 0 and 1)
    f.ack = expected_seq;   // Acknowledge the frame with the same sequence number
    cout << "Receiving frame: Seq = " << f.seq << ", Ack = " << f.ack << endl;
    expected_seq = (expected_seq + 1) % 2;  // Toggle between 0 and 1
}

// Function to simulate sending a frame to the physical layer
void to_physical_layer(const frame &f) {
    cout << "Sending frame: Seq = " << f.seq << ", Ack = " << f.ack << endl;
}

// Function to simulate waiting for an event (e.g., frame arrival)
void wait_for_event(event_type &event) {
    event = frame_arrival;  // Simulating a frame arrival event
}

// Function to simulate starting a timer for a frame
void start_timer(seq_num seq) {
    cout << "Starting timer for frame: " << seq << endl;
}

// Function to simulate stopping a timer for a frame
void stop_timer(seq_num seq) {
    cout << "Stopping timer for frame: " << seq << endl;
}

// Stop-and-Wait protocol implementation
void stop_and_wait_protocol() {
    seq_num frame_to_send = 0;      // Sequence number of the next frame to send
    seq_num expected_frame = 0;     // Expected sequence number of the incoming frame
    packet buffer;                  // Buffer to store the packet data
    frame s, r;                     // Frames for sending and receiving
    event_type event;               // Event type (frame arrival, timeout)

    for (int i = 0; i < TOTAL_FRAMES; i++) {
        from_network_layer(buffer);        // Get the next packet to send
        s.info = buffer;                   // Copy the packet into the frame
        s.seq = frame_to_send;             // Set the sequence number
        s.ack = 1 - expected_frame;        // Acknowledge the last received frame

        to_physical_layer(s);              // Send the frame
        start_timer(s.seq);                // Start the timer for this frame

        wait_for_event(event);             // Wait for an event (e.g., frame arrival)

        if (event == frame_arrival) {
            from_physical_layer(r);        // Receive a frame
            if (r.seq == expected_frame) { // Check if it's the expected frame
                to_network_layer(r.info);  // Deliver the packet to the network layer
                expected_frame = (expected_frame + 1) % 2;  // Toggle between 0 and 1
            }

            if (r.ack == frame_to_send) {  // Check if the frame was acknowledged
                stop_timer(frame_to_send); // Stop the timer
                frame_to_send = (frame_to_send + 1) % 2;  // Toggle between 0 and 1
            }
        }
    }

    cout << "All frames sent and acknowledged." << endl;
}

int main() {
    stop_and_wait_protocol();  // Start the Stop-and-Wait protocol
    return 0;
}
