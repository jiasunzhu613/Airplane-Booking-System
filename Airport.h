//
// Created by Jonathan Zhu on 2023-06-06.
//
#include <string>

using std::string;

#ifndef AIRPLANE_BOOKING_SYSTEM_AIRPORT_H
#define AIRPLANE_BOOKING_SYSTEM_AIRPORT_H


class Airport {
private:
    string code;
    string name;
public:
    Airport(string c, string n);
    string getCode() const;
    string getName() const;
    void setCode(string c);
    void setName(string n);
};


#endif //AIRPLANE_BOOKING_SYSTEM_AIRPORT_H
