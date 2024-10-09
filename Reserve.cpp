#include <iostream>
using namespace std;

#define MAX 5 
class CircularQueue {
    int front, rear;
    int arr[MAX]; 

public:
    CircularQueue() {
        front = rear = -1;
    }

    bool isFull() {
        if (front == 0 && rear == MAX - 1) {
            return true; 
        }
        if (rear + 1 == front) {
            return true;
        }
        return false;
    }

    bool isEmpty() {
        return (front == -1);
    }

    void enqueue(int reservationID) {
        if (isFull()) {
            cout << "Queue is full, cannot add more reservations.\n";
            return;
        }

        if (isEmpty()) {
            front = rear = 0; 
        } else if (rear == MAX - 1 && front > 0) {
            rear = 0;
        } else {
            rear++;
        }

        arr[rear] = reservationID;
        cout << "Reservation with ID " << reservationID << " added.\n";
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty, no reservations to remove.\n";
            return;
        }

        cout << "Reservation with ID " << arr[front] << " removed.\n";

        if (front == rear) {
            
            front = rear = -1;
        } else if (front == MAX - 1) {
            front = 0;
        } else {
            front++; 
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "No reservations.\n";
            return;
        }

        cout << "Current reservations: ";
        int i = front;
        while (i != rear) {
            cout << arr[i] << " ";
            if (i == MAX - 1) {
                i = 0; 
            } else {
                i++;
            }
        }
        cout << arr[rear] << endl;
    }
};

int main() {
    CircularQueue reservationQueue;

    reservationQueue.enqueue(101); 
    reservationQueue.enqueue(102);
    reservationQueue.enqueue(103);
    reservationQueue.enqueue(104);
    reservationQueue.enqueue(105); 
    reservationQueue.display();

    reservationQueue.dequeue();
    reservationQueue.dequeue();

    reservationQueue.display(); 

    reservationQueue.enqueue(106);
    reservationQueue.enqueue(107);

    reservationQueue.display(); 

    return 0;
}