

tuple<int, int> kth(int* A, int n1, int* B, int n2, int k);
tuple<int, int> kthHelp(int *A, int aoff, int n1, int *B, int boff, int n2, int k);
void merge_par_opt(int *A, int n1, int *B, int n2, int *C);
void mergesort_par_opt(int *A, int start, int end);

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
tuple<int, int> kth(int* A, int n1, int* B, int n2, int k){
    return kthHelp(A, 0, n1, B, 0, n2, k);
}

tuple<int, int> kthHelp(int *A, int aoff, int n1, int *B, int boff, int n2, int k){
    if(n1 + n2 == 0) return make_tuple(aoff, boff);
    else if(n1 == 0) return make_tuple(aoff, boff + k);
    else if(n2 == 0) return make_tuple(aoff + k, boff);
    else{
        int amid = n1 / 2;
        int bmid = n2 / 2;

        if(A[amid] < B[bmid]){
            if(k > amid + bmid){
                return kthHelp(A+amid+1, aoff+amid+1, n1 - amid - 1,  B, boff, n2, k-amid-1);
            }else{
                if(n2 == 1 && bmid == 0) bmid ++; // important corner case
                return kthHelp(A, aoff, n1, B, boff, n2 - bmid, k);
            }
        }else{
            if(k > amid + bmid){
                return kthHelp(A, aoff, n1, B+bmid+1, boff+bmid+1, n2-bmid-1,  k-bmid-1);
            }else{
                if(n1 == 1 && amid == 0) amid ++; // important corner case
                return kthHelp(A, aoff, n1 - amid, B, boff, n2, k);
            }
        }
    }
}

// O(log n) depth merge
void merge_par_opt(int *A, int n1, int *B, int n2, int *C){

    int n = n1 + n2;
    if(n <= THRESHOLD_MERGE){
        merge_seq(A, n1, B, n2, C);
    }else if(n1 == 0){
        parallel_for(0, n2, [&](int i){ C[i] = B[i]; });
    }else if(n2 == 0){
        parallel_for(0, n1, [&](int i){ C[i] = A[i]; });
    }else{
        int k = sqrt(n);  // pretty important here
        int m = (n) / k;

        // pseudocode: 
        //m = (n − 1)/k + 1;
        // parfor i in [0 : k]
        // s = min(i × m, n);
        // e = min((i + 1) × m, n);
        // (sa, sb) = kth(A, B, s);
        // (ea, eb) = kth(A, B, e);
        // mergeFway(A[sa : ea], B[sb : eb], R[s : e]);
        // return;

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
}

void mergesort_par_opt(int *A, int start, int end){
    if(start >= end) return;

    if(end - start < THRESHOLD_SORT){
        // cout << "111" << endl;
        sort(A+start, A+end+1);
        return ;
    }

    int mid = start + (end - start) / 2; // made a serious bug here

    auto left = [&] () { mergesort_par_opt(A, start, mid); };
    auto right = [&] () { mergesort_par_opt(A, mid + 1, end); };
    par_do(left, right);
 
    int* result = new int[end - start + 1];
    merge_par_opt(A + start, mid - start + 1, A + mid + 1, end - mid, result);

    parallel_for(start, end+1, [&](int i){ A[i] = result[i - start];});
}