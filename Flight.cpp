//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Flight.h"

Flight::Flight(){}

Flight::Flight(string f, string t, string id, string id2, int y, int m, int d, int h, int min, int tt) {
    passengers = array<Passenger*, NUM_OF_PASSENGERS>{}; // expressed as a json::array of passenger id's
    passengers.fill(new Passenger{});
    seatTaken = array<bool, NUM_OF_PASSENGERS>{}; // expressed as a Json::array
    seatTaken.fill(false);

    time = array<system_clock::time_point, 2>{}; // expressed as a json::array
    time.fill(system_clock::now());
    from = f; // expressed as a airport 3-lettered code
    to = t; // expressed as a airport 3-lettered code (use as first key)
    flightID = id;
    attendentID = id2;

    // below: expressed as the default string conversion
    time[0] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
    time[1] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min + tt};
}

array<Passenger*, NUM_OF_PASSENGERS>& Flight::getPassengers() {return passengers;}

string Flight::getFrom() const {return from;}

string Flight::getTo() const {return to;}

string Flight::getFlightID() const {return flightID;}

string Flight::getAttedentID() const {return attendentID;}

array<bool, NUM_OF_PASSENGERS>& Flight::getSeatTaken() {return seatTaken;}

string Flight::getSeatsNotTaken() {
    stringstream ss;
    for (int i = 0; i < NUM_OF_PASSENGERS; i++){
        if (!seatTaken[i]){
            ss << i << " ";
        }
    }
}

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

void Flight::setFlightID(std::string id) {flightID = id;}

void Flight::setAttendentID(std::string id) {attendentID = id;}

void Flight::setTime(int y, int m, int d, int h, int min, int t) {
    time[0] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min};
    time[1] = sys_days{date::year{y}/m/d} + hours{h} + minutes{min + t};
}

void Flight::buySeat(Passenger* passenger, int ind) {
    passengers[ind] = passenger;
    seatTaken[ind] = true;
}

void Flight::removeSeat(int ind) {
    seatTaken[ind] = false;
//    delete passengers[ind];
    passengers[ind] = new Passenger{};
}

string Flight::toString() {
    stringstream ss;
    ss << flightID;
    return ss.str();
}

string Flight::getDetails() {
    stringstream ss;
    ss << "To: " << to << std::endl;
    ss << "From: " << from << std::endl;
    ss << "Flight ID: " << flightID << std::endl;
    ss << "Departure: " << time[0] << std::endl;
    ss << "Arrival: " << time[1] << std::endl;
    ss << "Seats Available: ";
    for (int i = 0; i < NUM_OF_PASSENGERS; i++){
        if (!seatTaken[i]){
            ss << i << " ";
        }
    }
    return ss.str();
}
