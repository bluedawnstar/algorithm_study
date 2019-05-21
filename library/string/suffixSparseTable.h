#pragma once

// This class is not appropriate for LCP calculating.
// Use LcpArraySparseTable to calculate LCP fast.
// This class is intended for general suffix operation in O(logN)

// It's slower than SuffixArray and SimpleSuffixArray.
struct SuffixSparseTable {
    static int ch2i(char ch) {
        return ch - 'a';
    }

    //---

    struct Entry {
        pair<int,int> g;
        int p;

        bool operator <(const Entry& rhs) const {
            return g < rhs.g;
        }
    };

    int N;
    string S;

    // P[k][i] : the group of 2^k-th character of i-th suffix
    vector<vector<int>> P;          // sparse table
    int H;                          // P's height

    SuffixSparseTable(const char* s, int n) {
        build(s, n);
    }

    explicit SuffixSparseTable(const string& s) {
        build(s.c_str(), int(s.length()));
    }


    // O(N*(logN)^2)
    void build(const char* s, int n) {
        S.assign(s, s + n);

        N = n;
        int logN = int(log2(n)) + 3;
        P = vector<vector<int>>(logN, vector<int>(N));

        vector<Entry> L(N);

        for (int i = 0; i < N; i++)
            P[0][i] = ch2i(s[i]);

        int step = 1;
        for (int cnt = 1; (cnt >> 1) < N; step++, cnt <<= 1) {
            for (int i = 0; i < N; i++) {
                L[i].g.first = P[step - 1][i];
                L[i].g.second = (i + cnt < N) ? P[step - 1][i + cnt] : -1;
                L[i].p = i;
            }
            sort(L.begin(), L.end());

            P[step][L[0].p] = 0;
            for (int i = 1; i < N; i++) {
                if (L[i].g == L[i - 1].g)
                    P[step][L[i].p] = P[step][L[i - 1].p];
                else
                    P[step][L[i].p] = i;
            }
        }
        H = step;
    }

    void build(const string& s) {
        build(s.c_str(), int(s.length()));
    }

    //--- LCP and compare

    // longest common prefix
    // (x, y) = (suffix position, suffix position), O(logN)
    int lcp(int x, int y) const {
        int res = 0;

        if (x == y)
            return N - x;

#if 0
        for (int k = H - 1; k >= 0 && x < N && y < N; k--) {
            if (P[k][x] == P[k][y]) {
                x += 1 << k;
                y += 1 << k;
                res += 1 << k;
            }
        }
#else
        while (x < N && y < N && P[0][x] == P[0][y]) {
            int k;
            for (k = 1; k < H && P[k][x] == P[k][y]; k++)
                ;
            x += 1 << (k - 1);
            y += 1 << (k - 1);
            res += 1 << (k - 1);
        }
#endif

        return res;
    }

    // (x, y) = (suffix position, suffix position), O(logN)
    int compare(int x, int y) const {
        if (x == y)
            return 0;

        int lenX = N - x;
        int lenY = N - y;
        int n = lcp(x, y);

        if (n < lenX && n < lenY)
            //return (S[x + n] < S[y + n]) ? -1 : 1;
            return (S[x + n] - S[y + n]);
        else
            //return (n == lenX) ? -1 : 1;
            return lenX - lenY;
    }

    //---

    // O(N*(logN)^2)
    vector<int> buildSuffixArray() const {
        vector<int> sa(N);
        iota(sa.begin(), sa.end(), 0);
        sort(sa.begin(), sa.end(), [this](int a, int b) {
            return compare(a, b) < 0;
        });
        return sa;
    }

    //---

    // O(NlogN), but fast enough in normal situation
    vector<int> buildLcpArray(const vector<int>& sa) const {
        vector<int> res(N);
        for (int i = 1; i < N; i++)
            res[i] = lcp(sa[i - 1], sa[i]);
        return res;
    }

    // Kasai algorithm - O(N)
    vector<int> buildLcpArrayFast(const vector<int>& sa) const {
        vector<int> rank(N), height(N);
        for (int i = 0; i < N; i++)
            rank[sa[i]] = i;

        for (int i = 0, h = 0; i < N; i++) {
            if (rank[i] > 0) {
                int j = sa[rank[i] - 1];
                while (i + h < N && j + h < N && S[i + h] == S[j + h])
                    h++;
                height[rank[i]] = h;
                if (h > 0)
                    h--;
            }
        }

        return height;
    }
};
