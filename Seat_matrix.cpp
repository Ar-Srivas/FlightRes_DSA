#include <iostream>
using namespace std;

class Seat_mat {
public:
    int m = 10;  // Number of rows
    int n = 10;  // Number of columns
    int seat[10][10];
    int count_seat;  // Counts the number of seats booked

    Seat_mat() {
        default_Seat_Matrix();  // Initialize seat matrix in the constructor
        count_seat = 0;  // Initialize booked seat count
    }

    void default_Seat_Matrix() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                seat[i][j] = 0;  // 0 indicates available seat
            }
        }
    }

    int get_booked_seat_count() const {
        return count_seat;  // Return the number of booked seats
    }

    void display_seat_Matrix() const {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cout << seat[i][j] << " ";  // Print the seat matrix
            }
            cout << endl;
        }
    }

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
