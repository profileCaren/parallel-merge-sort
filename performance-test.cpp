#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <algorithm>

#include "pbbslib/utilities.h"
#include "pbbslib/get_time.h"

#include "./utils.h"
#include "./mergesort.h"

using namespace std;

const int SEQUENTIAL = 1;
const int TWO_WAY = 2;
const int K_WAY = 3;

double runQuickSort(int size);
double runMergeSort(int size, int type);
void runBetween(int sizeStart, int sizeEnd, int stride);

int main(int argc, char** argv) {

    runBetween(5000000, 100000000, 5000000);
    // runAll();

	return 0;
}

void runAll(int size = 100000000){
	cout << "worker num: " <<  num_workers() << endl;
    cout << "Input size: " << size << endl;
    cout << "Quicksort (standard library): " << runQuickSort(size) << endl;;
    cout << "Sequential Merge Sort: " << runMergeSort(size, SEQUENTIAL) << endl;;
    cout << "2-way Merge Sort: " << runMergeSort(size, TWO_WAY) << endl;
    cout << "K-way Merge Sort: " << runMergeSort(size, K_WAY) << endl;
}

void runBetween(int sizeStart, int sizeEnd, int stride){
    for(int i = sizeStart; i <= sizeEnd; i+= stride){
        cout << i << "\t" << runMergeSort(i, TWO_WAY) << endl;
    }
    for(int i = sizeStart; i <= sizeEnd; i+= stride){
        cout << i << "\t" << runMergeSort(i, K_WAY) << endl;
    }
}

double runQuickSort(int size){
    int *A = new int[size];
	initRandomArray(A, size);

	timer t; 
	t.start();
    sort(A, A+size);
	t.stop(); 

    return t.get_total();
}

// run two-way mergesort, and return runnign time
double runMergeSort(int size, int type){
    int *A = new int[size];
	initRandomArray(A, size);

	timer t; 
	t.start();
    mergesort(A, size, type);
	t.stop(); 

    return t.get_total();
}