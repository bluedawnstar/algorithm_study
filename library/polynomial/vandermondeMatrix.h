#pragma once

#include "polyFFTMod.h"
#include "polyFFTMod2.h"

/*
https://en.wikipedia.org/wiki/Vandermonde_matrix
https://en.wikipedia.org/wiki/Generating_function

  <Multiplication of a Vandermonde matrix and a vector>

  |   1      1      1    ...    1       | |  c_0  |   |y_0|
  | a_0^1  a_1^1  a_2^1  ...  a_(n-1)^1 | |  c_1  |   |y_1|
  | a_0^2  a_1^2  a_2^2  ...  a_(n-1)^2 | |  ...  | = |y_2| 
  |   ...    ...    ...  ...    ...     | |c_(n-1)|   |...|
  | a_0^m  a_1^m  a_2^m  ...  a_(n-1)^m |             |y_m|

     c0 * (1 + a0 * x + a0^2 * x^2 + a0^3 * x^3 + ...)  +  c1 * (1 + a1 * x + a1^2 * x^2 + a1^3 * x^3 + ...) + ... +  c(n-1) * (1 + a(n-1) * x + a(n-1)^2 * x^2 + a(n-1)^3 * x^3 + ...)

   = c0 / (1 - a0 * x)  +  c1 / (1 - a1 * x)  + ... +  c(n-1) / (1 - a(n-1) * x)

     c0 * (1 - a1 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)  +  c1 * (1 - a0 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)  +  c(n-1) * (1 - a0 * x) * ... * (1 - a(n - 2) * x)
   = ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
         (1 - a0 * x) * (1 - a1 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)
*/

template <int mod>
struct VandermondeMatrixMultiplierMod {
    // C = { c0, c1, c2, ..., c(n-1) }, A = { a0, a1, a2, ..., a(n-1) }
    static vector<int> multiply(const vector<int>& C, const vector<int>& A, int m) {
        //assert(C.size() == A.size());

        int n = int(C.size());

        vector<pair<vector<int>, vector<int>>> f(n);
        for (int i = 0; i < n; i++)
            f[i] = { { C[i] }, { 1, mod - A[i] } };

        auto r = multiply(f);
        r.second.resize(m + 1);

        auto res = PolyFFTMod<mod>::multiply(r.first, PolyFFTMod<mod>::inverse(r.second));
        res.resize(m + 1);

        return res;
    }

    // A = { (c0, 1 - a0 * x), (c1, 1 - a1 * x), ..., (c(n-1), 1 - a(n-1) * x) }
    // result.first = c0 * (1 - a1 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)  +  c1 * (1 - a0 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)  +  c(n-1) * (1 - a0 * x) * ... * (1 - a(n - 2) * x)
    // result.second = (1 - a0 * x) * (1 - a1 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)
    static pair<vector<int>, vector<int>> multiply(const vector<pair<vector<int>, vector<int>>>& A) {
        return recMult(A, 0, int(A.size()) - 1);
    }

private:
    static pair<vector<int>, vector<int>> recMult(const vector<pair<vector<int>, vector<int>>>& A, int left, int right) {
        if (left == right)
            return A[left];

        int mid = (left + right) >> 1;
        auto x = recMult(A, left, mid);
        auto y = recMult(A, mid + 1, right);

        auto q0 = PolyFFTMod<mod>::multiply(x.first, y.second);
        auto q1 = PolyFFTMod<mod>::multiply(x.second, y.first);
        if (q0.size() < q1.size())
            swap(q0, q1);
        for (int i = 0; i < int(q1.size()); i++)
            q0[i] = (q0[i] + q1[i]) % mod;

        return make_pair(q0, PolyFFTMod<mod>::multiply(x.second, y.second));
    };
};

template <int mod>
struct VandermondeMatrixMultiplierMod2 : public PolyFFTMod2<mod> {
    // C = { c0, c1, c2, ..., c(n-1) }, A = { a0, a1, a2, ..., a(n-1) }
    vector<int> multiply(const vector<int>& C, const vector<int>& A, int m) {
        //assert(C.size() == A.size());

        int n = int(C.size());

        vector<pair<vector<int>, vector<int>>> f(n);
        for (int i = 0; i < n; i++)
            f[i] = { { C[i] }, { 1, mod - A[i] } };

        auto r = multiply(f);
        r.second.resize(m + 1);

        auto res = PolyFFTMod2<mod>::multiply(r.first, PolyFFTMod2<mod>::inverse(r.second));
        res.resize(m + 1);

        return res;
    }

    // A = { (c0, 1 - a0 * x), (c1, 1 - a1 * x), ..., (c(n-1), 1 - a(n-1) * x) }
    // result.first = c0 * (1 - a1 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)  +  c1 * (1 - a0 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)  +  c(n-1) * (1 - a0 * x) * ... * (1 - a(n - 2) * x)
    // result.second = (1 - a0 * x) * (1 - a1 * x) * (1 - a2 * x) * ... * (1 - a(n-1) * x)
    pair<vector<int>, vector<int>> multiply(const vector<pair<vector<int>, vector<int>>>& A) {
        return recMult(A, 0, int(A.size()) - 1);
    }

private:
    pair<vector<int>, vector<int>> recMult(const vector<pair<vector<int>, vector<int>>>& A, int left, int right) {
        if (left == right)
            return A[left];

        int mid = (left + right) >> 1;
        auto x = recMult(A, left, mid);
        auto y = recMult(A, mid + 1, right);

        auto q0 = PolyFFTMod2<mod>::multiply(x.first, y.second);
        auto q1 = PolyFFTMod2<mod>::multiply(x.second, y.first);
        if (q0.size() < q1.size())
            swap(q0, q1);
        for (int i = 0; i < int(q1.size()); i++)
            q0[i] = (q0[i] + q1[i]) % mod;

        return make_pair(q0, PolyFFTMod2<mod>::multiply(x.second, y.second));
    };
};
