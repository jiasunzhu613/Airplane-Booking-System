//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Attendent.h"
Attendent::Attendent(string fn, string ln, string id): Person(fn, ln), attendentID{id} {}
Attendent::Attendent(string fn, string ln, string a, string p, string id): Person(fn, ln, a, p), attendentID{id} {}

string Attendent::getAttendentID() const {return attendentID;}

void Attendent::setAttendentID(std::string id) {attendentID = id;}
