//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Airport.h"

Airport::Airport(std::string c, std::string n): code{c}, name{n} {}

string Airport::getCode() const {return code;}

string Airport::getName() const {return name;}

void Airport::setCode(std::string c) {code = c;}

void Airport::setName(std::string n) {name = n;}