#include <iostream>
#include <sstream>
#include <vector>
#include "Seat_matrix.cpp"  // Adjust the filename based on your project structure
#include "GraphForAirportFlight1.cpp"  // Ensure the filename is correct

using namespace std;

int main(int argc, char* argv[])
{
    Seat_mat s1;  // Create a Seat_mat object
    AirportGraph flightMap;

    // Adding flights to the airport graph
    flightMap.addFlight("JFK", "LAX");
    flightMap.addFlight("JFK", "ORD");
    flightMap.addFlight("LAX", "SFO");
    flightMap.addFlight("ORD", "DFW");

    // Check command line arguments
    if (argc < 2) {
        cout << "Invalid arguments. Please provide a command." << endl;
        return 1;
    }

    string command = argv[1];

    // Display available flights
    if (command == "flights") {
        if (argc < 3) {
            cout << "Please provide an airport code." << endl;
            return 1;
        }
        string airportCode = argv[2]; // Airport code should be passed as an argument
        cout << flightMap.getFlightsFrom(airportCode) << endl; // Ensure this function returns a string
    }
    // Book seats
    else if (command == "book") {
        if (argc < 3) {
            cout << "Please provide the number of tickets." << endl;
            return 1;
        }

        int number_tickets = atoi(argv[2]);

        if (argc < (3 + 2 * number_tickets)) {
            cout << "Insufficient seat position arguments." << endl;
            return 1;
        }

        vector<pair<int, int>> seat_positions; // To store seat positions

        // Retrieve seat positions from command line arguments
        for (int i = 0; i < number_tickets; i++) {
            int pos_i = atoi(argv[3 + 2 * i]); // Row
            int pos_j = atoi(argv[4 + 2 * i]); // Column
            seat_positions.push_back(make_pair(pos_i, pos_j));
        }

        // Book the seats
        for (const auto& pos : seat_positions) {
            int row = pos.first - 1; // Adjust for 0-based index
            int col = pos.second - 1; // Adjust for 0-based index
            
            // Call to book the seat
            s1.book_seat(row, col);
        }

        // Display the updated seat matrix after booking
        cout << "Updated Seat Matrix after Booking:" << endl;
        s1.display_seat_Matrix();
        cout << "Total seats booked: " << s1.get_booked_seat_count() << endl; // Show booked seat count
    } 
    // Display the seat matrix
    else if (command == "seats") {
        s1.display_seat_Matrix(); // Display current seat matrix
    }
    else {
        cout << "Invalid command. Available commands are:" << endl;
        cout << "  flights <airportCode> - Display flights from the given airport." << endl;
        cout << "  book <numTickets> <row1> <col1> <row2> <col2>... - Book seats." << endl;
        cout << "  seats - Display the current seat matrix." << endl;
    }

    return 0;
}
