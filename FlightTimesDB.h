//
// Created by Jonathan Zhu on 2023-06-11.
//
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Airport.h"
#include "Database.h"

using std::unordered_map, std::string, std::stringstream;


#ifndef AIRPLANE_BOOKING_SYSTEM_FLIGHTTIMESDB_H
#define AIRPLANE_BOOKING_SYSTEM_FLIGHTTIMESDB_H

class FlightTimesDB {
private:
    unordered_map<string, Airport> airports;
    Database db;

public:
    FlightTimesDB();

    unordered_map<string, Airport>& getAirports();

    void addAirport(Airport airport);

    void reset();
    void save();

    friend void Database::loadFlightTimes(FlightTimesDB*);
    friend void Database::saveFlightTimes(FlightTimesDB*);
};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHTTIMESDB_H
