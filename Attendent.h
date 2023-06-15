//
// Created by Jonathan Zhu on 2023-06-06.
//
#pragma once

#include "Person.h"

#ifndef AIRPLANE_BOOKING_SYSTEM_ATTENDENT_H
#define AIRPLANE_BOOKING_SYSTEM_ATTENDENT_H


class Attendent : public Person{
private:
    //Variables
    string attendentID;
    string password;
public:
    //Constructors
    Attendent();
    Attendent(string, string, string, string);//fn, ln, id, pw
    Attendent(string, string, string, string, string, string);//fn, ln, add, phone, id, pw

    //Accessors and Mutators
    string getAttendentID() const;
    string getPassword() const;
    void setAttendentID(string);
    void setPassword(string pw);
};


#endif //AIRPLANE_BOOKING_SYSTEM_ATTENDENT_H
