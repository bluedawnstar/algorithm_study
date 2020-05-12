#pragma once

#include "suffixArray.h"
#include "suffixAutomaton.h"
#include "../rangeQuery/segmentTreeRMQ.h"
#include "../rangeQuery/fenwickTreeMultAdd.h"

// 'T' is a type of result
template <typename T, int MaxCharN = 26, int BaseChar = 'a'>
struct DistinctSubstringCounterWithSuffixArray {
    int N;
    int currSuffixPos;

    SuffixArray<MaxCharN,BaseChar> SA;
    RMQ         addedSuffix;    // suffix string poistion
    vector<int> revSA;          // suffix string position to suffix array index

    FenwickTreeMultAdd<T>   fsum;

    DistinctSubstringCounterWithSuffixArray(const string& s)
        : N(int(s.length())), currSuffixPos(N - 1), SA(s), addedSuffix(N), fsum(N), revSA(N) {

        for (int i = 0; i < N; i++)
            revSA[SA.suffixArray[i]] = i;
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
                int mid = lo + (hi - lo) / 2;
                if (SA.lcp(mid - 1, suffix) > prevLCP)
                    hi = mid - 1;
                else
                    lo = mid + 1;
            }
            int L = lo - 1;

            lo = suffix + 1, hi = N - 1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (SA.lcp(suffix, mid) <= prevLCP)
                    hi = mid - 1;
                else
                    lo = mid + 1;
            }
            int R = lo - 1;

            int nextSuffixPos = addedSuffix.query(L, R);
            if (nextSuffixPos >= INT_MAX)
                break;
            int nextSuffix = revSA[nextSuffixPos];
            int currLCP = SA.lcp(min(suffix, nextSuffix), max(suffix, nextSuffix));
            fsum.addRange(nextSuffixPos + prevLCP, nextSuffixPos + currLCP - 1, -1);
            prevLCP = currLCP;
        }
        addedSuffix.update(suffix, currSuffixPos);
        fsum.addRange(currSuffixPos, N - 1, 1);

        return currSuffixPos--;
    }

    T query(int right) const {
        return fsum.sumRange(currSuffixPos + 1, right);
    }
};

template <typename T, int MaxCharN = 26, int BaseChar = 'a'>
struct DistinctSubstringCounterWithSuffixAutomaton : public SuffixAutomaton<MaxCharN,BaseChar> {
    FenwickTreeMultAdd<T> bit;

    explicit DistinctSubstringCounterWithSuffixAutomaton(int maxN)
        : SuffixAutomaton<MaxCharN,BaseChar>(maxN), bit(maxN) {
        // no action
    }

    void extend(char c) {
        SuffixAutomaton<MaxCharN,BaseChar>::extend(c);
        update(last);
    }

    T query(int left) const {
        return bit.sumRange(left, state[last].len - 1);
    }

private:
    void update(int node) {
        bit.addRange(0, state[node].len - 1, 1);

        int prevRightEnd = -1;
        int rightEnd = state[node].rightEnd;
        int x = state[node].suffixLink;
        while (x > 0) {
            if (state[x].rightEnd != prevRightEnd) {
                if (prevRightEnd >= 0)
                    bit.addRange(prevRightEnd - state[x].len + 1, prevRightEnd, 1);
                bit.addRange(state[x].rightEnd - state[x].len + 1, state[x].rightEnd, -1);
            }
            prevRightEnd = state[x].rightEnd;
            state[x].rightEnd = rightEnd;
            x = state[x].suffixLink;
        }
    }
};
