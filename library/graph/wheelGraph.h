#pragma once

/*
  <Wheel Graph>

    https://en.wikipedia.org/wiki/Wheel_graph

  1. Properties
  
    1) The number of Hamiltonian cycles

      - when it is defined as the 1-skeleton of an (n-1)-gonal pyramid, (n >= 4)

          n^2 - 3*n + 3   ----- (1)

      - when is is defined as a graph with n + 1 vertices, (n >= 3)

          n * (n - 1) + 1       ----- (2)

      => from (1)

         (n + 1)^2 - 3*(n + 1) + 3   --> (1)
       = n^2 + 2n + 1 - 3n - 3 + 3
       = n^2 - n + 1
       = n * (n - 1) + 1             --> (2)

  2. Sequences
    https://oeis.org/A002061

    f(n) = n * (n - 1) + 1

      n   f(n)
    ------------
      0     1
      1     1
      2     3
      3     7
      4    13
      5    21
      6    31
      7    43
      8    57
      9    73
       ...
*/
