#include "my_DS.h"

class my_Algo{
    public:
        my_Algo(my_station** input_heap){
            my_bike_heap = input_heap;
        }
        void change_bike(int station, int type);
        void transfer_leave(int src, int des, int type, int num, int time);
        void transfer_arrive(int src, int des, int type, int num, int time);
        int change_bike_type = 10000;
        int change_bike_id = 0;
    
    private:
        my_station** my_bike_heap;
        int* heap_ptr = new int;
        int transfer_bike_list[3][100];
};