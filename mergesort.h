#include <iostream>
#include "pbbslib/binary_search.h"

using namespace std;
const int threshold = 3;

void mergesort(int *A, int size, bool isParallel);
int binary_search(int *arr, int size, int num);
int kth(int *arr1, int *arr2, int *end1, int *end2, int k);

// sequential
void mergesort_seq(int *A, int start, int end, int* aux);
void merge_seq(int *A, int n1, int *B, int n2, int* C);

// (O(n^3) depth parallel
void mergesort_par(int *A, int start, int end);
void merge_par(int *A, int n1, int *B, int n2, int* C);

// O(n^2) depth parallel
void merge_par_opt(int *A, int n1, int *B, int n2, int* C);
void mergesort_par_opt(int *A, int start, int end);

// ====================== Implementation =================== //

// int kth(int *arr1, int *arr2, int *end1, int *end2, int k) { 
//     if (arr1 == end1) 
//         return arr2[k]; 
//     if (arr2 == end2) 
//         return arr1[k]; 
//     int mid1 = (end1 - arr1) / 2; 
//     int mid2 = (end2 - arr2) / 2; 
//     if (mid1 + mid2 < k) { 
//         if (arr1[mid1] > arr2[mid2]) 
//             return kth(arr1, arr2 + mid2 + 1, end1, end2, 
//                 k - mid2 - 1); 
//         else
//             return kth(arr1 + mid1 + 1, arr2, end1, end2, 
//                 k - mid1 - 1); 
//     } 
//     else { 
//         if (arr1[mid1] > arr2[mid2]) 
//             return kth(arr1, arr2, arr1 + mid1, end2, k); 
//         else
//             return kth(arr1, arr2, end1, arr2 + mid2, k); 
//     } 
// } 

void mergesort(int *A, int size, bool isParallel){
    if(isParallel){
        mergesort_par(A, 0, size);
    }else{
        int* aux = new int[size];
        mergesort_seq(A, 0, size, aux);
    }
}

void mergesort_seq(int *A, int start, int end, int* aux){
    if(start >= end) return;

    int mid = start + (end - start) / 2;

    // cout << "unsorted A[" << start << ", " << end << "]" << endl;
    // for(int i = start; i <= end; i++){
    //     cout << A[i] << ", ";
    // }
    // cout << endl;

    mergesort_seq(A, start, mid, aux);
    mergesort_seq(A, mid + 1, end, aux);

    merge_seq(A+start, mid - start + 1, A + mid + 1, end - mid, aux);
    for(int i = start; i <= end; i++){
        // cout << result[i - start] << ", ";
        A[i] = aux[i - start];
    }

    // cout << "sorted A[" << start << ", " << end << "]" << endl;
    // for(int i = start; i <= end; i++){
    //     cout << A[i] << ", ";
    // }
}

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

