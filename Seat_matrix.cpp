#include <iostream>
using namespace std;

class Seat_mat {
public:
    int m = 10; 
    int n = 10; 
    int seat[10][10];
    int count_seat;  
    Seat_mat() {
        default_Seat_Matrix();
        count_seat = 0;  
    }
    void default_Seat_Matrix() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                seat[i][j] = 0;
            }
        }
    }
    int get_booked_seat_count() const {
        return count_seat;
    }

    void display_seat_Matrix() const {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << (seat[i][j] == 0 ? 0 : 1) << " "; 
        }
        cout << endl;
    }
}

    void book_seat(int pos_i, int pos_j) {
        if (pos_i < 0 || pos_i >= m || pos_j < 0 || pos_j >= n) {
            cout << "Invalid seat position." << endl;
            return;
        }

        if (seat[pos_i][pos_j] == 1) {
            cout << "Seat already booked." << endl;
            return;
        }

        if (count_seat == m * n) {
            cout << "Flight is full." << endl;
            return;
        }

        seat[pos_i][pos_j] = 1; 
        count_seat++;
        cout << "Seat booked successfully." << endl;
    }
};
