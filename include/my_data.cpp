#include "my_data.h"

int my_data::get_station_num(){
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

my_graph* my_data::read_map(){
    // read station data
    ifstream ifs("./test_case/map.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open map file.\n";
        return 0;
    }
    else {
        my_graph* graph_ptr = new my_graph();  // create an object pointer
        graph_ptr->initial_graph(station_num);
        while (ifs >> start_station >> end_station >> distance) {   // overloading >> operator?
            // add edges into graph
            graph_ptr->add_edge(start_station-1, end_station-1, distance); // station id start from 0
            // cout << "start: "<< start_station-1 << " end: " << end_station-1 << " distance: " << distance << "\n";
        }
        // graph_ptr->print_graph();
        // cout << "min distance: " << graph_ptr->dijkstra(0, 5) << endl;
        ifs.close();
        return graph_ptr;  // return the object pointer
    }
}

my_station** my_data::read_station(){
    ifstream ifs("./test_case/station.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open station file.\n";
        return 0;
    }
    else {
        my_station** station_ptr = new my_station*[station_num];  // station_ptr point to an array, and the array will store each station's object pointer
        while (ifs >> station_id >> num_elec >> num_lady >> num_road) {
            // add bikes_id into priority queue (inplemented by min heap) respectively
            my_station* ms = new my_station();
            my_MinHeap mm_elec(ms->electric, 0);   // sent in a pointer point to its min heap (there will be multiple min heap), and the initial heap size
            my_MinHeap mm_lady(ms->lady, 0);
            my_MinHeap mm_road(ms->road, 0);
            for(int i=0; i<num_elec; i++){
                mm_elec.insertKey(station_id*100 + i);
            }
            for(int i=0; i<num_lady; i++){
                mm_lady.insertKey(station_id*100 + i);
            }
            for(int i=0; i<num_road; i++){
                mm_road.insertKey(station_id*100 + i);
            }
            station_ptr[station_id-1] = ms; // station id start from 0, storing the station's object pointer
        }
        ifs.close();
        return station_ptr;
    }
}

void my_data::read_fee(){
    ifstream ifs("./test_case/fee.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open fee file.\n";
    }
    else {
        int count = 0;
        while (!ifs.eof()) {
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
        }
    }
    ifs.close();
}

int my_data::read_user(){
    ifstream ifs("./test_case/user.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open user file.\n";
        return 0;
    }
    else {
        string s;
        while (std::getline(ifs, s)){
            user_num ++;
        }
    }
    ifs.close();
    return user_num / 2;
}