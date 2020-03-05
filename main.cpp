#include <iostream>
#include <cstdio>
#include <stdlib.h>
// #include <time.h>
#include "pbbslib/utilities.h"
#include "pbbslib/get_time.h"

#include "./mergesort.h"
using namespace std;

void testBinarySearch();
void testMerge();
void testMergeSort();

// pseudo random function
inline uint32_t hash32(uint32_t a) {
	a = (a+0x7ed55d16) + (a<<12);
	a = (a^0xc761c23c) ^ (a>>19);
	a = (a+0x165667b1) + (a<<5);
	a = (a+0xd3a2646c) ^ (a<<9);
	a = (a+0xfd7046c5) + (a<<3);
	a = (a^0xb55a4f09) ^ (a>>16);
	if (a<0) a = -a;
	return a;
}

int main(int argc, char** argv) {
	int n = atoi(argv[1]);

	cout << "worker num: " <<  num_workers() << endl;

	// testBinarySearch();
	// testMerge();
	testMergeSort();
	
	return 0;
}

void testMergeSort(){
	int size = 50;
	int *A = new int[size];

	// 1. Init the random array
	parallel_for (0, size, [&](int i){
        A[i] = (hash32(i)) % (size*2);
    }); 

	for(int i = 0; i < size; i++){
		cout << A[i] << ", ";
	}
	cout << endl;

	// 2. sort it
	timer t; 
	t.start();
	mergesort(A, 0, size - 1);

	t.stop(); 
	cout << "time: " << t.get_total() << endl;

	// 3. print the sorted result
	for (int i = 0; i < size; i++) {
		cout << A[i] << " ";
	}
	cout << endl;


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
	merge(A, size, B, size, result);

	cout << "merged: " << endl;
	for(int i = 0; i < size * 2; i++){
		cout << result[i] << ", " ;
	}
	cout <<endl;
}