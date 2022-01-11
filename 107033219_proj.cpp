#include "./include/my_data.h"

// rounded to the nearest integer
int round(float input){
    int lower = (int)input;
    if(input >= (lower+0.5))
        return lower+1;
    else
        return lower;
}

int main(){
{   // two parts scope
    // the variables that two parts share
    my_data read_data;
    int user_num = read_data.read_user();
    int station_num = read_data.get_station_num();
    int revenue_1 = 0;
    int reject_count_1 = 0;
    int reject_list[user_num][5]; // station_id, bike_type, user_id, rent_time, reject_count
    int reject_num = 0;

// ---------------------- part 1 ----------------------
{   // part 1 scope
    
    my_graph my_graph;
    my_graph = *(read_data.read_map());  // recieve a object's pointer of graph

    read_data.read_fee();
    
    my_station** my_bike_heap = read_data.read_station();  // my_bike_heap["station_id-1"] -> "bike_type"["bike_no.#_in_heap"]

    int rent_list[user_num][5]; // station_id, bike_type, user_id, rent_time, rent_bike_id
    int revenue = 0;
    int* heap_ptr = new int;    // create a pointer to the heap(array) address (later depends on station and bike_tyoe)

    ifstream ifs("./test_case/user.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open user file.\n";
        return 0;
    }
    else {
        string action;
        int station_id;
        string bike_type;
        string user_id_string;
        int user_id;
        int rent_time;
        int return_time;
        int rent_num = 0;
        int rent_bike_id;
        reject_num = 0;
        ofstream ofs;
        ofs.open("part1_response.txt", ios::out);
        while(!ifs.eof()){
            ifs >> action;
            if(action == "rent"){
                ifs >> station_id >> bike_type >> user_id_string >> rent_time;
                user_id = stoi(user_id_string);
                cout << action << " " << station_id << " " << bike_type << " " << user_id_string << " " << rent_time << endl;
                ofs << action << " " << station_id << " " << bike_type << " " << user_id_string << " " << rent_time << endl;
                rent_list[rent_num][0] = station_id;
                // rent_list[rent_num][1] = bike_type;
                rent_list[rent_num][2] = user_id;
                rent_list[rent_num][3] = rent_time;

                if(bike_type == "electric"){
                    rent_list[rent_num][1] = 0;
                    heap_ptr = my_bike_heap[station_id-1]->electric;
                }
                else if(bike_type == "lady"){
                    rent_list[rent_num][1] = 1;
                    heap_ptr = my_bike_heap[station_id-1]->lady;
                }
                else if(bike_type == "road"){
                    rent_list[rent_num][1] = 2;
                    heap_ptr = my_bike_heap[station_id-1]->road;
                }
                
                my_MinHeap* my_heap = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)

                if(my_heap->isEmpty()){
                    cout << "-------------------------- reject --------------------------" << endl;
                    ofs << "reject" << endl;
                    reject_count_1 ++;
                    reject_list[reject_num][0] = station_id;
                    reject_list[reject_num][1] = rent_list[rent_num][1];    // bike type
                    reject_list[reject_num][2] = user_id;
                    reject_list[reject_num][3] = rent_time;
                    reject_list[reject_num][4] = 1;
                    for(int i=0; i<reject_num; i++){
                        if(reject_list[i][0] == station_id && reject_list[i][1] == rent_list[rent_num][1])
                            reject_list[reject_num][4] += 1;
                    }
                    reject_num ++;
                    delete my_heap;
                    continue;   // rent_num will not ++, so rent_list will not include this trade
                }
                else{
                    cout << "accept" << endl;
                    ofs << "accept" << endl;
                    rent_bike_id = my_heap->extractMin();   // remove(rent) the bike
                    rent_list[rent_num][4] = rent_bike_id;
                }

                delete my_heap;
                rent_num ++;
            }
            else if(action == "return"){
                ifs >> station_id >> user_id_string >> return_time;
                user_id = stoi(user_id_string);
                cout << action << " " << station_id << " " << user_id_string << " " << return_time << endl;
                ofs << action << " " << station_id << " " << user_id_string << " " << return_time << endl;
                for(int i=0; i<rent_num; i++){  // not i <= rent_num, cuz rent_num will be the actual value +1 (++ at the last time)
                    if(user_id == rent_list[i][2]){ // success to rent before
                        // charge fee
                        int shortest_time = my_graph.dijkstra(rent_list[i][0]-1, station_id-1); // rent_list[i][0] is starting station
                        int user_time = return_time - rent_list[i][3];  // rent_list[i][3] is rent time
                        if(user_time == shortest_time){
                            revenue += (read_data.discount[rent_list[i][1]] * user_time); // rent_list[i][1] is rental bike type
                        }
                        else{
                            revenue += (read_data.regular[rent_list[i][1]] * user_time);
                        }

                        // return bike
                        if(rent_list[i][1] == 0){
                            heap_ptr = my_bike_heap[station_id-1]->electric;
                        }
                        else if(rent_list[i][1] == 1){
                            heap_ptr = my_bike_heap[station_id-1]->lady;
                        }
                        else if(rent_list[i][1] == 2){
                            heap_ptr = my_bike_heap[station_id-1]->road;
                        }
                        
                        my_MinHeap* my_heap = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
                        
                        my_heap->insertKey(rent_list[i][4]);    // rent_list[i][4] is rent_bike_id

                        delete my_heap;
                        rent_list[i][2] = 0;    // i.e. delete the rental record, avoiding the problem that same user rent bike later
                        break;
                    }
                }
            }
        }
    }
    ifs.close();


    // print status
    ofstream ofs;
    ofs.open("part1_status.txt", ios::out);
    if (!ofs.is_open()) {
        cout << "Failed to open file.\n";
    }
    else {
        cout << "-------------- status --------------" << endl;
        for(int i=0; i<station_num; i++){
            cout << i+1 << ":" << endl;
            ofs << i+1 << ":" << endl;
            // electric
            cout << "electric: ";
            ofs << "electric: ";
            heap_ptr = my_bike_heap[i]->electric;   // i is station_id-1
            my_MinHeap* my_heap_elec = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
            my_heap_elec->printHeapSort(ofs);
            delete my_heap_elec;
            // lady
            cout << "lady: ";
            ofs << "lady: ";
            heap_ptr = my_bike_heap[i]->lady;   // i is station_id-1
            my_MinHeap* my_heap_lady = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
            my_heap_lady->printHeapSort(ofs);
            delete my_heap_lady;
            // road
            cout << "road: ";
            ofs << "road: ";
            heap_ptr = my_bike_heap[i]->road;   // i is station_id-1
            my_MinHeap* my_heap_road = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
            my_heap_road->printHeapSort(ofs);
            delete my_heap_road;
        }
        cout << "revenue: " << revenue << endl;
        ofs << revenue;
        ofs.close();

        revenue_1 = revenue;
    }
}   // end of part 1 scope

// ---------------------- part 2 ----------------------
cout << "------------------------------------------------------------------------" << endl;
cout << "------------------------------------------------------------------------" << endl;
cout << "-------------------------------- part 2 --------------------------------" << endl;
cout << "------------------------------------------------------------------------" << endl;
cout << "------------------------------------------------------------------------" << endl;
{   // part 2 scope

    // declare new objects (different from part 1 but doing the similar things)
    my_data read_data;
    
    int station_num = read_data.get_station_num();

    my_graph my_graph;
    my_graph = *(read_data.read_map());  // recieve a object's pointer of graph

    read_data.read_fee();
    
    my_station** my_bike_heap = read_data.read_station();  // my_bike_heap["station_id-1"] -> "bike_type"["bike_no.#_in_heap"]

    int user_num = read_data.read_user();

    int rent_list[user_num][6]; // station_id, bike_type, user_id, rent_time, rent_bike_id, change bike or not
    int revenue = 0;
    int* heap_ptr = new int;    // create a pointer to the heap(array) address (later depends on station and bike_tyoe)

    int reject_count = 0;

    ofstream ofs;
    ofs.open("part2_response.txt", ios::out);

    // print reject list
    cout << "reject list:" << endl;
    for(int i=0; i<reject_num; i++){
        for(int j=0; j<5; j++){
            cout << reject_list[i][j] << " ";
        }
        cout << endl;
    }
    cout << "-------------------------" << endl;

    // variables for transfer
    int max_reject_station[3];  // electric, lady, road
    int max_reject_count[3];
    int max_num_station[3];
    int max_num[3];
    int transfer_num[3];
    int arrive_time[3];
    my_Algo algo(my_bike_heap);

    if(reject_num > 0){
        // find the station with max reject counts of each bike type (i.e. transfer destination)
        for(int i=0; i<3; i++){
            int max = 0;
            for(int j=0; j<station_num; j++){
                for(int k=0; k<reject_num; k++){
                    if(reject_list[k][1] == i && reject_list[k][0] == j+1){ // bike type that is being checked
                        if(reject_list[k][4] > max){
                            max = reject_list[k][4];
                            max_reject_station[i] = j+1;
                            max_reject_count[i] = max;
                        }
                    }
                }
            }
        }

        // find the station with max number of each bike at initial (i.e. transfer source)
        for(int i=0; i<3; i++){
            int max = 0;
            for(int j=0; j<station_num; j++){
                if(i == 0){
                    if(my_bike_heap[j]->electric[100] > max){
                        max = my_bike_heap[j]->electric[100];
                        max_num_station[i] = j+1;
                        max_num[i] = max;
                    }
                }
                if(i == 1){
                    if(my_bike_heap[j]->lady[100] > max){
                        max = my_bike_heap[j]->lady[100];
                        max_num_station[i] = j+1;
                        max_num[i] = max;
                    }
                }
                if(i == 2){
                    if(my_bike_heap[j]->road[100] > max){
                        max = my_bike_heap[j]->road[100];
                        max_num_station[i] = j+1;
                        max_num[i] = max;
                    }
                }
            }
        }

        // transfer number
        for(int i=0; i<3; i++){
            if(max_num[i] - max_reject_count[i] >= 3){
                transfer_num[i] = max_reject_count[i];  // if enough, transfer the needed number
            }
            else{
                transfer_num[i] = max_num[i] - 3;   // if not enough, preserve 3 bikes at least
            }
        }

        // arrive time
        for(int i=0; i<3; i++){
            arrive_time[i] = my_graph.dijkstra(max_num_station[i]-1, max_reject_station[i]-1);
        }
        
        // transfer the bikes from source station
        for(int i=0; i<3; i++){
            algo.transfer_leave(max_num_station[i], max_reject_station[i], i, transfer_num[i], 0);
            if(i == 0){
                cout << "transfer " << max_num_station[i] << " " << max_reject_station[i] << " electric " << transfer_num[i] << " " << 0 << endl;
                ofs << "transfer " << max_num_station[i] << " " << max_reject_station[i] << " electric " << transfer_num[i] << " " << 0 << endl;
            }
            if(i == 1){
                cout << "transfer " << max_num_station[i] << " " << max_reject_station[i] << " lady " << transfer_num[i] << " " << 0 << endl;
                ofs << "transfer " << max_num_station[i] << " " << max_reject_station[i] << " lady " << transfer_num[i] << " " << 0 << endl;
            }
            if(i == 2){
                cout << "transfer " << max_num_station[i] << " " << max_reject_station[i] << " road " << transfer_num[i] << " " << 0 << endl;
                ofs << "transfer " << max_num_station[i] << " " << max_reject_station[i] << " road " << transfer_num[i] << " " << 0 << endl;
            }
        }
    }

    ifstream ifs("./test_case/user.txt", ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open user file.\n";
        return 0;
    }
    else {
        string action;
        int station_id;
        string bike_type;
        string user_id_string;
        int user_id;
        int rent_time;
        int return_time;
        int rent_num = 0;
        int rent_bike_id;
        while(!ifs.eof()){
            ifs >> action;
            if(action == "rent"){
                ifs >> station_id >> bike_type >> user_id_string >> rent_time;
                user_id = stoi(user_id_string);
                cout << action << " " << station_id << " " << bike_type << " " << user_id_string << " " << rent_time << endl;
                ofs << action << " " << station_id << " " << bike_type << " " << user_id_string << " " << rent_time << endl;
                
                if(reject_num > 0){
                    // transfer bike arrive (if any)
                    for(int i=0; i<3; i++){
                        if(rent_time >= arrive_time[i]){
                            algo.transfer_arrive(max_num_station[i], max_reject_station[i], i, transfer_num[i], 0);
                            revenue -= read_data.transferring * my_graph.dijkstra(max_num_station[i]-1, max_reject_station[i]-1);
                            arrive_time[i] = 1450;  // impossibe time, cuz it's already arrived and finish transfer
                        }
                    }
                }
                rent_list[rent_num][0] = station_id;
                // rent_list[rent_num][1] = bike_type;
                rent_list[rent_num][2] = user_id;
                rent_list[rent_num][3] = rent_time;
                rent_list[rent_num][5] = 0; // default: doesn't change bike

                if(bike_type == "electric"){
                    rent_list[rent_num][1] = 0;
                    heap_ptr = my_bike_heap[station_id-1]->electric;
                }
                else if(bike_type == "lady"){
                    rent_list[rent_num][1] = 1;
                    heap_ptr = my_bike_heap[station_id-1]->lady;
                }
                else if(bike_type == "road"){
                    rent_list[rent_num][1] = 2;
                    heap_ptr = my_bike_heap[station_id-1]->road;
                }
                
                my_MinHeap* my_heap = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)

                if(my_heap->isEmpty()){
                    cout << "-------------------------- reject --------------------------" << endl;
                    algo.change_bike(station_id, rent_list[rent_num][1]);
                    if(algo.change_bike_type == 10000){   // other types at that station are also empty
                        cout << "----------------------- surely reject ----------------------" << endl;
                        ofs << "reject" << endl;
                        reject_count ++;
                        delete my_heap;
                        continue;   // rent_num will not ++, so rent_list will not include this trade
                    }
                    else{   // successfully change bike
                        cout << "successfully change bike !!!!!!" << endl;
                        if(algo.change_bike_type == 0){
                            cout << "discount electric" << endl;
                            ofs << "discount electric" << endl;
                        }
                        if(algo.change_bike_type == 1){
                            cout << "discount lady" << endl;
                            ofs << "discount lady" << endl;
                        }
                        if(algo.change_bike_type == 2){
                            cout << "discount road" << endl;
                            ofs << "discount road" << endl;
                        }
                        rent_list[rent_num][1] = algo.change_bike_type;  // update the rent list
                        rent_list[rent_num][4] = algo.change_bike_id;
                        rent_list[rent_num][5] = 1; // does change bike
                    }
                }
                else{
                    cout << "accept" << endl;
                    ofs << "accept" << endl;
                    rent_bike_id = my_heap->extractMin();   // remove(rent) the bike
                    rent_list[rent_num][4] = rent_bike_id;
                }

                delete my_heap;
                rent_num ++;
            }
            else if(action == "return"){
                ifs >> station_id >> user_id_string >> return_time;
                user_id = stoi(user_id_string);
                cout << action << " " << station_id << " " << user_id_string << " " << return_time << endl;
                ofs << action << " " << station_id << " " << user_id_string << " " << return_time << endl;
                
                if(reject_num > 0){
                    // transfer bike arrive (if any)
                    for(int i=0; i<3; i++){
                        if(return_time >= arrive_time[i]){
                            algo.transfer_arrive(max_num_station[i], max_reject_station[i], i, transfer_num[i], 0);
                            revenue -= read_data.transferring * my_graph.dijkstra(max_num_station[i]-1, max_reject_station[i]-1);
                            arrive_time[i] = 1450;  // impossibe time, cuz it's already arrived and finish transfer
                        }
                    }
                }
                for(int i=0; i<rent_num; i++){  // not i <= rent_num, cuz rent_num will be the actual value +1 (++ at the last time)
                    if(user_id == rent_list[i][2]){ // success to rent before
                        // charge fee
                        float switching_rate = 1.0;
                        if(rent_list[i][5] == 1)   // user has changed bike before
                            switching_rate = read_data.switching;

                        int shortest_time = my_graph.dijkstra(rent_list[i][0]-1, station_id-1); // rent_list[i][0] is starting station
                        int user_time = return_time - rent_list[i][3];  // rent_list[i][3] is rent time
                        if(user_time == shortest_time){
                            revenue += (round(read_data.discount[rent_list[i][1]]*switching_rate) * user_time); // rent_list[i][1] is rental bike type
                        }
                        else{
                            revenue += (round(read_data.regular[rent_list[i][1]]*switching_rate) * user_time);
                        }

                        // return bike
                        if(rent_list[i][1] == 0){
                            heap_ptr = my_bike_heap[station_id-1]->electric;
                        }
                        else if(rent_list[i][1] == 1){
                            heap_ptr = my_bike_heap[station_id-1]->lady;
                        }
                        else if(rent_list[i][1] == 2){
                            heap_ptr = my_bike_heap[station_id-1]->road;
                        }
                        
                        my_MinHeap* my_heap = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
                        
                        my_heap->insertKey(rent_list[i][4]);    // rent_list[i][4] is rent_bike_id

                        delete my_heap;
                        rent_list[i][2] = 0;    // i.e. delete the rental record, avoiding the problem that same user rent bike later
                        break;
                    }
                }
            }
        }
    }
    ifs.close();

    // print status
    ofstream ofs1;
    ofs1.open("part2_status.txt", ios::out);
    if (!ofs1.is_open()) {
        cout << "Failed to open file.\n";
    }
    else {
        cout << "-------------- status --------------" << endl;
        for(int i=0; i<station_num; i++){
            cout << i+1 << ":" << endl;
            ofs1 << i+1 << ":" << endl;
            // electric
            cout << "electric: ";
            ofs1 << "electric: ";
            heap_ptr = my_bike_heap[i]->electric;   // i is station_id-1
            my_MinHeap* my_heap_elec = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
            my_heap_elec->printHeapSort(ofs1);
            delete my_heap_elec;
            // lady
            cout << "lady: ";
            ofs1 << "lady: ";
            heap_ptr = my_bike_heap[i]->lady;   // i is station_id-1
            my_MinHeap* my_heap_lady = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
            my_heap_lady->printHeapSort(ofs1);
            delete my_heap_lady;
            // road
            cout << "road: ";
            ofs1 << "road: ";
            heap_ptr = my_bike_heap[i]->road;   // i is station_id-1
            my_MinHeap* my_heap_road = new my_MinHeap(heap_ptr, heap_ptr[100]); // create a pointer of heap object (has the class' attribute & method)
            my_heap_road->printHeapSort(ofs1);
            delete my_heap_road;
        }
        cout << "revenue: " << revenue << endl;
        ofs1 << revenue;
        ofs1.close();

        // print conclusion
        int reject_improve = reject_count_1 - reject_count;
        int revenue_improve = revenue - revenue_1;
        cout << "-------------- conclusion --------------" << endl;
        cout << "<rejection count>" << endl;
        cout << "part 1: " << reject_count_1 << endl;
        cout << "part 2: " << reject_count << endl;
        cout << "improvement: " << reject_improve << endl;
        cout << "---" << endl;
        cout << "<revenue>" << endl;
        cout << "part 1: " << revenue_1 << endl;
        cout << "part 2: " << revenue << endl;
        cout << "improvement: " << revenue_improve << endl;
    }
}   // end of part 2 scope
}   // end of two parts scope
    return 0;
}