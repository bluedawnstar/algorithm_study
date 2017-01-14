#pragma once

#include "../tree/sparseTable.h"
#include "../tree/treeSegmentTree.h"
#include "../tree/treeFenwickTree.h"

// 'T' is a type of result
template <typename T>
struct DistinctSubstringCounter {
    int N;
    int currSuffixPos;

    SparseTableMin  lcpTable;
    RMQ             addedSuffix;    // suffix string poistion
    vector<int>     revSA;          // suffix string position to suffix array index

    FenwickTreeMultAdd<T>   fsum;

    DistinctSubstringCounter(const vector<int>& SA, const vector<int>& lcpArray, int n)
        : lcpTable(lcpArray, n), addedSuffix(n), fsum(n), revSA(n) {
        N = n;
        currSuffixPos = N - 1;

        for (int i = 0; i < N; i++)
            revSA[SA[i]] = i;
    }

    // extend from right to left in string
    // return extended suffix position
    int extend() {
        int suffix = revSA[currSuffixPos];
        int prevLCP = 0;
        while (true) {
            int lo, hi;

            lo = 0, hi = suffix;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                if (lcpTable.query(mid, suffix) > prevLCP)
                    hi = mid - 1;
                else
                    lo = mid + 1;
            }
            int L = lo - 1;

            lo = suffix + 1, hi = N - 1;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                if (lcpTable.query(suffix + 1, mid) <= prevLCP)
                    hi = mid - 1;
                else
                    lo = mid + 1;
            }
            int R = lo - 1;

            int nextSuffixPos = addedSuffix.query(L, R);
            if (nextSuffixPos >= INT_MAX)
                break;
            int nextSuffix = revSA[nextSuffixPos];
            int currLCP = lcpTable.query(min(suffix, nextSuffix) + 1, max(suffix, nextSuffix));
            fsum.addRange(nextSuffixPos + prevLCP, nextSuffixPos + currLCP - 1, -1);
            prevLCP = currLCP;
        }
        addedSuffix.update(suffix, currSuffixPos);
        fsum.addRange(currSuffixPos, N - 1, 1);

        return currSuffixPos--;
    }

    T query(int right) {
        return fsum.sumRange(currSuffixPos + 1, right);
    }
};
