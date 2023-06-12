//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Flight.h"


Flight::Flight(string f, string t, int y, int m, int d, int h, int min) {
    passengers = array<Passenger*, NUM_OF_PASSENGERS>{}; // expressed as a json::array of passenger id's
    seatTaken = array<bool, NUM_OF_PASSENGERS>{}; // expressed as a Json::array
    time = array<system_clock::time_point, 2>{}; // expressed as a json::array
    from = f; // expressed as a airport 3-lettered code
    to = t; // expressed as a airport 3-lettered code (use as first key)

    // below: expressed as the default string conversion
    time[0] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
//    time[1] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min + flightDB.getAirports()[to].getTimesToAirport()[from]};
}

array<Passenger *, NUM_OF_PASSENGERS>& Flight::getPassengers() {return passengers;}

string Flight::getFrom() const {return from;}

string Flight::getTo() const {return to;}

string Flight::getDepartureTime() const {
    stringstream ss;
    ss << time[0];
    return ss.str();
}

string Flight::getArrivalTime() const {
    stringstream ss;
    ss << time[1];
    return ss.str();
}


void Flight::setFrom(string f) {from = f;}

void Flight::setTo(string t) {to = t;}

void Flight::setTime(int y, int m, int d, int h, int min) {
    time[0] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
//    time[1] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min + flightDB.getAirports()[to].getTimesToAirport()[from]};
}

void Flight::buySeat(Passenger passenger, int ind) {
    passengers[ind] = &passenger;
}
