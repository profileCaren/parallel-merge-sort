#include <iostream>
#include <cstdio>
#include <stdlib.h>

using namespace std;

uint32_t hash32(uint32_t a) {
	a = (a+0x7ed55d16) + (a<<12);
	a = (a^0xc761c23c) ^ (a>>19);
	a = (a+0x165667b1) + (a<<5);
	a = (a+0xd3a2646c) ^ (a<<9);
	a = (a+0xfd7046c5) + (a<<3);
	a = (a^0xb55a4f09) ^ (a>>16);
	if (a<0) a = -a;
	return a;
}

void initRandomArray(int *A, int size){
  	for (int i=0; i < size; i++){
        A[i] = (hash32(i)) % (size*2);
    }
}

void initSortedArray(int *A, int size, int stride){
    for(int i=0; i < size; i++){
        A[i] = i * stride;
    }
}

void printArray(int *A, int size){
	for(int i = 0; i < size; i++){
		cout << A[i] << ", ";
	}
	cout << endl;
}