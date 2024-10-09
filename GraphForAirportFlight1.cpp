#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream> // Include stringstream for string handling
using namespace std;

class Flight {
public:
    string destination;

    Flight(string dest) {
        destination = dest;
    }
};

class AirportGraph {
    map<string, vector<Flight>> adjList;

public:
    void addFlight(const string& source, const string& destination) {
        adjList[source].push_back(Flight(destination));
    }

    // Modify to return a string of flight details instead of printing
    string getFlightsFrom(const string& airportCode) {
        if (adjList.find(airportCode) == adjList.end()) {
            return "No flights found from airport " + airportCode + "\n"; // Return error message
        }

        stringstream ss;
        ss << "Flights from " << airportCode << ":\n"; // Collect flights in a string
        for (const Flight& flight : adjList[airportCode]) {
            ss << "To " << flight.destination << "\n"; // Add flight info to the string
        }
        return ss.str(); // Return the collected flight information
    }
};
