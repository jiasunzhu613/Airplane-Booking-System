//
// Created by Jonathan Zhu on 2023-06-06.
//
#include "FlightDB.h"

using namespace std;


int main(){
    FlightDB db = FlightDB{};
    db.reset();
    db.addPassenger(Passenger{"Jon", "Zhu", "123 Sesame Street", "your mom", "a"});
    db.addPassenger(Passenger{"a", "b", "123 Sesame Street", "your mom", "aa"});
//    int y, m, d, h, min;
//    cin >> y >> m >> d >> h >> min;
    db.addFlight(Flight{"a", "b", "some id", 2014, 12, 4, 10, 30});
    db.getFlights()["some id"].buySeat(&db.getPassengers()["a"], 2);
    db.getFlights()["some id"].buySeat(&db.getPassengers()["a"], 3);
    db.getFlights()["some id"].buySeat(&db.getPassengers()["aa"], 1);
    db.getFlights()["some id"].buySeat(&db.getPassengers()["aa"], 0);



    db.addAirport(Airport{"MOM", "YOUR MOM INTERNATIONAL AIRPORT"});
    db.save();
    return 0;
}