void merge_seq(int *A, int n1, int *B, int n2, int* C){
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

// O(log n^2) depth merge
void merge_par(int *A, int n1, int *B, int n2, int* C){

    int n = n1 + n2;
    if(n <= threshold){
        merge_seq(A, n1, B, n2, C);
    }else if(n1 == 0){
        parallel_for(0, n2, [&](int i){ C[i] = B[i]; });
    }else if(n2 == 0){
        parallel_for(0, n1, [&](int i){ C[i] = A[i]; });
    }else{
        int m = n1 / 2;
        int m2 = binary_search(B, n2, A[m]);

        // if(m2 == 0) m ++; // myth
        C[m+m2] = A[m];

        auto left = [&] () { merge_par(A, m, B, m2, C);};
        auto right = [&] () { merge_par(A + m + 1, n1 - m - 1, B + m2, n2 - m2, C + m + m2 + 1);};

        par_do(left, right);
    }
 
}

// pseudocode:
// kthHelp(A, aoff, B, boff, k) =
    // if (|A| + |B| == 0) then return (aoff,boff);
    // else if (|A| == 0) then return (aoff, boff + k);
    // else if (|B| == 0) then return (aoff + k, boff);
    // else
    // amid = |A|/2; bmid = |B|/2;
    // case (A[amid] < B[bmid], k > amid + bmid) of
    // (T,T) ⇒ return kthHelp(A[amid+1:], aoff+amid+1, B, boff, k-amid-1);
    // (T,F) ⇒ return kthHelp(A, aoff, B[:bmid], boff, k);
    // (F,T) ⇒ return kthHelp(A, aoff, B[bmid+1:], boff+bmid+1, k-bmid-1);
    // (F,F) ⇒ return kthHelp(A[:amid], aoff, B, boff, k);
// kth(A, B, k) =return kthHelp(A, 0, B, 0, k);
// reference: https://www.cs.ucr.edu/~yihans/teaching/parallel.pdf

tuple<int, int> kthHelp(int *A, int aoff, int n1, int *B, int boff, int n2, int k){
    if(n1 + n2 == 0) return make_tuple(aoff, boff);
    else if(n1 == 0) return make_tuple(aoff, boff + k);
    else if(n2 == 0) return make_tuple(aoff + k, boff);
    // else if(n1 == 1 && n2 == 1 && k == 0) return make_tuple(aoff, boff);
    else{
        int amid = n1 / 2;
        int bmid = n2 / 2;

        // cout << n1 << "," << n2 << "," << k << "," << amid << ", " << bmid << endl;
        // cout << A[0] << ", " << A[1] << endl; 
        if(A[amid] < B[bmid]){
            if(k > amid + bmid){
                return kthHelp(A+amid+1, aoff+amid+1, n1 - amid - 1,  B, boff, n2, k-amid-1);
            }else{
                 // ?
                 if(n2 == 1 && bmid == 0) 
                    bmid ++;
                return kthHelp(A, aoff, n1, B, boff, n2 - bmid, k);
            }
        }else{
            if(k > amid + bmid){
                return kthHelp(A, aoff, n1, B+bmid+1, boff+bmid+1, n2-bmid-1,  k-bmid-1);
            }else{
                // ?
                if(n1 == 1 && amid == 0) 
                    amid ++;
                return kthHelp(A, aoff, n1 - amid, B, boff, n2, k);
            }
        }
    }
}

tuple<int, int> kth(int* A, int n1, int* B, int n2, int k){
    return kthHelp(A, 0, n1, B, 0, n2, k);
}

// O(log n) depth merge
void merge_par_opt(int *A, int n1, int *B, int n2, int *C){

    int n = n1 + n2;
    if(n <= threshold){
        merge_seq(A, n1, B, n2, C);
    }else if(n1 == 0){
        parallel_for(0, n2, [&](int i){ C[i] = B[i]; });
    }else if(n2 == 0){
        parallel_for(0, n1, [&](int i){ C[i] = A[i]; });
    }else{
        int k = sqrt(n);  // pretty important here
        int m = (n) / k;
        // int* pivots = new int[m];

        // 1. find global k-th, 2k-th, ... m*kth elements in two sorted array.
        // parallel_for(0, m, [&](int i){
        //     pivots[i] = kth(A, B, A+n1, B+n2, i * k);
        // });

            //m = (n − 1)/k + 1;
            // parfor i in [0 : k]
            // s = min(i × m, n);
            // e = min((i + 1) × m, n);
            // (sa, sb) = kth(A, B, s);
            // (ea, eb) = kth(A, B, e);
            // mergeFway(A[sa : ea], B[sb : eb], R[s : e]);
            // return;


        // 2. use binary search to do the spliting
        parallel_for(0, m + 1, [&](int i){ 
            int s = min(i*m, n);
            int e = min((i+1) * m, n);
            
            auto start = kth(A, n1, B, n2, s);
            auto end = kth(A, n1, B, n2, e);
            int sa = get<0>(start);
            int sb = get<1>(start);
            int ea = get<0>(end);
            int eb = get<1>(end);
            
            merge_par_opt(A+sa, ea - sa, B+sb,  eb -sb, C+s);
        });
    }
    // 

}
void mergesort_par_opt(int *A, int start, int end){
    if(start >= end) return;

    int mid = start + (end - start) / 2; // made a serious bug here

    // @TODO: add par_do
    auto left = [&] () { mergesort_par(A, start, mid); };
    auto right = [&] () { mergesort_par(A, mid + 1, end); };
    par_do(left, right);
 
    int* result = new int[end - start + 1];
    merge_par_opt(A + start, mid - start + 1, A + mid + 1, end - mid, result);

    parallel_for(start, end+1, [&](int i){ A[i] = result[i - start];});

}

// @TODO: add a auxilary array to reduce space cost.
void mergesort_par(int *A, int start, int end){
    if(start >= end) return;

    int mid = start + (end - start) / 2; // made a serious bug here

    // @TODO: add par_do
    auto left = [&] () { mergesort_par(A, start, mid); };
    auto right = [&] () { mergesort_par(A, mid + 1, end); };
    par_do(left, right);
 
    int* result = new int[end - start + 1];
    merge_par(A + start, mid - start + 1, A + mid + 1, end - mid, result);

    parallel_for(start, end+1, [&](int i){ A[i] = result[i - start];});
}

