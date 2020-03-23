# Parallel Merge Sort

This project is:

- the implementation and evaluation of parallel merge sort. 
- the final project of [CS260 Parallel Algorithms](https://www.cs.ucr.edu/~yihans/teaching/palgo.html) (UCR 2020 Winter).

This project:

1. implemented 2-way merge sort. (O(log^3 n) depth)
2. implemented k-way merge sort. (O(log^2 n) depth)
3. evaluated both algorithms. (by comparing them with quicksort and sequential merge sort).


## How to Run the Code

### Make

Compile with:

```shell
make
```

Run with:

```shell
./out/main.out
```

If everything goes well, the result would looks like below:

```
worker num:     8
input size:     10000000
quick sort time:        1.04655
2-way merge sort time:  0.192095
k-way merge sort time:  0.201927
sequential mergesort time:      1.59164
```

### Use VS Code 

1. Download VS Code;
2. F5.


