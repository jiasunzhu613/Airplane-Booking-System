//
// Created by Jonathan Zhu on 2023-06-11.
//

#include "FlightDB.h"

FlightDB::FlightDB() {
    Database db{};
}

unordered_map<string, Flight>& FlightDB::getFlights() {return flights;}
unordered_map<string, Passenger>& FlightDB::getPassengers() {return passengers;}
unordered_map<string, Attendent>& FlightDB::getAttendents() {return attendents;}
unordered_map<string, Airport>& FlightDB::getAirports() {return airports;}

void FlightDB::addFlight(Flight flight) {flights[flight.getFlightID()] = flight;}
void FlightDB::addPassenger(Passenger passenger) {
    passengers[passenger.getPassengerID()] = passenger;
}
void FlightDB::addAttendent(Attendent attendent) {
    attendents[attendent.getAttendentID()] = attendent;
}
void FlightDB::addAirport(Airport airport) {airports[airport.getCode()] = airport;}

void FlightDB::reset() {db.load(this);}
void FlightDB::save() {db.save(this);}