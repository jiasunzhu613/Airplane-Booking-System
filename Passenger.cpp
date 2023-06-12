//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Passenger.h"
Passenger::Passenger(){
    passengerID = "invalid id";
    password = "";
}
Passenger::Passenger(string fn, string ln, string id, string pw): Person(fn, ln), passengerID{id}, password{pw} {}
Passenger::Passenger(string fn, string ln, string a, string p, string id, string pw): Person(fn, ln, a, p), passengerID{id}, password{pw}  {}

string Passenger::getPassengerID() const {return passengerID;}

string Passenger::getPassword() const {return password;}

void Passenger::setPassengerID(std::string id) {passengerID = id;}

void Passenger::setPassword(std::string p) {password = p;}


