//
// Created by Jonathan Zhu on 2023-06-06.
//
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

const static string FLIGHTS_FILE_LOCATION = "../flights.json";

class Database {
private:
    ifstream in;
    ofstream out;
    Json::Value flights_db;
    Json::Reader jsonReader;
public:
    Database();


};


#endif //AIRPLANE_BOOKING_SYSTEM_DATABASE_H
