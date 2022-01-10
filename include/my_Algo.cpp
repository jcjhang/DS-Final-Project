#include "my_Algo.h"

void my_Algo::change_bike(int station, int type){
    change_bike_type = 10000;
    if(type == 0){  // out of electric bike
        if(my_bike_heap[station-1]->lady[100] > my_bike_heap[station-1]->road[100])
            change_bike_type = 1;
        else if(my_bike_heap[station-1]->road[100] != 0)
            change_bike_type = 2;
    }
    if(type == 1){  // out of lady bike
        if(my_bike_heap[station-1]->electric[100] > my_bike_heap[station-1]->road[100])
            change_bike_type = 0;
        else if(my_bike_heap[station-1]->road[100] != 0)
            change_bike_type = 2;
    }
    if(type == 2){  // out of road bike
        if(my_bike_heap[station-1]->electric[100] > my_bike_heap[station-1]->lady[100])
            change_bike_type = 0;
        else if(my_bike_heap[station-1]->lady[100] != 0)
            change_bike_type = 1;
    }

    // pop the changed bike
    if(change_bike_type == 0){
        heap_ptr = my_bike_heap[station-1]->electric;
    }
    else if(change_bike_type == 1){
        heap_ptr = my_bike_heap[station-1]->lady;
    }
    else if(change_bike_type == 2){
        heap_ptr = my_bike_heap[station-1]->road;
    }
    else if(change_bike_type == 10000){
        return;
    }
    my_MinHeap* my_heap = new my_MinHeap(heap_ptr, heap_ptr[100]);
    change_bike_id = my_heap->extractMin();
    
    delete my_heap;
}

void my_Algo::transfer_leave(int src, int des, int type, int num, int time){
    // pop the transfer bike
    if(type == 0){
        heap_ptr = my_bike_heap[src-1]->electric;
    }
    else if(type == 1){
        heap_ptr = my_bike_heap[src-1]->lady;
    }
    else if(type == 2){
        heap_ptr = my_bike_heap[src-1]->road;
    }
    my_MinHeap* my_heap = new my_MinHeap(heap_ptr, heap_ptr[100]);
    for(int i=0; i<num; i++){
        transfer_bike_list[type][i] = my_heap->extractMin();
    }
    delete my_heap;
}

void my_Algo::transfer_arrive(int src, int des, int type, int num, int time){
    // push the transfer bike
    if(type == 0){
        heap_ptr = my_bike_heap[des-1]->electric;
    }
    else if(type == 1){
        heap_ptr = my_bike_heap[des-1]->lady;
    }
    else if(type == 2){
        heap_ptr = my_bike_heap[des-1]->road;
    }
    my_MinHeap* my_heap = new my_MinHeap(heap_ptr, heap_ptr[100]);
    for(int i=0; i<num; i++){
        my_heap->insertKey(transfer_bike_list[type][i]);
    }
    delete my_heap;
}