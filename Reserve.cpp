// CircularQueue.cpp
#include <iostream>
#include <vector>
using namespace std;

class CircularQueue {
private:
    int front, rear, size;
    vector<int> reservations;

public:
    CircularQueue(int maxSize) : front(-1), rear(-1), size(maxSize) {
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
