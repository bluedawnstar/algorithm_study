#pragma once

// Making directly from string
// This class is not appropriate for LCP calculating.
// Use LcpArraySparseTable to calculate LCP.
// This class is intended for general suffix operation in O(1) or O(logN)
struct SuffixArraySparseTable {
    struct Entry {
        int nr[2], p;
    };

    int N;
    int logN;

    // P[k][i] : the group of 2^k-th character of i-th suffix
    vector<vector<int>> P;          // sparse table
    int H;                          // P's height

    static int ch2i(char ch) {
        return ch - 'a';
    }

    SuffixArraySparseTable(const char* s, int n) {
        build(s, n);
    }

    SuffixArraySparseTable(const string& s) {
        build(&s[0], (int)s.length());
    }

    // (suffix position, suffix position)
    // WARNING: Use LcpArraySparseTable for LCP
    int lcp(int x, int y) {
        int res = 0;

        if (x == y)
            return N - x;

        for (int k = H - 1; k >= 0 && x < N && y < N; k--) {
            if (P[k][x] == P[k][y]) {
                x += 1 << k;
                y += 1 << k;
                res += 1 << k;
            }
        }

        return res;
    }

    //TODO: fill other important algorithms

private:
    // O(NlogN)
    void build(const char* s, int n) {
        N = n;
        logN = (int)log2(n) + 3;
        P = vector<vector<int>>(logN, vector<int>(N));

        vector<Entry> L(N);

        for (int i = 0; i < N; i++)
            P[0][i] = ch2i(s[i]);

        int stp, cnt;
        for (stp = 1, cnt = 1; (cnt >> 1) < N; stp++, cnt <<= 1) {
            for (int i = 0; i < N; i++) {
                L[i].nr[0] = P[stp - 1][i];
                L[i].nr[1] = i + cnt < N ? P[stp - 1][i + cnt] : -1;
                L[i].p = i;
            }
            sort(L.begin(), L.end(), [](const Entry& a, const Entry& b) {
                return a.nr[0] == b.nr[0] ? (a.nr[1] < b.nr[1])
                    : (a.nr[0] < b.nr[0]);
            });

            for (int i = 0; i < N; i++) {
                if ((i > 0) && (L[i].nr[0] == L[i - 1].nr[0]) && (L[i].nr[1] == L[i - 1].nr[1]))
                    P[stp][L[i].p] = P[stp][L[i - 1].p];
                else
                    P[stp][L[i].p] = i;
            }
        }
        H = stp;
    }
};
