#pragma once

/*
  1. Generalized pentagonal numbers
    // https://en.wikipedia.org/wiki/Pentagonal_number_theorem
    // https://oeis.org/A001318

    inf
    PROD (1 - x^i) = (1 - x)(1 - x^2)(1 - x^3)...
    i=1
                   = 1 - x - x^2 + x^5 + x^7 - x^12 - x^15 + x^22 + x^26 - ...

                         inf
                   = 1 + SUM (-1)^k ( x^k(3k+1)/2 + x^k(3k-1)/2 )
                         k=1

                   = SUM (-1)^abs(j) * x^{j*(3*j-1)/2}
                   j=0,1,-1,2,-2,...
*/
struct PentagonalNumber {
    // result = { (degree, value), ...}
    template <typename T>
    static vector<pair<T,int>> generalizedPentagonalNumbers(T maxDegree) {
        vector<pair<T,int>> res;
        for (T j = 0;;) {
            T k = j * (3 * j - 1) / 2;      // k = j*(3*j - 1) / 2,  j = 0, 1, -1, 2, -2, 3, -3, ...
            if (k > maxDegree)
                break;
            res.emplace_back(k, (j & 1) ? -1 : 1);

            if (j <= 0)
                j = 1 - j;
            else
                j = -j;
        }

        return res;
    }
};
