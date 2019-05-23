#pragma once

/*
  https://en.wikipedia.org/wiki/Cayley%27s_formula

  1. Cayley's formula
    the number of labelled trees = n^(n-2), (n is the number of vertices, n >= 2).

    <=> the number of spanning trees of a complete graph with labelled vertices.

  2. Cayley's formula to labelled forests (generalization)
    T(n,k) = k*n^(n - k - 1)

    - T(n,k) is the number of labelled forests on n vertices with k connected components.

  3. If D = { d[1], d[2], ..., d[n] } is the degree sequence of a tree,
     then the number of labelled trees with this degree sequence is

     (n-2)! / {(d[1] - 1)! * (d[2] - 1)! * ... * (d[n] - 1)!}


  4. The number of labelled rooted forests
     = the number of labelled tree with one extra vertex
     = (n + 1)^(n - 1)
    
     - Each labelled rooted forest can be turned into a labelled tree with one extra vertex, by adding a vertex with label n + 1
       and connecting it to all roots of the trees in the forest.

*/
