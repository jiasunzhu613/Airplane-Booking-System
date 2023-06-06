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


class Flight {
private:
    const static int NUM_OF_PASSENGERS = 11;
    array<Passenger*, NUM_OF_PASSENGERS> passengers{};
    
public:
    array<Passenger*, NUM_OF_PASSENGERS> getPassengers() const;
    void buySeat(Passenger passenger, int ind);

};


#endif //AIRPLANE_BOOKING_SYSTEM_FLIGHT_H
