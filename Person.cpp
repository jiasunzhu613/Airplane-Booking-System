//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Person.h"

Person::Person() {}
Person::Person(string fn, string ln): firstName{fn}, lastName{ln}{}
Person::Person(string fn, string ln, string a, string p): firstName{fn}, lastName{ln}, address{a}, phone{p}{}

string Person::getFirstName() const {return firstName;}
string Person::getLastName() const {return lastName;}
string Person::getAddress() const {return address;}
string Person::getPhoneNumber() const {return phone;}

void Person::setFirstName(string in) {firstName = in;}
void Person::setLastName(string in) {lastName = in;}
void Person::setAddress(string in) {address = in;}
void Person::setPhoneNumber(string in) {phone = in;}
