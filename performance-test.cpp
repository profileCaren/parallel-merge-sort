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

int main(int argc, char** argv) {
	// int n = atoi(argv[1]);

	cout << "worker num: " <<  num_workers() << endl;

    int size = 10000000;
    cout << "Quicksort: " << runQuickSort(size) << endl;;
    cout << "Sequential: " << runMergeSort(size, SEQUENTIAL) << endl;;
    cout << "2-way: " << runMergeSort(size, TWO_WAY) << endl;
    cout << "K-way: " << runMergeSort(size, K_WAY) << endl;

	return 0;
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