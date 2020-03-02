#include "pbbslib/utilities.h"

using namespace std;

void scan(int* In, int* Out, int* B, int* C, int n) {
	if (n==0) return;
	if (n == 1) {
		Out[0] = In[0];
		return;
	}
	parallel_for(0, n/2, [&](int i){B[i] = In[2*i] + In[2*i+1];});
		
	
	scan(B, C, B+n/2, C+n/2, n/2);	
	Out[0] = In[0];
	
	parallel_for (1, n, [&](int i) {
		if (i%2) Out[i] = C[i/2];
		else Out[i] = C[i/2-1] + In[i];
	});
}
