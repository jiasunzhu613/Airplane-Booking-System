//
// Created by Jonathan Zhu on 2023-06-06.
//
#pragma once

#include "Person.h"

#ifndef AIRPLANE_BOOKING_SYSTEM_ATTENDENT_H
#define AIRPLANE_BOOKING_SYSTEM_ATTENDENT_H


class Attendent : public Person{
private:
    string attendentID;
public:
    Attendent();
    Attendent(string, string, string);//fn, ln, id
    Attendent(string, string, string, string, string);//fn, ln, add, phone, id
    string getAttendentID() const;
    void setAttendentID(string);
};


#endif //AIRPLANE_BOOKING_SYSTEM_ATTENDENT_H
