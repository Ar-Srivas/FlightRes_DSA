#include <iostream>
#include <sstream>
#include <vector>
#include "Seat_matrix.cpp"  // Adjust the filename based on your project structure
#include "GraphForAirportFlight1.cpp"  // Ensure the filename is correct

using namespace std;

void displayHelp() {
    cout << "Available commands are:" << endl;
    cout << "  add_flight <source> <destination> - Add a flight from source to destination." << endl;
    cout << "  flights <airportCode> - Display flights from the given airport." << endl;
    cout << "  book <numTickets> <row1> <col1> <row2> <col2>... - Book seats." << endl;
    cout << "  seats - Display the current seat matrix." << endl;
}

int main(int argc, char* argv[])
{
    Seat_mat s1;  // Create a Seat_mat object
    AirportGraph flightMap;

    // Adding initial flights to the airport graph
    flightMap.addFlight("DEL", "BOM");  // Delhi to Mumbai
flightMap.addFlight("DEL", "BLR");  // Delhi to Bangalore
flightMap.addFlight("DEL", "HYD");  // Delhi to Hyderabad
flightMap.addFlight("DEL", "CCU");  // Delhi to Kolkata
flightMap.addFlight("DEL", "AMD");  // Delhi to Ahmedabad
flightMap.addFlight("BOM", "BLR");  // Mumbai to Bangalore
flightMap.addFlight("BOM", "CCU");  // Mumbai to Kolkata
flightMap.addFlight("BOM", "HYD");  // Mumbai to Hyderabad
flightMap.addFlight("BOM", "COK");  // Mumbai to Kochi
flightMap.addFlight("BLR", "HYD");  // Bangalore to Hyderabad
flightMap.addFlight("BLR", "CCU");  // Bangalore to Kolkata
flightMap.addFlight("BLR", "MAA");  // Bangalore to Chennai
flightMap.addFlight("HYD", "CCU");  // Hyderabad to Kolkata
flightMap.addFlight("HYD", "MAA");  // Hyderabad to Chennai
flightMap.addFlight("MAA", "COK");  // Chennai to Kochi
flightMap.addFlight("COK", "TRV");  // Kochi to Thiruvananthapuram
flightMap.addFlight("CCU", "AMD");  // Kolkata to Ahmedabad
flightMap.addFlight("CCU", "JAI");  // Kolkata to Jaipur
flightMap.addFlight("DEL", "GOI");  // Delhi to Goa
flightMap.addFlight("DEL", "VAR");  // Delhi to Varanasi
flightMap.addFlight("BOM", "GOI");  // Mumbai to Goa


    // Check command line arguments
    if (argc < 2) {
        cout << "Invalid arguments. Please provide a command." << endl;
        displayHelp();
        return 1;
    }

    string command = argv[1];

    // Add flights to the airport graph dynamically
    if (command == "add_flight") {
        if (argc != 4) {
            cout << "Usage: add_flight <source> <destination>" << endl;
            return 1;
        }
        string source = argv[2];
        string destination = argv[3];
        flightMap.addFlight(source, destination);
        cout << "Flight added from " << source << " to " << destination << "." << endl;
    }
    // Display available flights
    else if (command == "flights") {
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
        cout << "Invalid command." << endl;
        displayHelp();
    }

    return 0;
}
