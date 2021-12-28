#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class my_graph{
    public:
        // my_graph(int vertice){
        //     V = vertice;
        //     graph = new int*[V];
        //     for (int i = 0; i < V; i++){
        //         graph[i] = new int[V];
        //         for (int j = 0; j < V; j++)
        //             graph[i][j] = 0;  // initialization, 0 stand for disconnected
        //     }
        // }
        void initial_graph(int v);  // vertice
        void add_edge(int s, int e, int w); // start, end, weight(distance)
        void print_graph();
        int minDistance(int dist[], bool sptSet[]);
        int dijkstra(int src, int des);

    private:
        int V;  // cannot use static, cuz its value need to be decided in one of the (specific) object
        int **graph;  // allocate a 2D array
};

class my_station{
    public:
        int* electric = new int[101];   // will be the pointer point to its min heap, 101th record the heap size
        int* lady = new int[101];
        int* road = new int[101];
};

// A class for Min Heap
class my_MinHeap
{
    int *harr;  // pointer to array of elements in heap
    int capacity = 100; // maximum possible size of min heap
    int heap_size = 0;  // Current number of elements in min heap
public:
    // Constructor
    my_MinHeap(int* bike_type, int my_heap_size){
        harr = bike_type;
        heap_size = my_heap_size;
    }

    int parent(int i) { return (i-1)/2; }
    int left(int i) { return (2*i + 1); }   // to get index of left child of node at index i
    int right(int i) { return (2*i + 2); }  // to get index of right child of node at index i

    int getMin() { return harr[0]; }    // Returns the minimum key (key at root) from min heap
    
    void swap(int *x, int *y){
        int temp = *x;
        *x = *y;
        *y = temp;
    }

    // Inserts a new key 'k'
    void insertKey(int k);

    // to heapify a subtree with the root at given index
    void MinHeapify(int i);
  
    // to extract(remove + return) the root which is the minimum element
    int extractMin();

    bool isEmpty();

    void printHeapSort(ofstream& ofs);
};