#include <iostream>
#include <cstdio>
#include <stdlib.h>
// #include <time.h>
#include "pbbslib/utilities.h"
#include "pbbslib/get_time.h"

#include "./mergesort.h"
#include "./utils.h"

using namespace std;

void testBinarySearch();
void testMerge();
void compareMergeSort(int size, bool isPrint);

int main(int argc, char** argv) {
	// int n = atoi(argv[1]);

	cout << "worker num: " <<  num_workers() << endl;

	// testBinarySearch();
	// testMerge();
	compareMergeSort(100, true);
	return 0;
}

void compareMergeSort(int size, bool isPrint){
	if(size > 100) isPrint = false;

	int *A = new int[size];
	initRandomArray(A, size);
	if(isPrint) cout<< "original array: " << endl;
	if(isPrint)	printArray(A, size);
	
	timer t; 
	t.start();
	sort(A, A + size);
	t.stop(); 

	if(isPrint) cout << "after quick sort (standard library):" << endl;
	if(isPrint) printArray(A, size);
	cout << "quick sort time: " << t.get_total() << endl;

	initRandomArray(A, size);
	t.reset();
	t.start();
	mergesort_par(A, 0, size - 1);
	t.stop(); 
	if(isPrint) cout << "after parallel merge sort:" << endl;
	if(isPrint) printArray(A, size);
	cout << "parallel mergesort time: " << t.get_total() << endl;

	initRandomArray(A, size);
	t.reset();
	t.start();
	int* aux = new int[size];
	mergesort_seq(A, 0, size - 1, aux);
	t.stop(); 
	if(isPrint) cout << "after sequential merge sort:" << endl;
	if(isPrint) printArray(A, size);
	cout << "sequential mergesort time: " << t.get_total() << endl;
}

void testBinarySearch(){
	int size = 100;
	int *A = new int[size];

	parallel_for(0, size, [&](int i){
		A[i] = i*2;
	});

	cout << "The array:" <<endl;
	for (int i = 0; i < size; i++) cout << A[i] << " ";
	cout << endl << endl;

	int search = binary_search(A, size, -1);
	cout << "search -1, should be 0, found: " << search << endl;

	search = binary_search(A, size, 9);
	cout << "search 9, should be 5, found: " << search << endl;

	search = binary_search(A, size, 10);
	cout << "search 10, should be 5, found: " << search << endl;

	search = binary_search(A, size, 11);
	cout << "search 11, should be 6, found: " << search << endl;

	search = binary_search(A, size, 198);
	cout << "search 198, should be  " << size -1 << ", found: " << search << endl;

	search = binary_search(A, size, 199);
	cout << "search 199, should be  " << size << ", found: " << search << endl;

}

void testMerge(){
	int size = 50;
	int *A = new int[size];
	int *B = new int[size];

	for(int i = 0; i < size; i++){
		A[i] = 2*i;
		B[i] = 2 * i + 1;
	}

	for(int i = 0; i < size; i++){
		cout << A[i] << ", ";
	}
	cout << endl << endl;
	for(int i = 0; i < size; i++){
		cout << B[i] << ", ";
	}
	cout << endl << endl;

	int* result = new int [size * 2];
	merge_par(A, size, B, size, result);

	cout << "merged: " << endl;
	for(int i = 0; i < size * 2; i++){
		cout << result[i] << ", " ;
	}
	cout <<endl;
}