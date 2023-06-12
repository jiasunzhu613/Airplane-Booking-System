//
// Created by Jonathan Zhu on 2023-06-06.
//
#pragma once

#include "Person.h"

#ifndef AIRPLANE_BOOKING_SYSTEM_PASSENGER_H
#define AIRPLANE_BOOKING_SYSTEM_PASSENGER_H


class Passenger : public Person{
private:
    string passengerID;
    string password;
public:
    Passenger();
    Passenger(string, string, string, string);//fn, ln, id, pw
    Passenger(string, string, string, string, string, string);//fn, ln, add, phone, id, pw
    string getPassengerID() const;
    string getPassword() const;
    void setPassengerID(string);
    void setPassword(string p);
};


#endif //AIRPLANE_BOOKING_SYSTEM_PASSENGER_H
