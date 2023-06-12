//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Database.h"
#include "FlightDB.h"

Database::Database(){
    in.open(FILE_LOCATION);
    jsonReader.parse(in, db);
}

void Database::load(FlightDB* inMemoryDB){
    //read in airports
    for (const string to : db["Airports"].getMemberNames()){
        Json::Value& airportJSON = db["Airports"][to];
        string name = airportJSON["Name"].asString();
        inMemoryDB->airports[to] = Airport{to, name};
        Json::Value& distancesJSON = airportJSON["Distances"];
        for (const string from : distancesJSON.getMemberNames()){
            inMemoryDB->airports[to].addTimeToAirport(from, distancesJSON[from].asInt());
        }
    }

    for (const string to : db["Flights"].getMemberNames()){
        Json::Value& flightJSON = db["Flights"][to];
        string from = flightJSON["From"].asString();
        //reference time format: 2022-03-12 05:45:00
        //2022-03-12 05:45:00
        //0123456789012345678
        //          ^here onwards add 10 to digit
        //do times first
        string time = flightJSON["Departure"].asString();
        int y = stoi(time.substr(0, 4));
        int m = stoi(time.substr(5, 7));
        int d = stoi(time.substr(8, 10));
        int h = stoi(time.substr(11, 13));
        int min = stoi(time.substr(14, 16));
        inMemoryDB->flights.push_back(Flight{from, to, y, m, d, h, min});
        for (int i = 0; i < flightJSON["Passengers"].size(); i++){
            inMemoryDB->flights[-1].getPassengers()[i] = &inMemoryDB->passengers[flightJSON["Passengers"][i].asString()];
        }
        for (int i = 0; i < flightJSON["Seats Taken"].size(); i++){
            inMemoryDB->flights[-1].getSeatTaken()[i] = flightJSON["Seats Taken"][i].asBool();
        }
        //then do seats and passengers, order is wtv
    }
    for (const string id : db["Passengers"].getMemberNames()){
        Json::Value& passengerJSON = db["Passengers"][id];
        string fn = passengerJSON["First Name"].asString();
        string ln = passengerJSON["Last Name"].asString();
        string address = passengerJSON["Address"].asString();
        string phone = passengerJSON["Phone Number"].asString();
        inMemoryDB->passengers[id] = Passenger{fn, ln, address, phone, id};
    }
}

void Database::save(FlightDB* inMemoryDB){
    Json::Value ndb;

    for (auto[code, airport] : inMemoryDB->airports){
        Json::Value& airportJSON = ndb["Airports"][code];
        airportJSON["Name"] = airport.getName();
        Json::Value& distancessJSON = airportJSON["Distances"];
        for (auto[from, time] : airport.getTimesToAirport()){
            distancessJSON[from] = time;
        }
    }

    for (auto[id, passenger] : inMemoryDB->passengers){
        Json::Value& passengerJSON = ndb["Passengers"][id];
        passengerJSON["First Name"] = passenger.getFirstName();
        passengerJSON["Last Name"] = passenger.getLastName();
        passengerJSON["Address"] = passenger.getAddress();
        passengerJSON["Phone Number"] = passenger.getPhoneNumber();
    }

    for (auto flight : inMemoryDB->flights){
        Json::Value& flightJSON = ndb["Flights"][flight.getTo()];
        flightJSON["From"] = flight.getFrom();
        flightJSON["Departure"] = flight.getDepartureTime();
        flightJSON["Passengers"] = Json::arrayValue;
        for (auto passenger : flight.getPassengers()){
            string id = passenger->getPassengerID();
            flightJSON["Passengers"].append(id);
        }
        flightJSON["Seats Taken"] = Json::arrayValue;
        for (bool b : flight.getSeatTaken()){
            flightJSON["Seats Taken"].append(b);
        }
    }
    db = ndb;
    out.open(FILE_LOCATION);
    out << db;
    out.close();
}


