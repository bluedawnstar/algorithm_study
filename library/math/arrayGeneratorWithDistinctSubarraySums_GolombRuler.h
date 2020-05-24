#pragma once

#include "golombRuler.h"


// algorithm to generate arrays with distinct subarray sums based on Golomb ruler
struct ArrayGeneratorWithDistinctSubarraySums {
    // O(n)
    static vector<int> build(int N) {
        int p = N + 1;
        while (!isPrimeNumber(p))
            p++;

        vector<int> S;
        S.reserve(N + 1);

        // Golomb ruler
        S.push_back(0);
        for (int i = 1; i <= N; i++)
            S.push_back(2 * i * p + i * i % p);

        //---

        vector<int> res;
        res.reserve(N);
        for (int i = 1; i <= N; i++)
            res.push_back(S[i] - S[i - 1]);

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

template <int MaxSum>
struct DistinctSubarrySumChecker {
    static bool check(const vector<int>& A, int N) {
        static bitset<MaxSum + 1> S;
        S.reset();

        for (int i = 0; i < N; i++) {
            int sum = 0;
            for (int j = i; j < N; j++) {
                sum += A[j];
                if (S.test(sum))
                    return false;
                S.set(sum);
            }
        }

        return true;
    }

    //--- incremental checker

    bitset<MaxSum + 1> bs;
    vector<int> sum;
    int index;

    void init(int N) {
        bs.reset();
        sum = vector<int>(N);
        index = 0;
    }

    bool checkAndAdd(int x) {
        for (int i = 0; i <= index; i++) {
            if (bs.test(sum[i] + x))
                return false;
        }

        for (int i = 0; i <= index; i++) {
            sum[i] += x;
            bs.set(sum[i]);
        }

        index++;

        return true;
    }
};
