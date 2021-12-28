#include "./include/read_data.h"

int main()
{
    // cout << "in main" << endl;
    read_data read_data;
    
    int station_num = read_data.get_station_num();
    // cout << "station num: " << station_num << endl;

    my_graph my_graph;
    my_graph = *(read_data.read_map());  // recieve a object's pointer of graph
    // my_graph.print_graph();
    // cout << "min distance in main: " << my_graph.dijkstra(0, 5) << endl;

    read_data.read_fee();
    
    my_station** my_bike_heap = read_data.read_station();  // my_bike_heap["station_id-1"] -> "bike_type"["bike_no.#_in_heap"]
    
    // // each station's object address
    // for(int i=0; i<station_num; i++){
    //     cout << my_bike_heap[i] << endl;
    // }

    // station 1's electric bike's id (100~109)
    // for(int i=0; i<6; i++){
    //     cout << my_bike_heap[0]->electric[i] << endl;
    // }

    int user_num = read_data.read_user();
    // cout << "user num: " << user_num << endl;

    int rent_list[user_num][5]; // station_id, bike_type, user_id, rent_time, rent_bike_id
    int revenue = 0;

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
        int* heap_ptr = new int;    // create a pointer to the heap(array) address (later depends on station and bike_tyoe)
        int rent_bike_id;
        while(!ifs.eof()){
            ifs >> action;
            if(action == "rent"){
                ifs >> station_id >> bike_type >> user_id_string >> rent_time;
                user_id = stoi(user_id_string);
                cout << action << " " << station_id << " " << bike_type << " " << user_id_string << " " << rent_time << endl;
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
                    delete my_heap;
                    continue;   // rent_num will not ++, so rent_list will not include this trade
                }
                else{
                    cout << "accept" << endl;
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
                for(int i=0; i<rent_num; i++){  // not i <= rent_num, cuz rent_num will be the actual value +1 (++ at the last time)
                    if(user_id == rent_list[i][2]){ // success to rent before
                        // charge fee
                        int shortest_time = my_graph.dijkstra(rent_list[i][0]-1, station_id-1); // rent_list[i][0] is starting station
                        int user_time = return_time - rent_list[i][3];  // rent_list[i][3] is rent time
                        if(user_time == shortest_time){
                            revenue += read_data.discount[rent_list[i][1]]; // rent_list[i][1] is rental bike type
                        }
                        else{
                            revenue += read_data.regular[rent_list[i][1]];
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
                        break;
                    }
                }
            }
        }
    }
    ifs.close();
    cout << "revenue: " << revenue << endl;
    return 0;
}