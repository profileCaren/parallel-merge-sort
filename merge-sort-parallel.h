

void mergesort_par(int *A, int start, int end);
void merge_par(int *A, int n1, int *B, int n2, int* C);

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


/*

the psuedocode provided by Yihan Sun.
I modified a bit to make it work

    if (A’ is empty or B’ is empty) base_case;
    m = n1/2;
    m2 = binary_search(B’, A’[m]);
    C[m+m2+1] = A’[m];
    in parallel:
        merge(A’, m, B’, m2, C);
        merge(A’+m+1, n1-m-1, B’+m2+1, n2-m2-1, C+m+m2);
    return C;
    cout << n1 << "," << n2 << endl;

 */
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