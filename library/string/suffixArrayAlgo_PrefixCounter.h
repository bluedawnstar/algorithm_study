#pragma once

#include "suffixArray.h"

// https://www.hackerearth.com/practice/algorithms/string-algorithm/string-searching/practice-problems/algorithm/prefix-and-substring-queries-87616b64/

struct PrefixCounter {
    struct PersistentSegmentTree {
        int N;
        vector<int> value;
        vector<int> L;
        vector<int> R;

        int initRoot;

        PersistentSegmentTree() {
        }

        explicit PersistentSegmentTree(int n) {
            initRoot = build(n);
        }

        // return root node index
        int build(int n) {
            N = n;
            value.reserve(N * 4);
            L.reserve(N * 4);
            R.reserve(N * 4);
            return initRoot = dfsBuild(0, N - 1);
        }

        //---

        // return root node index, O(logN)
        int add(int root, int index, int val) {
            return dfsAdd(root, 0, N - 1, index, val);
        }

        // O(logN)
        int query(int root, int left, int right) const {
            return dfsQuery(root, 0, N - 1, left, right);
        }

    private:
        int dfsBuild(int left, int right) {
            int l = -1, r = -1;
            if (left < right) {
                int mid = (left + right) >> 1;
                l = dfsBuild(left, mid);
                r = dfsBuild(mid + 1, right);
            }
            value.push_back(0);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsAdd(int node, int left, int right, int index, int val) {
            int newValue;
            int l = -1, r = -1;
            if (left == right) {
                newValue = value[node] + val;
            } else {
                int mid = (left + right) >> 1;
                if (index <= mid) {
                    l = dfsAdd(L[node], left, mid, index, val);
                    r = R[node];
                } else {
                    l = L[node];
                    r = dfsAdd(R[node], mid + 1, right, index, val);
                }
                newValue = value[l] + value[r];
            }
            value.push_back(newValue);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsQuery(int node, int left, int right, int indexL, int indexR) const {
            if (indexR < left || right < indexL)
                return 0;

            if (indexL <= left && right <= indexR)
                return value[node];

            int mid = (left + right) >> 1;
            return dfsQuery(L[node], left, mid, indexL, indexR)
                + dfsQuery(R[node], mid + 1, right, indexL, indexR);
        }
    };

    vector<int> roots;
    PersistentSegmentTree segTree;
    SuffixArray sa;

    void build(const char s[], int n) {
        sa.build(s, n);

        roots.clear();
        int lastRoot = segTree.build(n);
        for (int i = 0; i < n; i++) {
            lastRoot = segTree.add(lastRoot, sa.suffixArray[i], 1);
            roots.push_back(lastRoot);
        }
    }

    void build(const string& s) {
        return build(s.data(), int(s.length()));
    }

    // return the number of s[0..prefixLength-1] in [left, right]
    int query(int prefixLength, int left, int right) const {
        int res = 0;
        if (right - left + 1 >= prefixLength) {
            auto idxL = sa.upperBoundLcpBackward(sa.suffixArrayRev[0], prefixLength) + 1;
            auto idxR = sa.upperBoundLcpForward(sa.suffixArrayRev[0], prefixLength) - 1;
            if (idxL <= idxR) {
                res = segTree.query(roots[idxR], left, right - prefixLength + 1);
                if (idxL > 0)
                    res -= segTree.query(roots[idxL - 1], left, right - prefixLength + 1);
            }
        }
        return res;
    }
};
