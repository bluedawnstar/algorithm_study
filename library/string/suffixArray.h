#pragma once

/*
    <Benchmark Test>

    -------------------+----------------+------------------+-------------------
         Test Case     |   T        N   | Algorithm        | Speed (sec)
    -------------------+----------------+------------------+--------------------
      random string    | 10000    10000 | Basic            |  2.637 (1)
                       |                | Larsson-Sadakane |  3.812 (2)
                       |                | Manber-Myers     |  7.682
                       |                | Skew             |  5.222
                       +----------------+------------------+-------------------
                       |  1000   100000 | Basic            |  4.178 (2)
                       |                | Larsson-Sadakane |  3.524 (1)
                       |                | Manber-Myers     | 11.336
                       |                | Skew             |  7.279
                       +----------------+------------------+-------------------
                       |   100  1000000 | Basic            | 48.459
                       |                | Larsson-Sadakane |  6.239 (1)
                       |                | Manber-Myers     | 26.858
                       |                | Skew             | 14.278 (2)
    -------------------+----------------+------------------+--------------------
     repeated string   | 10000    10000 | Basic            |  8.502
  (ex: "abaaba...aba") |                | Larsson-Sadakane |  5.908 (1)
                       |                | Manber-Myers     |  9.799
                       |                | Skew             |  5.143 (1)
                       +----------------+------------------+-------------------
                       |  1000   100000 | Basic            | 10.36
                       |                | Larsson-Sadakane |  7.122 (2)
                       |                | Manber-Myers     | 11.853
                       |                | Skew             |  5.202 (1)
                       +----------------+------------------+-------------------
                       |   100  1000000 | Basic            | 20.531
                       |                | Larsson-Sadakane | 10.823 (2)
                       |                | Manber-Myers     | 25.701
                       |                | Skew             |  6.784 (1)
    -------------------+----------------+------------------+--------------------
*/

#include "lcpArraySparseTable.h"

struct SuffixArray {
    vector<int>         suffixArray;
    vector<int>         lcpArray;
    LcpArraySparseTable lcpSparseTable;

    vector<int>         suffixArrayRev;

    SuffixArray() {
    }

    SuffixArray(const char* s, int n, int charMin = 'a', int charMax = 'z') {
        build(s, n, charMin, charMax);
    }

    SuffixArray(const string& s, int charMin = 'a', int charMax = 'z') {
        build(s, charMin, charMax);
    }

    void build(const char* s, int n, const vector<int>& sa, int charMin = 'a', int charMax = 'z') {
        suffixArray = sa;
        lcpArray = SuffixArray::buildLcpArray(suffixArray, s, n);
        lcpSparseTable.build(lcpArray);

        suffixArrayRev.resize(n);
        for (int i = 0; i < n; i++)
            suffixArrayRev[suffixArray[i]] = i;
    }

    void build(const char* s, int n, int charMin = 'a', int charMax = 'z') {
        suffixArray = SuffixArray::buildSuffixArray(s, n, charMin, charMax);
        lcpArray = SuffixArray::buildLcpArray(suffixArray, s, n);
        lcpSparseTable.build(lcpArray);

        suffixArrayRev.resize(n);
        for (int i = 0; i < n; i++)
            suffixArrayRev[suffixArray[i]] = i;
    }

    void build(const string& s, int charMin = 'a', int charMax = 'z') {
        build(&s[0], int(s.length()), charMin, charMax);
    }


    int size() const {
        return int(suffixArray.size());
    }

    // get suffix index from suffix array index
    int operator[](int index) const {
        return suffixArray[index];
    }

    // get suffix array index from suffix index
    int suffixToSuffixArray(int index) const {
        return suffixArrayRev[index];
    }

    // inclusive (left index to Suffix Array, left index to Suffix Array) -- not suffix index
    int lcp(int left, int right) const {
        if (left == right)
            return int(suffixArray.size()) - suffixArray[left];
        else
            return lcpSparseTable.lcp(left, right);
    }

    // inclusive (left suffix index, right suffix index)
    int lcpWithSuffixIndex(int left, int right) const {
        int saL = suffixArrayRev[left];
        int saR = suffixArrayRev[right];
        return lcp(min(saL, saR), max(saL, saR));
    }

    //--- find

