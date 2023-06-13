//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Attendent.h"

Attendent::Attendent() {}
Attendent::Attendent(string fn, string ln, string id, string pw): Person(fn, ln), attendentID{id}, password{pw} {}
Attendent::Attendent(string fn, string ln, string a, string p, string id, string pw): Person(fn, ln, a, p), attendentID{id}, password{pw} {}

string Attendent::getAttendentID() const {return attendentID;}

string Attendent::getPassword() const {return password;}

void Attendent::setAttendentID(std::string id) {attendentID = id;}

void Attendent::setPassword(std::string pw) {password = pw;}
