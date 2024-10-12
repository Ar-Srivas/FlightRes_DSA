#include <iostream>
using namespace std;

class Seat_mat {
public:
    int m = 10;  // Number of rows
    int n = 10;  // Number of columns
    int seat[10][10];  // 2D array to hold seat status
    int count_seat;  // Counts the number of seats booked

    // Constructor to initialize the seat matrix
    Seat_mat() {
        default_Seat_Matrix();  // Initialize seat matrix
        count_seat = 0;  // Initialize booked seat count
    }

    // Initialize the seat matrix to available
    void default_Seat_Matrix() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                seat[i][j] = 0;  // 0 indicates available seat
            }
        }
    }

    // Get the count of booked seats
    int get_booked_seat_count() const {
        return count_seat;  // Return the number of booked seats
    }

    // Display the seat matrix
    void display_seat_Matrix() const {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // Print "O" for available and "X" for booked seats
            cout << (seat[i][j] == 0 ? "O" : "X") << " "; 
        }
        cout << endl;
    }
}

    // Book a seat at the specified position
    void book_seat(int pos_i, int pos_j) {
        // Check if the indices are valid
        if (pos_i < 0 || pos_i >= m || pos_j < 0 || pos_j >= n) {
            cout << "Invalid seat position." << endl;
            return;
        }

        // Check if the seat is already booked
        if (seat[pos_i][pos_j] == 1) {
            cout << "Seat already booked." << endl;
            return;
        }

        // Check if the flight is full
        if (count_seat == m * n) {
            cout << "Flight is full." << endl;
            return;
        }

        // Book the seat
        seat[pos_i][pos_j] = 1;  // Mark the seat as booked
        count_seat++;  // Increment the booked seat count
        cout << "Seat booked successfully." << endl;
    }
};
