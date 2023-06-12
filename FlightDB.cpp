//
// Created by Jonathan Zhu on 2023-06-11.
//

#include "FlightDB.h"

FlightDB::FlightDB() {
    flights = vector<Flight>{};
    passengers = unordered_map<string, Passenger>{};
    db = Database{};
}

vector<Flight>& FlightDB::getFlights() {return flights;}
unordered_map<string, Passenger>& FlightDB::getPassengers() {return passengers;}
unordered_map<string, Airport>& FlightTimesDB::getAirports() {return airports;}

void FlightDB::addFlight(Flight flight) {flights.push_back(flight);}
void FlightDB::addPassenger(Passenger passenger) {
    passengers[passenger.getPassengerID()] = passenger;
}
void FlightTimesDB::addAirport(Airport airport) {airports[airport.getCode()] = airport;}

void FlightDB::reset() {db.loadFlightDB(this);}
void FlightDB::save() {db.saveFlightDB(this);}