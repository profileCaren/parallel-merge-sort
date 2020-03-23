#include <iostream>
#include <cstdio>
#include <stdlib.h>
// #include <time.h>
#include <algorithm>

#include "pbbslib/utilities.h"
#include "pbbslib/get_time.h"

#include "./utils.h"
#include "./mergesort.h"

using namespace std;

void testBinarySearch();
void testMerge();
void compareMergeSort(int size, bool isPrint);
void testFindKthElement();
void testMergeOpt();

int main(int argc, char** argv) {
	// int n = atoi(argv[1]);
	int size = 10000000;
	cout << "worker num: \t" <<  num_workers() << endl;
	cout << "input size: \t" << size << endl;
	// testBinarySearch();
	// testMerge();
	compareMergeSort(size, true);
	// testFindKthElement();
	// testMergeOpt();
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
	cout << "quick sort time: \t" << t.get_total() << endl;

	initRandomArray(A, size);
	t.reset();
	t.start();
	mergesort_par(A, 0, size - 1);
	t.stop(); 
	if(isPrint) cout << "after parallel merge sort:" << endl;
	if(isPrint) printArray(A, size);
	cout << "2-way merge sort time: \t" << t.get_total() << endl;

	initRandomArray(A, size);
	t.reset();
	t.start();
	mergesort_par_opt(A, 0, size - 1);
	t.stop(); 
	if(isPrint) cout << "after parallel (optimized) merge sort:" << endl;
	if(isPrint) printArray(A, size);
	cout << "k-way merge sort time: \t" << t.get_total() << endl;

	initRandomArray(A, size);
	t.reset();
	t.start();
	int* aux = new int[size];
	mergesort_seq(A, 0, size - 1, aux);
	t.stop(); 
	if(isPrint) cout << "after sequential merge sort:" << endl;
	if(isPrint) printArray(A, size);
	cout << "sequential mergesort time: \t" << t.get_total() << endl;
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

void testMergeOpt(){
	int size = 10;
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
	merge_par_opt(A, size, B, size, result);

	cout << "merged: " << endl;
	for(int i = 0; i < size * 2; i++){
		cout << result[i] << ", " ;
	}
	cout <<endl;
}


void testFindKthElement(){
	int size1 = 15;
	int size2 = 13;
	int *A = new int[size1];
	int *B = new int[size2];
	initSortedArray(A, size1, 2);
	initSortedArray(B, size2, 3);
	
	printArray(A, size1);
	cout << endl;
	printArray(B, size2);
	cout << endl;

	int * result = new int [size1 + size2];
	// merge_par_opt(A, size1, B, size2, result);
	// printArray(result, size1+size2);
	// cout <<endl;

	merge_par(A, size1, B, size2, result);
	printArray(result, size1+size2);

	int k = 0;
	auto res = kth(A, size1, B, size2, k);
	cout<< "A: " << get<0>(res) << endl;
	cout<< "B: " << get<1>(res) << endl;

	 k = 3;
	res = kth(A, size1, B, size2, k);
	cout<< "A: " << get<0>(res) << endl;
	cout<< "B: " << get<1>(res) << endl;

	k = 10;
	res = kth(A, size1, B, size2, k);
	cout<< "A: " << get<0>(res) << endl;
	cout<< "B: " << get<1>(res) << endl;

}