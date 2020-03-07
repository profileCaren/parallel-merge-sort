
void merge_seq(int *A, int n1, int *B, int n2, int* C);
void mergesort_seq(int *A, int start, int end, int* aux);

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

void mergesort_seq(int *A, int start, int end, int* aux){
    if(start >= end) return;

    int mid = start + (end - start) / 2;

    mergesort_seq(A, start, mid, aux);
    mergesort_seq(A, mid + 1, end, aux);

    merge_seq(A+start, mid - start + 1, A + mid + 1, end - mid, aux);
    for(int i = start; i <= end; i++){
        // cout << result[i - start] << ", ";
        A[i] = aux[i - start];
    }
}