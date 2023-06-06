//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Flight.h"

const static int NUM_OF_PASSENGERS = 11;

array<Passenger *, NUM_OF_PASSENGERS> Flight::getPassengers() const {
    return passengers;
}

void Flight::buySeat(Passenger passenger, int ind) {
    passengers[ind] = &passenger;
}
