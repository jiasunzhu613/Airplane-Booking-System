//
// Created by Jonathan Zhu on 2023-06-11.
//

#include "FlightTimesDB.h"

FlightTimesDB::FlightTimesDB(){
    Database db{};
    airports = unordered_map<string, Airport>{};
}

unordered_map<string, Airport>& FlightTimesDB::getAirports() {return airports;}

void FlightTimesDB::addAirport(Airport airport) {airports[airport.getCode()] = airport;}

void FlightTimesDB::reset() {db.loadFlightTimes(this);}
void FlightTimesDB::save() {db.saveFlightTimes(this);}
