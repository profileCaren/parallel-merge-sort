#include <iostream>
using namespace std;

#include "globalValue.h"
#include "binary_search.h"
#include "merge-sort-seq.h"
#include "merge-sort-parallel.h"
#include "merge-sort-parallel-k-way.h"

// type == 1: sequential
// type == 2: 2-way
// type == 3: k-way
void mergesort(int *A, int size, int type){
    if(type == 1){
        int* aux = new int[size];
        mergesort_seq(A, 0, size, aux);
    }else if(type == 2){
        mergesort_par(A, 0, size);
    }else {
        mergesort_par_opt(A, 0, size);
    }
}
