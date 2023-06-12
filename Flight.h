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
#include "Airport.h"
#include "FlightDB.h"

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
const static int NUM_OF_PASSENGERS = 11;
//static FlightDB flightDB{}; // TODO: review later if we even need this variable

class Flight {
private:
    array<Passenger*, NUM_OF_PASSENGERS> passengers;
    array<bool, NUM_OF_PASSENGERS> seatTaken;
    string from;
    string to;
    array<system_clock::time_point, 2> time;

public:
    Flight(string f, string t, int y, int m, int d, int h, int min);
    array<Passenger*, NUM_OF_PASSENGERS>& getPassengers();
    string getFrom() const;
    string getTo() const;
    array<bool, NUM_OF_PASSENGERS>& getSeatTaken();
    string getDepartureTime() const;
    string getArrivalTime() const;
    void setFrom(string f);
    void setTo(string t);
    void setTime(int y, int m, int d, int h, int min);
    void buySeat(Passenger passenger, int ind);
};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHT_H
