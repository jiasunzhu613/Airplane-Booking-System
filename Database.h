//
// Created by Jonathan Zhu on 2023-06-06.
//
#pragma once

#include <string>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <fstream>
#include <sstream>

using std::string, std::cout, std::endl, std::ifstream, std::ofstream;

#ifndef AIRPLANE_BOOKING_SYSTEM_DATABASE_H
#define AIRPLANE_BOOKING_SYSTEM_DATABASE_H

const static string FILE_LOCATION = "../jsonFiles/flightsDB.json";

class FlightDB;
class Database {
private:
    ifstream in;
    ofstream out;
    Json::Value db;
    Json::Reader jsonReader;
public:
    Database();

    void load(FlightDB*);
    void save(FlightDB*);
};


#endif //AIRPLANE_BOOKING_SYSTEM_DATABASE_H
