#pragma once

/*
  1. basic
     - the number of paths in an N x M matrix

         C(N + M - 2, N - 1)
       = C(N + M - 2, M - 1)

  2. with stopover

     1) stopover (i, j),     1 <= i <= N, 1 <= j <= M

        F(i,j) = C(i + j - 2, i - 1) * C(N - i + M - j, N - i)
        

     2) about all cells
     
         N   M
        SUM SUM F(i,j) = N * C(N + M - 1, N)
        i=1 j=1
        
        https://www.hackerearth.com/problem/algorithm/blocked-matrix-ce17659d/
*/
