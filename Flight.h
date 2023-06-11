//
// Created by Jonathan Zhu on 2023-06-06.
//
#include <vector>
#include <array>
#include <utility>
#include <chrono>
#include "date/date.h"
#include "Passenger.h"
#include "Airport.h"

using std::vector, std::array, std::pair;
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

class Flight {
private:
    array<Passenger*, NUM_OF_PASSENGERS> passengers;
    array<bool, NUM_OF_PASSENGERS> seatTaken;
    Airport* from;
    Airport* to;
    array<system_clock::time_point, 2> time;

public:
    Flight(Airport* f, Airport* t, int y, int m, int d, int h, int min);
    array<Passenger*, NUM_OF_PASSENGERS> getPassengers() const;
    Airport* getFrom() const;
    Airport* getTo() const;
    array<system_clock::time_point, 2> getTime() const;
    void setFrom(Airport* f);
    void setTo(Airport* t);
    void setTime(int y, int m, int d, int h, int min);
    void buySeat(Passenger passenger, int ind);
};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHT_H
