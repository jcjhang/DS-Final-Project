#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "my_DS.h"

using namespace std;

class read_data{
    // friend class my_DS; // not causing error ?
    public:
        int get_station_num();
        my_graph* read_map();
        my_station** read_station();
        void read_fee();
        void read_user();
        // for fee
        int discount[3];    // elec, lady, road
        int regular[3];
        int waiting;
        float switching;    // bug: if switching is "int", eof will always be 0 (false)
        int transferring;
    
    private:
        // for map
        int start_station;
        int end_station;
        int distance;   // required time between stations
        // for station
        int station_id;
        int station_num = 0;
        int num_elec;
        int num_lady;
        int num_road;
        // for user
        int user;
        int rent_time;
        string bike_type;
        int bike_id;
};