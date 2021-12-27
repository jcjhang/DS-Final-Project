#include "./include/read_data.h"

int main()
{
    cout << "in main" << endl;
    read_data rd;
    
    int station_num = rd.get_station_num();
    cout << "station num: " << station_num << endl;
    
    my_station** my_bike_heap = rd.read_station();  // my_bike_heap["station_id-1"] -> "bike_type"["bike_no.#_in_heap"]
    
    // // each station's object address
    // for(int i=0; i<station_num; i++){
    //     cout << my_bike_heap[i] << endl;
    // }

    // station 1's electric bike's id (100~109)
    // for(int i=0; i<6; i++){
    //     cout << my_bike_heap[0]->elec[i] << endl;
    // }

    my_graph mg;
    mg = *(rd.read_map());  // recieve a object's pointer of graph
    // mg.print_graph();
    // cout << "min distance in main: " << mg.dijkstra(0, 5) << endl;
    return 0;
}