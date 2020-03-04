#include <iostream>
#include "pbbslib/binary_search.h"

using namespace std;
const int threshold = 2;
int binary_search(int *arr, int size, int num){
    int left = 0;
    int right = size - 1; // small bug here
    while(left <= right){
        int mid = left + (right - left) / 2;
        if(arr[mid] > num){
            right = mid - 1;
        }else if (arr[mid] == num){
            return mid;
        }else{
            left = mid + 1;
        }
    }
    return left;
}

void sequential_merge(int *A, int n1, int *B, int n2, int* C){
    int i = 0;
    int j = 0;
    while(i < n1 && j < n2){
        if(A[i] <= B[j]){
            C[i+j] = A[i];
            i++;
        }else{ // A[i] > B[j]
            C[i+j] = B[j];
            j++;
        }
    }

    while(i < n1){
        C[i+j] = A[i];
        i++;
    }

    while(j < n2){
        C[i+j] = B[j];
        j++;
    }
}

// the psuedocode provided by Yihan Sun
// I modified a bit to make it work
//   if (A’ is empty or B’ is empty) base_case;
//   m = n1/2;
//   m2 = binary_search(B’, A’[m]);
//   C[m+m2+1] = A’[m];
//   in parallel:
//     merge(A’, m, B’, m2, C);
//     merge(A’+m+1, n1-m-1, B’+m2+1, n2-m2-1, C+m+m2);
//   return C;
// cout << n1 << "," << n2 << endl;

void merge(int *A, int n1, int *B, int n2, int* C){

    int n = n1 + n2;
    if(n < 4){
        sequential_merge(A, n1, B, n2, C);
    }else if(n1 == 0){
        parallel_for(0, n2, [&](int i){ C[i] = B[i]; });
    }else if(n2 == 0){
        parallel_for(0, n1, [&](int i){ C[i] = A[i]; });
    }else{
        int m = n1 / 2;
        int m2 = binary_search(B, n2, A[m]);

        // if(m2 == 0) m ++; // myth
        C[m+m2] = A[m];

        auto left = [&] () { merge(A, m, B, m2, C);};
        auto right = [&] () { merge(A + m + 1, n1 - m - 1, B + m2, n2 - m2, C + m + m2 + 1);};

        par_do(left, right);
    }
 
}

int* mergesort(int *A, int start, int end){
    if(start > end) return A;
    int mid = (end - start) / 2;

    int *left = mergesort(A, start, mid);
    int *right = mergesort(A, mid + 1, end); // TODO: maybe bug here

    int* result = new int[end - start];
    merge(left, mid, right, end - start - mid - 1, result);
    return result;
}
