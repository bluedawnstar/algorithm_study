#pragma once
#pragma warning(disable: 4334)

///////////////////////////////////////////////////////////////////////////////
// Sum over Subsets(SOS)
// http://codeforces.com/blog/usaxena95
// https://discuss.codechef.com/questions/107073/maxor-ediorial

// O(3^b)
// b : bit size
// A : values of all subsets
// return : F(x) = SUM A[i],  i = subsets of X
inline vector<int> solveSOSNaive(int bitSize, const vector<int>& A) {
    vector<int> F(1 << bitSize);

    for (int mask = 0; mask < (1 << bitSize); mask++) {
        F[mask] = A[0];
        for (int subset = mask; subset; subset = (subset - 1) & mask) {
            F[mask] += A[subset];
        }
    }

    return F;
}

// O(b * 2^b)
// b : bit size
// A : values of all subsets
// return : F(x) = SUM A[i],  i = subsets of X
// 
// https://www.hackerearth.com/practice/algorithms/dynamic-programming/bit-masking/practice-problems/algorithm/compatibility-queries-0c068f8f/
// 
inline vector<int> solveSOS(int bitSize, const vector<int>& A) {
    vector<int> F(1 << bitSize);

    for (int mask = 0; mask < (1 << bitSize); mask++)
        F[mask] = A[mask];

    for (int i = 0; i < bitSize; ++i) {
        for (int mask = 0; mask < (1 << bitSize); mask++) {
            if (mask & (1 << i))
                F[mask] += F[mask ^ (1 << i)];
        }
    }

    return F;
}

