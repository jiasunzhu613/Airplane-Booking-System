//
// Created by Jonathan Zhu on 2023-06-06.
//

#include "Database.h"

Database::Database(){
    in.open(FLIGHTS_FILE_LOCATION);
    jsonReader.parse(in, flights_db);
}


