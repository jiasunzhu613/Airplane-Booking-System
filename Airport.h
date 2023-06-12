//
// Created by Jonathan Zhu on 2023-06-06.
//
#pragma once

#include <string>
#include <unordered_map>

using std::string, std::unordered_map;

#ifndef AIRPLANE_BOOKING_SYSTEM_AIRPORT_H
#define AIRPLANE_BOOKING_SYSTEM_AIRPORT_H


class Airport {
private:
    string code;
    string name;
    unordered_map<string, int> timesToAirport;
public:
    Airport();
    Airport(string c, string n);
    string getCode() const;
    string getName() const;
    unordered_map<string, int> getTimesToAirport() const;
    void setCode(string c);
    void setName(string n);
    void addTimeToAirport(string code, int t);
};


#endif //AIRPLANE_BOOKING_SYSTEM_AIRPORT_H
