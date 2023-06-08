//
// Created by Jonathan Zhu on 2023-06-06.
//
/*
 *
 */
#include <utility>
#include <chrono>
#include "Passenger.h"
#include "Airport.h"
#include <vector>
#include <array>

using std::vector, std::array;

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
    array<Passenger*, NUM_OF_PASSENGERS> passengers{};
    array<bool, NUM_OF_PASSENGERS> seatTaken{};
    Airport* from;
    Airport* to;
    int time;

public:
    Flight(Airport* f, Airport* t, int tt);
    array<Passenger*, NUM_OF_PASSENGERS> getPassengers() const;
    Airport* getFrom() const;
    Airport* getTo() const;
    int getTime() const;
    void setFrom(Airport* f);
    void setTo(Airport* t);
    void setTime(int t);
    void buySeat(Passenger passenger, int ind);
};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHT_H
