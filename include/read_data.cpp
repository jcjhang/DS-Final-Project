#include "read_data.h"

int read_data::get_station_num(){
    ifstream ifs("./test_case/station.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open station file.\n";
        return 0;
    }
    else {
        string s;
        while (std::getline(ifs, s)){
            station_num ++;
        }
    }
    ifs.close();
    return station_num;
}

my_graph* read_data::read_map(){
    // // calculate the station number
    // ifstream ifs_("./test_case/map.txt", ios::in);
    // int num = 0;
    // while(ifs_ >> start_station >> end_station >> distance){
    //     if(end_station > num){
    //         num = end_station;
    //     }
    // }
    // cout << "station num: " << num << endl;

    // read station data
    ifstream ifs("./test_case/map.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open map file.\n";
        return 0;
    }
    else {
        my_graph* mg = new my_graph();  // create an object pointer
        mg->initial_graph(station_num);
        while (ifs >> start_station >> end_station >> distance) {   // overloading >> operator?
            // add edges into graph
            mg->add_edge(start_station-1, end_station-1, distance); // station id start from 0
            // cout << "start: "<< start_station-1 << " end: " << end_station-1 << " distance: " << distance << "\n";
        }
        mg->print_graph();
        cout << "min distance: " << mg->dijkstra(0, 5) << endl;
        ifs.close();
        return mg;  // return the object pointer
    }
}

my_station** read_data::read_station(){
    ifstream ifs("./test_case/station.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open station file.\n";
        return 0;
    }
    else {
        my_station** station_ptr = new my_station*[station_num];  // station_ptr will store each station's object pointer
        while (ifs >> station_id >> num_elec >> num_lady >> num_road) {
            // add bikes_id into priority queue (inplemented by min heap) respectively
            my_station* ms = new my_station();
            my_MinHeap mm_elec(ms->elec);   // sent in a pointer point to its min heap (there will be multiple min heap)
            my_MinHeap mm_lady(ms->lady);
            my_MinHeap mm_road(ms->road);
            for(int i=0; i<num_elec; i++){
                mm_elec.insertKey(station_id*100 + i);
            }
            for(int i=0; i<num_lady; i++){
                mm_lady.insertKey(station_id*100 + i);
            }
            for(int i=0; i<num_road; i++){
                mm_road.insertKey(station_id*100 + i);
            }
            station_ptr[station_id-1] = ms; // station id start from 0
        }
        ifs.close();
        return station_ptr;
    }
}

void read_data::read_fee(){
    ifstream ifs("./test_case/fee.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open fee file.\n";
    }
    else {
        int count = 0;
        while (!ifs.eof( )) {
            // store the fee information
            if(count < 3){  // the first 3 lines
                ifs >> bike_type >> discount[count] >> regular[count];
            }
            else{
                ifs >> waiting;
                ifs >> switching;
                ifs >> transferring;
            }
            count ++;
            // cout << "eof: " << ifs.eof() << endl;
        }
        // cout << "discount elec: " << discount[0] << endl;
        // cout << "regular elec: " << regular[0] << endl;
        // cout << "discount lady: " << discount[1] << endl;
        // cout << "regular lady: " << regular[1] << endl;
        // cout << "discount road: " << discount[2] << endl;
        // cout << "regular road: " << regular[2] << endl;
        // cout << "waiting: " << waiting << endl;
        // cout << "switching: " << switching << endl;
        // cout << "transferring: " << transferring << endl;
    }
    ifs.close();
}