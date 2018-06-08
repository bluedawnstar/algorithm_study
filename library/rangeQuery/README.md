## Range-Based Update & Query

### Data Structures
 - Fenwick Trees (Binary Indexed Trees)
   - [Fenwick Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/fenwickTree.h "Fenwick Tree")
   - [Fenwick Tree - Mult / Add](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/fenwickTreeMultAdd.h "Fenwick Tree Mult/Add")
   - [Fenwick Tree for XOR](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/fenwickTreeXor.h "Fenwick Tree for XOR")
   - [Fenwick Tree - 2D](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/fenwickTree2D.h "Fenwick Tree - 2D")
   - [Generalized Fenwick Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/generalizedBIT.h "Generalized Fenwick Tree")
   - [Partially Persistent Fenwick Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/fenwickTreePersistent.h "Partially Persistent Fenwick Tree")

   - Comparison
         Name                       | Build    | Add      | Add Range | Update      | Update Range  | Sum         | Sum Range   | General Query | General Query Range | Persistent
	:------------------------------:|:--------:|:--------:|:---------:|:-----------:|:-------------:|:-----------:|:-----------:|:-------------:|:-------------------:|:-----------:
	 FenwickTree                    | O(nlogn) | O(logn)  |    -      | O(logn)     |      -        | O(logn)     | O(logn)     |       -       |         -           |     X
	 FenwickTreeMultAdd             | O(nlogn) | O(logn)  | O(logn)   | O(logn)     |  O(logn)      | O(logn)     | O(logn)     |       -       |         -           |     X
	 FenwickTreeXor                 | O(nlogn) | O(logn)  |    -      | O(logn)     |      -        | O(logn)     | O(logn)     |       -       |         -           |     X
	 GeneralizedBIT                 | O(nlogn) | O(logn)  | O(klogn)  | O(logn)     |  O(klogn)     | O(logn)     | O(logn)     |    O(logn)    |       O(logn)       |     X
	 PartiallyPersistentFenwickTree |          | O(logn)  |    -      | O((logn)^2) |      -        | O((logn)^2) | O((logn)^2) |       -       |         -           |     O

 - Segment Trees - general operation
   - [Segment Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTree.h "Segment Tree")
   - [Segment Tree - Lazy Update](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLazy.h "Segment Tree - Lazy Update")
   - [Compact Segment Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeCompact.h "Compact Segment Tree")
   - [Compact Segment Tree - Lazy Update](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeCompactLazy.h "Compact Segment Tree - Lazy Update")
   - [Compact Segment Tree - Lazy Add](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeCompactLazyAdd.h "Compact Segment Tree - Lazy Add")
   - [Persistent Segment Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreePersistent.h "Persistent Segment Tree")
   - [Persistent Segment Tree - Lazy Update](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreePersistentLazy.h "Persistent Segment Tree - Lazy Update")

   - Comparison
         Name                       | Build    | Add      | Add Range | Update      | Update Range  | Query   | Query Range | Persistent
	:------------------------------:|:--------:|:--------:|:---------:|:-----------:|:-------------:|:-------:|:-----------:|:-----------:
	 SegmentTree                    | O(nlogn) |    -     |    -      | O(logn)     |  O(klogn)     | O(logn) |   O(logn)   |     X
	 SegmentTreeLazy                | O(nlogn) |    -     |    -      | O(logn)     |  O(logn)      | O(logn) |   O(logn)   |     X
	 CompactSegmentTree             | O(nlogn) | O(logn)  | O(klogn)  | O(logn)     |  O(klogn)     | O(logn) |   O(logn)   |     X
	 CompactSegmentTreeLazy         | O(nlogn) | O(logn)  | O(logn)   | O(logn)     |  O(logn)      | O(logn) |   O(logn)   |     X
	 PersistentSegmentTree          | O(nlogn) |    -     |    -      | O(logn)     |  O(klogn)     | O(logn) |   O(logn)   |     O
	 PersistentSegmentTreeLazy      | O(nlogn) |    -     |    -      | O(logn)     |  O(logn)      | O(logn) |   O(logn)   |     O

 - Segment Trees - special operation
   - [Segment Tree for RMQ](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeRMQ.h "Segment Tree for RMQ")
   - [Segment Tree for Maximum Subarray](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeMaxSubarray.h "Segment Tree for Maximum Subarray")
   - [Segment Tree for 1D Line Segment](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLine1D.h "Segment Tree for 1D Line Segment")
     - It can be used for calculating area of rectangles
   - [Li Chao Tree for Lower Envelope of lines](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLine2DMin.h "Li Chao Tree for Lower Envelope of lines")
   - [Li Chao Tree for Lower Envelope of lines (array version)](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLine2DArrayMin.h "Li Chao Tree for Lower Envelope of lines")
   - [Li Chao Tree for Upper Envelope of lines](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLine2DMax.h "Li Chao Tree for Upper Envelope of lines")
   - [Li Chao Tree for Upper Envelope of lines (array version)](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLine2DArrayMax.h "Li Chao Tree for Upper Envelope of lines")
   - [Li Chao Tree for Lower Envelope of line segments](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLineSegment2DMin.h "Li Chao Tree for Lower Envelope of line segments")
   - [Li Chao Tree for Upper Envelope of line segments](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/segmentTreeLineSegment2DMax.h "Li Chao Tree for Upper Envelope of line segments")

   - Comparison
         Name                                   | Build    | Add      | Add Range | Update      | Update Range  | Query   | Query Range | Persistent
	:------------------------------------------:|:--------:|:--------:|:---------:|:-----------:|:-------------:|:-------:|:-----------:|:-----------:
	 SegmentTreeRMQ                             | O(nlogn) |    -     |    -      | O(logn)     |     -         | O(logn) |   O(logn)   |     X
	 SegmentTreeMaxSubarray                     | O(nlogn) |    -     |    -      | O(logn)     |  O(klogn)     | O(1)    |   O(logn)   |     X
	 SegmentTreeLine1D                          | O(nlogn) | O(logn)  |  O(logn)  |    -        |     -         | O(logn) |   O(logn)   |     X
	 SegmentTreeLine2DMin (Li Chao Tree)        | O(nlogX) | O(logX)  |    -      |    -        |     -         | O(logX) |     -       |     X
	 SegmentTreeLine2DMax (Li Chao Tree)        | O(nlogX) | O(logX)  |    -      |    -        |     -         | O(logX) |     -       |     X
	 SegmentTreeLine2DArrayMin (Li Chao Tree)   | O(nlogn) | O(logn)  |    -      |    -        |     -         | O(logn) |     -       |     X
	 SegmentTreeLine2DArrayMax (Li Chao Tree)   | O(nlogn) | O(logn)  |    -      |    -        |     -         | O(logn) |     -       |     X
	 SegmentTreeLineSegment2DMin (Li Chao Tree) | O(nlogn) | O(logn)  |    -      |    -        |     -         | O(logn) |     -       |     X
	 SegmentTreeLineSegment2DMax (Li Chao Tree) | O(nlogn) | O(logn)  |    -      |    -        |     -         | O(logn) |     -       |     X

 - Sparse Table
   - [Sparse Table](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sparseTable.h "Sparse Table")
   - [Indirect Sparse Table](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sparseTableIndex.h "Indirect Sparse Table")
   - [Sparse Table for RMQ](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sparseTableMin.h "Sparse Table for RMQ")
   - [Indirect Sparse Table for RMQ](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sparseTableMinIndex.h "Indirect Sparse Table for RMQ")
   - [Reversed Sparse Table](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sparseTable.h "Reversed Sparse Table")
   - [Disjoint Sparse Table](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/disjointSparseTable.h "Disjoint Sparse Table")
   - [Sparse Table - 2D](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sparseTable2D.h "Sparse Table - 2D")

   - Comparison
         Name            | Build    | Sum     | Sum Range | Query   | Query Range 
	:-------------------:|:--------:|:-------:|:---------:|:-------:|:------------:
	 SparseTable         | O(nlogn) | O(logn) | O(logn)   |  O(1)   |   O(1)   
	 SparseTableIndex    | O(nlogn) | O(logn) | O(logn)   |  O(1)   |   O(1)   
	 SparseTableMin      | O(nlogn) | O(logn) | O(logn)   |  O(1)   |   O(1)   
	 SparseTableMinIndex | O(nlogn) | O(logn) | O(logn)   |  O(1)   |   O(1)   
	 ReverseSparseTable  | O(nlogn) | O(logn) | O(logn)   |  O(1)   |   O(1)   
	 DisjointSparseTable | O(nlogn) |  O(1)   |  O(1)     |  O(1)   |   O(1)   

 - Sqrt Decomposition
   - [MO's Algorithm](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/MOAlgorithm.h "MO's algorithm")
   - [Sqrt-decomposition](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sqrtDecomposition.h "Sqrt-decomposition")
   - [Sqrt-decomposition for sum](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sqrtDecompositionSum.h "Sqrt-decomposition for sum")

   - Comparison
         Name                   | Build | Add        | Add Range  | Update      | Update Range  | Query      | Query Range 
	:--------------------------:|:-----:|:----------:|:----------:|:-----------:|:-------------:|:----------:|:-----------:
	 Sqrt-decomposition         | O(n)  | O(sqrt(n)) | O(sqrt(n)) | O(sqrt(n))  |  O(sqrt(n))   | O(sqrt(n)) | O(sqrt(n))  
	 Sqrt-decomposition for sum | O(n)  | O(sqrt(n)) | O(sqrt(n)) | O(sqrt(n))  |  O(sqrt(n))   | O(sqrt(n)) | O(sqrt(n))  
	 MO's algorithm             |   -   |     -      |     -      |     -       |      -        | O(sqrt(n)) | O(sqrt(n))  

 - Misc
   - [Sqrt Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/sqrtTree.h "Sqrt Tree")
   - [Merge Sort Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/rangeQuery/mergeSortTree.h "Merge Sort Tree")

   - Comparison
         Name        | Build       | Add  | Add Range | Update | Update Range | kth - Range        | Count Range | Query  | Query Range 
	:---------------:|:-----------:|:----:|:---------:|:------:|:------------:|:------------------:|:-----------:|:------:|:-----------:
	 Sqrt Tree       | O(nloglogn) |  -   |     -     |    -   |       -      |       -            |      -      |  O(1)  |    O(1)  
	 Merge Sort Tree | O(nlogn)    |  -   |     -     |    -   |       -      | O((logn)^2 * logX) | O((logn)^2) |    -   |     -  
