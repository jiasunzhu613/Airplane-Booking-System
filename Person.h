//
// Created by Jonathan Zhu on 2023-06-06.
//
#pragma once

#include <string>

using std::string;

#ifndef AIRPLANE_BOOKING_SYSTEM_PERSON_H
#define AIRPLANE_BOOKING_SYSTEM_PERSON_H


class Person {
private:
    string firstName;
    string lastName;
    string address;
    string phone;
public:
    //Constructors
    Person();
    Person(string, string); // first and last
    Person(string, string, string, string); // all inputs

    //Accessors and Mutators
    string getFirstName() const;
    string getLastName() const;
    string getAddress() const;
    string getPhoneNumber() const;
    void setFirstName(string);
    void setLastName(string);
    void setAddress(string);
    void setPhoneNumber(string);
};


#endif //AIRPLANE_BOOKING_SYSTEM_PERSON_H
