## Tree

### Data Structures
 - Balanced Binary Search Tree
   - [Red-Black Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/redBlackTree.h "Red-Black Tree")
   - [Red-Black Tree - simple version](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/redBlackTreeSimple.h "Red-Black Tree - simple version")
   - [AVL Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/avlTree.h "AVL Tree")
   - [Scapegoat Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/scapegoatTree.h "Scapegoat Tree")
   - [Treap](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treap.h "Treap")
   - [Treap + Range Query](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treapRangeQuery.h "Treap + Range Query")
   - [Splay Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/splayTree.h "Splay Tree")
   - [Splay Tree + Range Query](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/splayTreeRangeQuery.h "Splay Tree + Range Query")

    |     Name             | Find     | Insert  | Erase   | Lower Bound | Upper Bound | kth     | Update Range | Query Range |
    |:--------------------:|:--------:|:-------:|:-------:|:-----------:|:-----------:|:-------:|:------------:|:-----------:|
    | RBTree               | O(logn)  | O(logn) | O(logn) | O(logn)     | O(logn)     | O(logn) | -            | -           |
    | AVLTree              | O(logn)  | O(logn) | O(logn) | O(logn)     | O(logn)     | O(logn) | -            | -           |
    | ScapegoatTree        | O(logn)  | O(logn) | O(logn) | O(logn)     | O(logn)     | O(logn) | -            | -           |
    | Treap                | O(logn)  | O(logn) | O(logn) | O(logn)     | O(logn)     | O(logn) | -            | -           |
    | TreapRangeQuery      | O(logn)  | O(logn) | O(logn) | O(logn)     | O(logn)     | O(logn) | O(logn)      | O(logn)     |
    | SplayTree            | O(logn)  | O(logn) | O(logn) | O(logn)     | O(logn)     | O(logn) | -            | -           |
    | SplayTreeRangeQuery  | O(logn)  | O(logn) | O(logn) | O(logn)     | O(logn)     | O(logn) | O(logn)      | O(logn)     |

 - N-ary Tree
   - [Basic Tree Class](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeBasic.h "Basic Tree Class")

    |     Name             | Build - DFS | Build - BFS | LCA     | K-th Ancestor | Centroid | Center  | Diameter | Hashing  |
    |:--------------------:|:-----------:|:-----------:|:-------:|:-------------:|:--------:|:-------:|:--------:|:--------:|
    | Tree (Basic Tree)    |     O(n)    |   O(n)      | O(logH) | O(logH)       |   O(n)   |   O(n)  |  O(n)    | O(nlogn) |

 - Dynamic N-ary Tree
   - [Link-Cut Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/linkCutTree.h "Link-Cut Tree")
   - [Link-Cut Tree + Path Query](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/linkCutTreePathQuery.h "Link-Cut Tree + Range Query")

    |     Name             | Build    |  LCA     | K-th Ancestor | Change Parent | Update Value | Update Path  | Query Path |
    |:--------------------:|:--------:|:--------:|:-------------:|:-------------:|:------------:|:------------:|:----------:|
    | LinkCutTree          | O(nlogn) | O(logn)  | O(logn)       | O(logn)       |     -        |    -         |   -        |
    | LinkCutTreePathQuery | O(nlogn) | O(logn)  | O(logn)       | O(logn)       |  O(logn)     |   O(logn)    |  O(logn)   |

 - LCA(Lowest Common Ancestor) of N-ary Tree
   - [LCA Sparse Table - simple version (no level check)](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/lcaSparseTableSimple.h "LCA Sparse Table - simple version (no level check)")
   - [LCA Sparse Table](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/lcaSparseTable.h "LCA Sparse Table")
   - [LCA Schieber Vishkin](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/lcaSchieberVishkin.h "LCA Schieber Vishkin")

    |     Name             | Build    |  LCA     | K-th Ancestor |
    |:--------------------:|:--------:|:--------:|:-------------:|
    | LCA Sparse Table     | O(nlogn) | O(logn)  | O(logn)       |
    | LCA Schieber Vishkin | O(n)     | O(1)     |   -           |


 - N-ary Tree Flattening
   - [Euler Tour](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeEulerTour.h "Euler Tour")
   - [DFS Tour](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeDfsTour.h "DFS Tour")
   - [DFS Tour + Path Sum](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeDfsTourPathSum.h "DFS Tour + Path Sum")
     - It can be used for connectivity test between two nodes in a tree.
   - [DFS Tour + Subtree Sum](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeDfsTourSubtreeSum.h "DFS Tour + Subtree Sum")
     - It's slightly different from normal DFS tour tree. It doesn't have leave times to prevent nodes from being added twice.
   - [DFS Tour + Subtree Update](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeDfsTourSubtreeUpdate.h "DFS Tour + Subtree Update")
   - [MO's Algorithm on Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeMO.h "MO's Algorithm on Tree")
     - It needs DFS tour tree.

    |     Name                       | Build    |  LCA  | Ancestor Check | Update Path  | Query Path | Update Subtree | Query Subtree |
    |:------------------------------:|:--------:|:-----:|:--------------:|:------------:|:----------:|:--------------:|:-------------:|
    | Euler Tour Tree                | O(nlogn) | O(1)  |    [O(1)]      | -            | -          | -              | -             |
    | DFS Tour Tree (Base)           | O(n)     |  -    |     O(1)       | -            | -          | -              | -             |
    | DFS Tour Tree + Path Sum       | O(nlogn) |  -    |     O(1)       | O(logn)      | O(logn)    | -              | -             |
    | DFS Tour Tree + Subtree Sum    | O(nlogn) |  -    |     O(1)       | -            | -          | O(logn)        | O(logn)       |
    | DFS Tour Tree + Subtree Update | O(nlogn) |  -    |     O(1)       | -            | -          | O(logn)        | O(logn)       |
    | MO's algorithm                 | O(q)     |  -    |      -         | -            | O(sqrt(n)) | -              | -             |

 - N-ary Tree Decomposition & Path Query
   - [Sparse Table on Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/rangeQuery-SparseTableOnTree.h "Sparse Table on Tree")
   - [Heavy-Light Decomposition](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeHeavyLightDecomposition.h "Heavy-Light Decomposition")
   - [Heavy-Light Decomposition + Path Query(Segment Tree)](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeHeavyLightDecompositionPathQuery.h "Heavy-Light Decomposition + Path Query(Segment Tree)")
   - [Heavy-Light Decomposition + Path Query(BIT)](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeHeavyLightDecompositionPathQueryBIT.h "Heavy-Light Decomposition + Path Query(BIT)")
   - [Heavy-Light Decomposition + Path Query(Segment Tree / Lazy Propagation)](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeHeavyLightDecompositionPathQueryLazy.h "Heavy-Light Decomposition + Path Query(Segment Tree / Lazy Propagation)")
   - [Block Tree for LCA](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeBlockTreeLCA.h "Block Tree for LCA")
     - Depth-based decomposition
   - [Block Tree for Path Query](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeBlockTreePathQuery.h "Block Tree for Path Query")
     - Depth-based decomposition, path query
   - [Block Tree for Path Query & Dynamic Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeBlockTreePathQueryDynamic.h "Block Tree for Path Query & Dynamic Tree")
     - Depth-based decomposition, path query, changing parent (dynamic tree)
   - [Block Tree for Path and Subtree Query](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treeBlockTreePathQuery.h "Block Tree for Path and Subtree Query")
     - Depth-based decomposition, path query, subtree update / query
   - [Path Sqrt Decomposition for Path Query](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/treePathSqrtDecomposition.h "Path Sqrt Decomposition for Path Query")
     - Path-based decomposition, path query
   - [Centroid Decomposition](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/centroidDecomposition.h "Centroid Decomposition")
     - ...

    |     Name                  | Build    |  LCA       | K-th Ancestor | Change Parent | Update Value  | Update Path | Query Path            | Update Subtree | Query Subtree |
    |:-------------------------:|:--------:|:----------:|:-------------:|:-------------:|:-------------:|:-----------:|:---------------------:|:--------------:|:-------------:|
    | SparseTableOnTree         | O(nlogn) | O(logn)    | O(logn)       | -             | -             | -           | O(logn) / O((logn)^2) | -              | -             | 
    | HeavyLightDecomposition   | O(n)     |  -         | -             | -             | -             | -           | -                     | -              | -             |
    | HLDPathQuery              | O(logn)  |  -         | -             | -             | O(logn)       | O(klogn)    | O(logn)               | -              | -             |
    | HLDPathQueryBIT (Sum)     | O(logn)  |  -         | -             | -             | O(logn)       | O(logn)     | O(logn)               | -              | -             |
    | HLDPathQueryLazy          | O(logn)  |  -         | -             | -             | O(logn)       | O(logn)     | O(logn)               | -              | -             |
    | BlockTreeLCA              | O(n)     | O(sqrt(H)) | O(sqrt(H))    | -             | -             | -           | -                     | -              | -             |
    | BlockTreePathQuery        | O(n)     | O(sqrt(H)) | O(sqrt(H))    | -             | >= O(sqrt(N)) | -           | O(sqrt(H))            | -              | -             |
    | BlockTreePathQueryDynamic | O(n)     | O(sqrt(H)) | O(sqrt(H))    | O(N)          | >= O(sqrt(N)) | -           | O(sqrt(H))            | -              | -             |
    | BlockTreeSubtreeUpdate    | O(n)     | O(sqrt(N)) | O(sqrt(N))    | -             | O(sqrt(N))    | -           | O(sqrt(N))            | O(sqrt(N))     | O(sqrt(N))    |
    | Centroid Decomposition    | O(n)     | -          | -             | -             | -             | -           | -                     | -              | -             |

 - Spatial Trees
   - [Interval Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/intervalTree.h "Interval Tree")
   - [KD Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/kdtree.h "KD Tree")
   - [KD Tree without updating](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/kdtreeNoUpdate.h "KD Tree - no updating")
   - [Metric Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/metricTree.h "Metric Tree")
   - [Quad Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/quadTree.h "Quad Tree")
   - [R Tree](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/rTree.h "R Tree")

### Algorithms
 - [Maximum Independent Set](https://github.com/bluedawnstar/algorithm_study/blob/master/library/tree/maxIndependentSet.h "Maximum Independent Set")
