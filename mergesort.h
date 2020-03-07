#include <iostream>
using namespace std;

#include "globalValue.h"
#include "binary_search.h"
#include "merge-sort-seq.h"
#include "merge-sort-parallel.h"
#include "merge-sort-parallel-k-way.h"


void mergesort(int *A, int size, bool isParallel){
    if(isParallel){
        mergesort_par(A, 0, size);
    }else{
        int* aux = new int[size];
        mergesort_seq(A, 0, size, aux);
    }
}








