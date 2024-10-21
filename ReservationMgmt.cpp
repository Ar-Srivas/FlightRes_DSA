#include <iostream>
#include <vector>

using namespace std;

class CircularQueue {
private:
    int front, rear, size;
    vector<int> reservations;

public:
    CircularQueue(int maxSize) {
        front = -1;
        rear = -1;
        size = maxSize;
        reservations.resize(size);
    }

    bool isFull() {
        return (front == (rear + 1) % size);
    }

    bool isEmpty() {
        return (front == -1);
    }


    void enqueue(int reservationID) {
        if (isFull()) {
            cout << "Reservation queue is full. Cannot accept more bookings.\n";
        } else {
            rear = (rear + 1) % size;
            reservations[rear] = reservationID;

            if (front == -1) {
                front = 0;
            }
            cout << "Reservation " << reservationID << " added.\n";
        }
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Reservation queue is empty. No reservations to cancel.\n";
        } else {
            cout << "Reservation " << reservations[front] << " canceled.\n";
            if (front == rear) {

                front = rear = -1;
            } else {
                front = (front + 1) % size;
            }
        }
    }


    void displayQueue() {
        if (isEmpty()) {
            cout << "No reservations in the queue.\n";
        } else {
            cout << "Current reservations: ";
            int i = front;
            while (i != rear) {
                cout << reservations[i] << " ";
                i = (i + 1) % size;
            }
            cout << reservations[rear] << endl;
        }
    }
};


vector<vector<int>> seatMatrix;
int rows, cols;


void initializeSeats(int r, int c) {
    rows = r;
    cols = c;
    seatMatrix.resize(rows, vector<int>(cols, 0));
}

bool checkSeatAvailability(int& row, int& col) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (seatMatrix[i][j] == 0) {
                row = i;
                col = j;
                seatMatrix[i][j] = 1;
                return true;
            }
        }
    }
    return false;
}


void cancelSeat(int row, int col) {
    if (row >= 0 && row < rows && col >= 0 && col < cols && seatMatrix[row][col] == 1) {
        seatMatrix[row][col] = 0; 
        cout << "Seat at Row " << row + 1 << ", Column " << col + 1 << " has been freed.\n";
    } else {
        cout << "Invalid seat cancellation.\n";
    }
}

int main() {
    int maxReservations;
    cout << "Enter the number of rows and columns for seat arrangement (e.g., 5 4): ";
    int r, c;
    cin >> r >> c;
    initializeSeats(r, c);

    cout << "Enter the maximum number of reservations the system can handle: ";
    cin >> maxReservations;

    CircularQueue reservationQueue(maxReservations);

    int choice, reservationID, row, col;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Book a Seat\n";
        cout << "2. Cancel a Reservation\n";
        cout << "3. View Reservations\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Checking seat availability...\n";
            if (checkSeatAvailability(row, col)) {
                cout << "Enter reservation ID: ";
                cin >> reservationID;
                reservationQueue.enqueue(reservationID);
                cout << "Seat booked at Row " << row + 1 << ", Column " << col + 1 << ".\n";
            } else {
                cout << "No seats available.\n";
            }
            break;

        case 2:
            cout << "Enter the row and column to cancel the seat (e.g., 2 3): ";
            cin >> row >> col;
            cancelSeat(row - 1, col - 1);
            reservationQueue.dequeue();
            break;

        case 3:
            reservationQueue.displayQueue();
            break;

        case 4:
            cout << "Exiting system.\n";
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
