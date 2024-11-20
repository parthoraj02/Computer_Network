#include <bits/stdc++.h>
using namespace std;

#define MAX_PKT 1024
#define TOTAL_FRAMES 8
#define WINDOW_SIZE 2

typedef int seq_num;

enum event_type { frame_arrival, timeout };

struct packet {
    char data[MAX_PKT];
};

struct frame {
    packet info;
    seq_num seq;
    seq_num ack;
};

void from_network_layer(packet &p, int seq) {
    cout << "Fetch packet " << seq << " from network layer" << endl;
}

void to_network_layer(packet &p, int seq) {
    cout << "Deliver packet " << seq << " to network layer" << endl;
}

void to_physical_layer(frame &f) {
    cout << "Send frame " << f.seq << endl;
}

void from_physical_layer(frame &f, seq_num &ack) {
    cout << "Received frame " << f.seq << " and send ack" << endl;
    ack = f.seq; // Acknowledge the received frame
}

void start_timer(frame &f) {
    cout << "Start timer for frame " << f.seq << endl;
}

void stop_timer(frame &f) {
    cout << "Stop timer for frame " << f.seq << endl;
}

void wait_for_event(event_type &event) {
    event = frame_arrival; // Simulate always receiving a frame
}

void p() {
    int window_start = 0;
    int next_frame = 0;
    int previous_ack = -1;  // Acknowledgment tracker for Go-Back-N
    event_type event;

    frame far[TOTAL_FRAMES];
    packet par[TOTAL_FRAMES];

    // Initialize packets and frames
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        from_network_layer(par[i], i);  // Fetch packet data from network layer
        far[i].info = par[i];  // Assign the packet to the frame
        far[i].seq = i;        // Set the sequence number for the frame
        far[i].ack = -1;       // Initial acknowledgment is -1
    }

    // Go-Back-N ARQ loop
    while (previous_ack < (TOTAL_FRAMES - 1)) {
        // Print the current window
        cout << "Current Window: ";
        for (int i = window_start; i < window_start + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            cout << far[i].seq << " ";
        }
        cout << endl;

        // Send frames within the window
        for (int i = window_start; i < window_start + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            if (far[i].seq >= next_frame) {  // Send the frame only if its sequence number is less than next_frame
                to_physical_layer(far[i]);
                start_timer(far[i]);
                next_frame++;
            }
        }

        // Wait for an event (frame arrival or timeout)
        if (window_start < TOTAL_FRAMES) {
            wait_for_event(event);
            if (event == frame_arrival) {
                seq_num ack = -1;
                from_physical_layer(far[window_start], ack);  // Get acknowledgment for the frame
                stop_timer(far[window_start]);

                // Deliver the packet to the network layer if it's within the current window
                to_network_layer(far[window_start].info, far[window_start].seq);

                // Update the previous_ack to the acknowledgment number
                previous_ack = ack;

                // Slide the window forward: Go-Back-N resends all frames after an unacknowledged frame
                if (ack == window_start) {
                    // If the first frame in the window is acknowledged, move the window forward
                    window_start = previous_ack + 1;
                }
            }
            else if (event == timeout) {
                // Handle timeout: resend all frames starting from the window_start
                cout << "Timeout occurred for frame " << window_start << endl;
                next_frame = window_start;
            }
        }
    }
}

int main() {
    p(); // Start the Go-Back-N ARQ protocol simulation
    return 0;
}
