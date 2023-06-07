//
// Created by Jonathan Zhu on 2023-06-06.
//
/*
 *
 */
#include "Passenger.h"
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
    string destination;
    int time;

public:
    Flight();
    Flight(string dest, int t);
    array<Passenger*, NUM_OF_PASSENGERS> getPassengers() const;
    void buySeat(Passenger passenger, int ind);

};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHT_H
