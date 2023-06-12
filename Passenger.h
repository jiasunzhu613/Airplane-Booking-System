//
// Created by Jonathan Zhu on 2023-06-06.
//
#include "Person.h"

#ifndef AIRPLANE_BOOKING_SYSTEM_PASSENGER_H
#define AIRPLANE_BOOKING_SYSTEM_PASSENGER_H


class Passenger : public Person{
private:
    string passengerID;
public:
    Passenger();
    Passenger(string, string, string);//fn, ln, id
    Passenger(string, string, string, string, string);//fn, ln, add, phone, id
    string getPassengerID() const;
    void setPassengerID(string);
};


#endif //AIRPLANE_BOOKING_SYSTEM_PASSENGER_H
