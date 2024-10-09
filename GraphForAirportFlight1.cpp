#include <iostream>
#include <map>
#include <vector>
#include <string>
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

    void displayFlightsFrom(const string& airportCode) {
        if (adjList.find(airportCode) == adjList.end()) {
            cout << "No flights found from airport " << airportCode << endl;
            return;
        }

        cout << "Flights from " << airportCode << ":" << endl;
        for (const Flight& flight : adjList[airportCode]) {
            cout << "  -> To " << flight.destination << endl;
        }
    }
};

int main() {
    AirportGraph flightMap;

    flightMap.addFlight("JFK", "LAX");
    flightMap.addFlight("JFK", "ORD");
    flightMap.addFlight("LAX", "SFO");
    flightMap.addFlight("ORD", "DFW");

    string airportCode;
    cout << "Enter the airport code to display flights from: ";
    cin >> airportCode;

    flightMap.displayFlightsFrom(airportCode);

    return 0;
}