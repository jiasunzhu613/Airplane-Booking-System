//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Airport.h"

Airport::Airport() {
    code = "";
    name = "";
    timesToAirport = unordered_map<string, int>{};
}

Airport::Airport(std::string c, std::string n){
    code = c;
    name = n;
    timesToAirport = unordered_map<string, int>{};
}

string Airport::getCode() const {return code;}

string Airport::getName() const {return name;}

unordered_map<string, int> Airport::getTimesToAirport() const {return timesToAirport;}

void Airport::setCode(std::string c) {code = c;}

void Airport::setName(std::string n) {name = n;}

void Airport::addTimeToAirport(string code, int t) {timesToAirport.insert(std::make_pair(code, t));}