#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Flight {
public:
    string destination;

    Flight(const string& dest) : destination(dest) {}
};

class AirportGraph {
    map<string, vector<Flight>> adjList;

public:
    // Add a flight from source to destination
    void addFlight(const string& source, const string& destination) {
        adjList[source].emplace_back(destination); // Use emplace_back for efficiency
    }

    // Get all flights from a given airport code
    string getFlightsFrom(const string& airportCode) const {
        auto it = adjList.find(airportCode);
        if (it == adjList.end()) {
            return "No flights found from airport " + airportCode + "\n";
        }

        stringstream ss;
        for (const Flight& flight : it->second) {
            ss << "To " << flight.destination << "\n"; 
        }
        return ss.str();
    }

    // Function to print all flights (for debugging or external calls)
    void printAllFlights() const {
        for (const auto& entry : adjList) {
            cout << "Flights from " << entry.first << ":\n";
            for (const Flight& flight : entry.second) {
                cout << "To " << flight.destination << "\n";
            }
        }
    }
};
