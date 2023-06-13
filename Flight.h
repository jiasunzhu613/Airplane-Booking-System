//
// Created by Jonathan Zhu on 2023-06-06.
//
#pragma once

#include <vector>
#include <array>
#include <utility>
#include <chrono>
#include <sstream>
#include "date/date.h"
#include "Passenger.h"
#include "Attendent.h"
#include "Airport.h"
//#include "FlightDB.h"

using std::vector, std::array, std::pair, std::stringstream;
using namespace std::chrono;
using namespace date;

#ifndef AIRPLANE_BOOKING_SYSTEM_FLIGHT_H
#define AIRPLANE_BOOKING_SYSTEM_FLIGHT_H

/*
 * - have json
 * - have designated flight to and from locations
 * - have designated flight times
 * - all start location is ottawa
 * - time on json is measured in minutes
 */
const static int NUM_OF_PASSENGERS = 10;
//static FlightDB flightDB{}; // TODO: review later if we even need this variable

class Flight {
private:
    array<Passenger*, NUM_OF_PASSENGERS> passengers;
    array<bool, NUM_OF_PASSENGERS> seatTaken;
    string from;
    string to;
    string flightID;
    string attendentID;
    array<system_clock::time_point, 2> time;

public:
    Flight();
    Flight(string f, string t, string id, string attendentID, int y, int m, int d, int h, int min, int tt);
    array<Passenger*, NUM_OF_PASSENGERS>& getPassengers();
    string getFrom() const;
    string getTo() const;
    string getFlightID() const;
    string getAttedentID() const;
    array<bool, NUM_OF_PASSENGERS>& getSeatTaken();
    string getSeatsNotTaken();
    string getDepartureTime() const;
    string getArrivalTime() const;
    void setFrom(string f);
    void setTo(string t);
    void setFlightID(string id);
    void setAttendentID(string id);
    void setTime(int y, int m, int d, int h, int min, int t);
    void buySeat(Passenger* passenger, int ind);
    void removeSeat(int ind);
    string toString();
    string getDetails();
};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHT_H
