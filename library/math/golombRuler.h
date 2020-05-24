#pragma once

/*
  <Golomb ruler>
    https://en.wikipedia.org/wiki/Golomb_ruler
    - a set of marks at integer positions along an imaginary ruler such that
      no two pairs of marks are the same distance apart

    ex)
      0   1           4       6
      |---|---x---x---|---x---|

      <-1->
      <-------4------->
      <-----------6----------->
          <-----3----->
          <--------5---------->
                      <---2--->

  1. Erdos-Turan construction
     - algorithm
       f(i) = 2 * i * p + i * i % p ,  (0 <= i <= N < p, p is an odd prime)

       f(i) - f(i - 1) <= 3*p
*/
struct GolombRuler {
    // result[i] = 2 * i * p + i * i % p ,  (0 <= i <= N < p, p is an odd prime)
    // O(n)
    static vector<int> build(int N) {
        int p = N + 1;
        while (!isPrimeNumber(p))
            p++;

        vector<int> res;
        res.reserve(N + 1);

        res.push_back(0);
        for (int i = 1; i <= N; i++)
            res.push_back(2 * i * p + i * i % p);

        return res;
    }

private:
    static bool isPrimeNumber(int x) {
        if (x == 1)
            return 0;
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0)
                return false;
        }
        return true;
    }
};