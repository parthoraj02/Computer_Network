#include <bits/stdc++.h>
using namespace std;

#define TOTAL_FRAMES 8       // Total number of frames to send
#define WINDOW_SIZE 3        // Sliding window size

enum event_type { frame_arrival, timeout };

// Simulates fetching a packet from the network layer
void from_network_layer(int frame) {
    cout << "Fetching frame " << frame << " from the network layer.\n";
}

// Simulates delivering a packet to the network layer
void to_network_layer(int frame) {
    cout << "Delivering frame " << frame << " to the network layer.\n";
}

// Simulates sending a frame to the physical layer
void to_physical_layer(int frame) {
    cout << "Sender sends frame: " << frame << endl;
}

// Simulates receiving an acknowledgment from the receiver
void from_physical_layer(int &ack, int frame) {
    // Simulate acknowledgment with some delay or potential loss
    if (rand() % 5 == 0) {  // Randomly simulate acknowledgment loss
        cout << "Acknowledgment for frame " << frame << " is lost!" << endl;
        ack = -1;  // No acknowledgment received
    } else {
        cout << "Receiver acknowledges frame: " << frame << endl;
        ack = frame;
    }
}

// Simulates starting a timer for a frame
void start_timer(int frame) {
    cout << "Start timer for frame: " << frame << endl;
}

// Simulates stopping a timer for a frame
void stop_timer(int frame) {
    cout << "Stop timer for frame: " << frame << endl;
}

// Simulates an event
void wait_for_event(event_type &event) {
    event = frame_arrival;  // Assuming the event is always a frame arrival for simplicity
}

// Go-Back-N ARQ protocol
void go_back_n_arq() {
    int window_start = 0;   // Start of the sliding window
    int next_frame = 0;     // Next frame to send
    int last_ack = -1;      // Last acknowledgment received

    while (last_ack < TOTAL_FRAMES - 1) {
        // Print the current window
        cout << "Current Window: ";
        for (int i = window_start; i < window_start + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            cout << i << " ";
        }
        cout << endl;

        // Sender sends frames within the current window
        for (int i = window_start; i < window_start + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            if (i >= next_frame) {  // Ensure only unsent frames are sent
                from_network_layer(i);
                to_physical_layer(i);
                start_timer(i);
                next_frame++;
            }
        }

        // Simulate receiving acknowledgment for the first frame in the window
        int ack = -1;
        from_physical_layer(ack, window_start);

        if (ack == -1) {
            // Timeout or acknowledgment loss, retransmit from the current window start
            cout << "Timeout occurred! Retransmitting frames starting from frame: " << window_start << endl;
            next_frame = window_start;  // Reset next_frame to window_start for retransmission
        } else {
            stop_timer(ack);
            to_network_layer(ack);
            last_ack = ack;

            // Slide the window forward if the acknowledgment is valid
            window_start = last_ack + 1;
        }

        cout << "-------------------------------------\n";
    }

    cout << "All frames sent and acknowledged.\n";
}

// Main function
int main() {
    srand(time(0)); // Seed random number generator for simulating acknowledgment loss
    go_back_n_arq();
    return 0;
}

