#pragma once

#include "../tree/sparseTable.h"
#include "../tree/segmentTree.h"
#include "../tree/fenwickTree.h"

// 'T' is a type of result
template <typename T>
struct DistinctSubstringCounterWithSuffixArray {
    int N;
    int currSuffixPos;

    SparseTableMin  lcpTable;
    RMQ             addedSuffix;    // suffix string poistion
    vector<int>     revSA;          // suffix string position to suffix array index

    FenwickTreeMultAdd<T>   fsum;

    DistinctSubstringCounterWithSuffixArray(const vector<int>& SA, const vector<int>& lcpArray, int n)
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

template <typename T>
struct SuffixAutomationWithBIT {
    static const size_t MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }

    struct StateT {
        int len = 0;                // the length of the longest string in a class
        int suffixLink = 0;         //  
        int next[MaxCharN] = { 0, }; // labeled next

        int rightEnd = 0;           //+
    };
    int N;
    vector<StateT> state;           // t0 is zeroth state, other states get numbers 1,2,3,...
    int last;

    FenwickTreeMultAdd<T> bit;

    SuffixAutomationWithBIT(int maxN) : state(maxN * 2), bit(maxN) {
        init();
    }

    void init() {
        last = 0;
        state[0].len = 0;
        state[0].suffixLink = -1;
        N = 1;
    }

    void extend(char c) {
        int i = ch2i(c);

        int cur = N++;
        state[cur].len = state[last].len + 1;

        // add edge from last nodes into new node
        int p;
        for (p = last; p >= 0 && !state[p].next[i]; p = state[p].suffixLink)
            state[p].next[i] = cur;

        if (p == -1) {
            state[cur].suffixLink = 0;                          // set new node's suffix link to t0
        } else {
            int q = state[p].next[i];
            if (state[p].len + 1 == state[q].len) {
                state[cur].suffixLink = q;                      // we do not have to split q, just set the correct suffix link
            } else {
                // we have to split, add q'
                int clone = N++;
                state[clone].len = state[p].len + 1;
                memcpy(state[clone].next, state[q].next, sizeof(state[q].next)); // copy next of q
                state[clone].suffixLink = state[q].suffixLink;          // copy suffix link of q

                state[clone].rightEnd = state[q].rightEnd;              //+

                for (; p >= 0 && state[p].next[i] == q; p = state[p].suffixLink)
                    state[p].next[i] = clone;                           // fix last next of all nodes to have the same class to p

                state[q].suffixLink = state[cur].suffixLink = clone;    // set q and cur's suffix link to clone
            }
        }

        state[cur].rightEnd = state[cur].len - 1;                       //+
        update(cur, state[cur].len);                                    //+

        last = cur;
    }

    T query(int left) {
        return bit.sumRange(left, state[last].len - 1);
    }

private:
    void update(int node, int pos) {
        bit.addRange(0, pos - 1, 1);

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
