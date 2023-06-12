//
// Created by Jonathan Zhu on 2023-06-11.
//
#include <vector>
#include <unordered_map>
#include "Flight.h"
#include "Passenger.h"

using std::vector, std::unordered_map;

#ifndef AIRPLANE_BOOKING_SYSTEM_FLIGHTDB_H
#define AIRPLANE_BOOKING_SYSTEM_FLIGHTDB_H


class FlightDB {
private:
    //TODO: either vector or unordered_map (dont know what to put as key if use unordered_map)
    vector<Flight> flights;
    unordered_map<string, Passenger> passengers;
    unordered_map<string, Airport> airports;
    Database db;
public:
    FlightDB();
    vector<Flight>& getFlights();
    unordered_map<string, Passenger>& getPassengers();
    unordered_map<string, Airport>& getAirports();

    void addFlight(Flight flight);
    void addPassenger(Passenger passenger);
    void addAirport(Airport airport);

    void reset();
    void save();

    friend void Database::load(FlightDB*);
    friend void Database::save(FlightDB*);
};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHTDB_H
