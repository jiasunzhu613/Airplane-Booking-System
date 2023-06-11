//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Flight.h"

Flight::Flight(Airport *f, Airport *t, int y, int m, int d, int h, int min) {
    passengers = array<Passenger*, NUM_OF_PASSENGERS>{};
    seatTaken = array<bool, NUM_OF_PASSENGERS>{};
    time = array<system_clock::time_point, 2>{};
    from = f;
    to = t;
    time[0] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
    //TODO: fetch .json file flight length and add it to min value
//    time[1] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
}

array<Passenger *, NUM_OF_PASSENGERS> Flight::getPassengers() const {return passengers;}

Airport* Flight::getFrom() const {return from;}

Airport* Flight::getTo() const {return to;}

array<system_clock::time_point, 2> Flight::getTime() const {return time;}

void Flight::setFrom(Airport* f) {from = f;}

void Flight::setTo(Airport* t) {to = t;}

void Flight::setTime(int y, int m, int d, int h, int min) {
    time[0] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
    //TODO: fetch .json file flight length and add it to min value
//    time[1] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
}

void Flight::buySeat(Passenger passenger, int ind) {
    passengers[ind] = &passenger;
}
