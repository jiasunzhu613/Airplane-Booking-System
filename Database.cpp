//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Database.h"
#include "FlightTimesDB.h"

Database::Database(){
    in.open(FLIGHTS_FILE_LOCATION);
    jsonReader.parse(in, flights_db);
}

void Database::load(FlightTimesDB* inMemoryDB){
    for (const string to : flights_db.getMemberNames()){
        Json::Value& airportJSON = flights_db[to];
        string name = airportJSON["Name"].asString();
        inMemoryDB->airports[to] = Airport{to, name};
        Json::Value& distancesJSON = airportJSON["Distances"];
        for (const string from : distancesJSON.getMemberNames()){
            inMemoryDB->airports[to].addTimeToAirport(from, distancesJSON[from].asInt());
        }
    }
}

void Database::save(FlightTimesDB* inMemoryDB){
    Json::Value ndb;

    for (auto[code, airport] : inMemoryDB->airports){
        Json::Value& airportJSON = ndb[code];
        airportJSON["Name"] = airport.getName();
        Json::Value& distancessJSON = airportJSON["Distances"];
        for (auto[from, time] : airport.getTimesToAirport()){
            distancessJSON[from] = time;
        }
    }

    flights_db = ndb;
    out.open(FLIGHTS_FILE_LOCATION);
    out << flights_db;
    out.close();
}


