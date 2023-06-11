//
// Created by Jonathan Zhu on 2023-06-11.
//

#include "FlightTimesDB.h"

FlightTimesDB::FlightTimesDB(){
    Database db{};
    airports = unordered_map<string, Airport>{};
}
