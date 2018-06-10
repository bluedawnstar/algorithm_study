#pragma once
#pragma warning(disable: 4334)

///////////////////////////////////////////////////////////////////////////////
// Sum over Subsets(SOS)
// http://codeforces.com/blog/usaxena95
// https://discuss.codechef.com/questions/107073/maxor-ediorial

// O(3^n)
// n : the number of elements
// A : values of all subsets
// return : F(x) = SUM A[i],  i = subsets of X
inline vector<int> solveSOSNaive(int n, const vector<int>& A) {
    vector<int> F(1 << n);

    for (int mask = 0; mask < (1 << n); mask++) {
        F[mask] = A[0];
        for (int subset = mask; subset; subset = (subset - 1) & mask) {
            F[mask] += A[subset];
        }
    }

    return F;
}

// O(n * 2^n)
// n : the number of elements
// A : values of all subsets
// return : F(x) = SUM A[i],  i = subsets of X
inline vector<int> solveSOS(int n, const vector<int>& A) {
    vector<int> F(1 << n);

    for (int mask = 0; mask < (1 << n); mask++)
        F[mask] = A[mask];

    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < (1 << n); mask++) {
            if (mask & (1 << i))
                F[mask] += F[mask ^ (1 << i)];
        }
    }

    return F;
}

