#pragma once

//#define USE_DOUBLE_HASH

struct SuffixRollingHash {
    static const int P1 = 31;
#ifdef USE_DOUBLE_HASH
    static const int P2 = 41;
#endif
    static int ch2i(int ch) {
        return ch - 'a' + 1;
    }

    int N;
    string S;

    vector<unsigned long long> pow1, hash1;
#ifdef USE_DOUBLE_HASH
    vector<unsigned long long> pow2, hash2;
#endif

    SuffixRollingHash(const char* s, int n) {
        build(s, n);
    }

    explicit SuffixRollingHash(const string& s) {
        build(s.c_str(), int(s.length()));
    }


    // O(N)
    void build(const char* s, int n) {
        N = n;
        S.assign(s, s + n);

#ifndef USE_DOUBLE_HASH
        pow1.resize(n + 1);
        pow1[0] = 1;
        for (int i = 1; i <= n; i++)
            pow1[i] = pow1[i - 1] * P1;

        hash1.resize(n + 1);
        for (int i = 0; i < n; i++)
            hash1[i + 1] = hash1[i] + ch2i(s[i]) * pow1[i];
#else
        pow1.resize(n + 1);
        pow2.resize(n + 1);
        pow1[0] = 1;
        pow2[0] = 1;
        for (int i = 1; i <= n; i++) {
            pow1[i] = pow1[i - 1] * P1;
            pow2[i] = pow2[i - 1] * P2;
        }

        hash1.resize(n + 1);
        hash2.resize(n + 1);
        for (int i = 0; i < n; i++) {
            hash1[i + 1] = hash1[i] + ch2i(s[i]) * pow1[i];
            hash2[i + 1] = hash2[i] + ch2i(s[i]) * pow2[i];
        }
#endif
    }

    void build(const string& s) {
        build(s.c_str(), int(s.length()));
    }

    //--- LCP and compare

    // longest common prefix
    // (x, y) = (suffix position, suffix position), O(logN)
    int lcp(int x, int y) const {
        if (x == y)
            return N - x;

        if (x > y)
            swap(x, y);
        int n = N - y;

        unsigned long long P1 = pow1[y - x];
#ifdef USE_DOUBLE_HASH
        unsigned long long P2 = pow2[y - x];
#endif

#if 0
        int lo = 1, hi = n;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;

#ifdef USE_DOUBLE_HASH
            auto h1 = (hash1[x + mid] - hash1[x]) * P1;
            auto h2 = (hash2[x + mid] - hash2[x]) * P2;
            if (h1 == (hash1[y + mid] - hash1[y]) && h2 == (hash2[y + mid] - hash2[y]))
#else
            auto h1 = (hash1[x + mid] - hash1[x]) * P1;
            if (h1 == (hash1[y + mid] - hash1[y]))
#endif
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        return lo - 1;
#else
        int lo = 0, hi = n - 1;

        while (lo <= hi && S[x + lo] == S[y + lo]) {
            ++lo;

            int step;
            for (step = 1; lo + step <= hi; step <<= 1) {
#ifdef USE_DOUBLE_HASH
                auto h1 = (hash1[x + lo + step] - hash1[x]) * P1;
                auto h2 = (hash2[x + lo + step] - hash2[x]) * P2;
                if (h1 != (hash1[y + lo + step] - hash1[y]) || h2 != (hash2[y + lo + step] - hash2[y]))
                    break;
#else
                auto h1 = (hash1[x + lo + step] - hash1[x]) * P1;
                if (h1 != (hash1[y + lo + step] - hash1[y]))
                    break;
#endif
            }
            lo += step >> 1;
        }

        return lo;
#endif
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
