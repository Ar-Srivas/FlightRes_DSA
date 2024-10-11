#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream> 
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
    string getFlightsFrom(const string& airportCode) {
        if (adjList.find(airportCode) == adjList.end()) {
            return "No flights found from airport " + airportCode + "\n";
        }

        stringstream ss;
        //ss << "Flights from " << airportCode << ":\n";
        for (const Flight& flight : adjList[airportCode]) {
            ss << "To " << flight.destination << "\n"; 
        }
        return ss.str();
    }
};
