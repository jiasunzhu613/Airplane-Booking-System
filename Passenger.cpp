//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Passenger.h"
Passenger::Passenger(string fn, string ln, string id): Person(fn, ln), passengerID{id} {}
Passenger::Passenger(string fn, string ln, string a, string p, string id): Person(fn, ln, a, p), passengerID{id} {}

string Passenger::getPassengerID() const {return passengerID;}

void Passenger::setPassengerID(std::string id) {passengerID = id;}