    // return min{x | lcp(left, x) <= length}, (left < x < N)
    // - left : left index of suffix array
    // - length : LCP length
    //
    //  .....OOOOOOOOOOOOOOOOOOOOOOOxxxxxxxxx
    //       left(lcp > length)     ^
    int lowerBoundLcpForward(int left, int length) const {
        int n = int(suffixArray.size());
        if (left >= n - 1)
            return n;

        int lo = left + 1, hi = n - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (lcp(left, mid) <= length)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return lo;
    }

    // return min{x | lcp(left, x) < length}, (left < x < N)
    // - left : left index of suffix array
    // - length : LCP length
    //
    //  .....OOOOOOOOOOOOOOOOOOOOOOOxxxxxxxxx
    //       left(lcp >= length)    ^
    int upperBoundLcpForward(int left, int length) const {
        int n = int(suffixArray.size());
        if (left >= n - 1)
            return n;

        int lo = left + 1, hi = n - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (lcp(left, mid) < length)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return lo;
    }

    // return max{x | lcp(x, right) <= length}, (0 <= x < right)
    // - right : right index of suffix array
    // - length : LCP length
    // 
    //  xxxxxxxxxOOOOOOOOOOOOOOOOOOOOOOO.....
    //          ^      (lcp > length)right
    int lowerBoundLcpBackward(int right, int length) const {
        int n = int(suffixArray.size());
        if (right <= 0)
            return -1;

        int lo = 0, hi = right - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (lcp(mid, right) > length)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return hi;
    }

    // return max{x | lcp(x, right) < length}, (0 <= x < right)
    // - right : right index of suffix array
    // - length : LCP length
    // 
    //  xxxxxxxxxOOOOOOOOOOOOOOOOOOOOOOO.....
    //          ^     (lcp >= length)right
    int upperBoundLcpBackward(int right, int length) const {
        int n = int(suffixArray.size());
        if (right <= 0)
            return -1;

        int lo = 0, hi = right - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (lcp(mid, right) >= length)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return hi;
    }

    //------------------------------------------------------------------------

    // O(NlogN)
    static vector<int> buildSuffixArray(const char* s, int n, int charMin = 'a', int charMax = 'z') {
        vector<int> SA(n);
        if (n <= 1)
            return SA;

        int m = charMax - charMin + 1;
        vector<int> cnt(max(n, m)), currG(n), nextG(n);

        for (int i = 0; i < n; i++) {
            currG[i] = s[i] - charMin;
            ++cnt[currG[i]];
        }
        for (int i = 1; i < m; i++)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--)
            SA[--cnt[currG[i]]] = i;

        int len = 1;
        do {
            int g = 0;
            for (int i = n - len; i < n; i++)
                nextG[g++] = i;
            for (int i = 0; i < n; i++) {
                if (SA[i] >= len)
                    nextG[g++] = SA[i] - len;
            }

            fill(cnt.begin(), cnt.begin() + m, 0);
            for (int i = 0; i < n; i++)
                ++cnt[currG[nextG[i]]];
            for (int i = 1; i < m; i++)
                cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--)
                SA[--cnt[currG[nextG[i]]]] = nextG[i];

            int curSA = SA[0];
            nextG[curSA] = g = 0;
            for (int i = 1; i < n; i++) {
                int prvSA = curSA;
                curSA = SA[i];

                nextG[curSA] = (prvSA + len < n && curSA + len < n
                    && currG[prvSA] == currG[curSA]
                    && currG[prvSA + len] == currG[curSA + len])
                    ? g : ++g;
            }
            swap(currG, nextG);

            len <<= 1;
            m = g + 1;
        } while (m < n);

        return SA;
    }

    static vector<int> buildSuffixArray(const string& s, int charMin = 'a', int charMax = 'z') {
        return buildSuffixArray(&s[0], int(s.length()), charMin, charMax);
    }

    // Kasai algorithm - O(N)
    static vector<int> buildLcpArray(const vector<int>& suffixArray, const char* s, int n) {
        vector<int> rank(n), height(n);
        for (int i = 0; i < n; i++)
            rank[suffixArray[i]] = i;

        for (int i = 0, h = 0; i < n; i++) {
            if (rank[i] > 0) {
                int j = suffixArray[rank[i] - 1];
                while (i + h < n && j + h < n && s[i + h] == s[j + h])
                    h++;
                height[rank[i]] = h;
                if (h > 0)
                    h--;
            }
        }

        return height;
    }

    static vector<int> buildLcpArray(const vector<int>& suffixArray, const string& s) {
        return buildLcpArray(suffixArray, &s[0], int(s.length()));
    }
};
