//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Flight.h"

Flight::Flight(Airport *f, Airport *t, int tt): from{f}, to{t}, time{tt}{}

array<Passenger *, NUM_OF_PASSENGERS> Flight::getPassengers() const {return passengers;}

Airport* Flight::getFrom() const {return from;}

Airport* Flight::getTo() const {return to;}

int Flight::getTime() const {return time;}

void Flight::setFrom(Airport* f) {from = f;}

void Flight::setTo(Airport* t) {to = t;}

void Flight::setTime(int t) {time = t;}

void Flight::buySeat(Passenger passenger, int ind) {
    passengers[ind] = &passenger;
}
