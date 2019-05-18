#pragma once

#include "fractionAlgo.h"

// https://en.wikipedia.org/wiki/Stern%E2%80%93Brocot_tree
/*
  0:   0/1|                                                               |1/0

  1:   0/1|                              1/1                              |1/0
                          +---------------|---------------+                   
  2:   0/1|              1/2             1/1             2/1              |1/0
                  +-------|-------+       |       +-------|-------+
  3:   0/1|      1/3     1/2     2/3     1/1     3/2     2/1     3/1      |1/0
              +---|---+   |   +---|---+   |   +---|---+   |   +---|---+    
  4:   0/1|  1/4 1/3 2/5 1/2 3/5 2/3 3/4 1/1 4/3 3/2 5/3 2/1 5/2 3/1 4/1  |1/0

  ...

*/


/*
F1 = {0/1,                                                                                                          1/1}
F2 = {0/1,                                                   1/2,                                                   1/1}
F3 = {0/1,                               1/3,                1/2,                2/3,                               1/1}
F4 = {0/1,                     1/4,      1/3,                1/2,                2/3,      3/4,                     1/1}
F5 = {0/1,                1/5, 1/4,      1/3,      2/5,      1/2,      3/5,      2/3,      3/4, 4/5,                1/1}
F6 = {0/1,           1/6, 1/5, 1/4,      1/3,      2/5,      1/2,      3/5,      2/3,      3/4, 4/5, 5/6,           1/1}
F7 = {0/1,      1/7, 1/6, 1/5, 1/4, 2/7, 1/3,      2/5, 3/7, 1/2, 4/7, 3/5,      2/3, 5/7, 3/4, 4/5, 5/6, 6/7,      1/1}
F8 = {0/1, 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8, 1/1}
*/
struct SternBrocotRank {
    int maxDenom;
    vector<int> cnt;
    vector<int> phi;

    long long fareySequenceLength;

    SternBrocotRank() : maxDenom(0) {
    }

    explicit SternBrocotRank(int maxDenom) {
        build(maxDenom);
    }

    // O(N*logN)
    void build(int maxDenom) {
        this->maxDenom = maxDenom;

        // build tables to remove duplicates
        vector<int> d(maxDenom + 1, 0);
        d[1] = 1;
        for (int i = 1; i <= maxDenom; i++) {
            for (int j = i * 2; j <= maxDenom; j += i)
                d[j] -= d[i];
            d[i] += d[i - 1];
        }

        cnt.assign(maxDenom + 1, 0);
        for (int i = 1; i <= maxDenom; i++)
            cnt[i] = d[maxDenom / i];

        // build phi table
        phi.resize(maxDenom + 1);
        iota(phi.begin(), phi.end(), 0);
        phi[0] = 1;     // caution

        for (int p = 2; p <= maxDenom; p++) {
            if (phi[p] == p) {
                phi[p] = p - 1;
                for (int i = 2 * p; i <= maxDenom; i += p) {
                    phi[i] = (phi[i] / p) * (p - 1);
                }
            }
        }

        fareySequenceLength = getFareySequenceLength(maxDenom);
    }

    // the number of fractions to less than a/b (values < a/b)
    // O(N)
    long long rank(int a, int b) {
        if (a <= b)
            return rankFarey(a, b);
        else
            return fareySequenceLength * 2 - 2 - rankFarey(b, a);
    }

private:
    //--- sequence length
    // return |f(n)|
    long long getFareySequenceLength(int n) {
        return accumulate(phi.begin(), phi.begin() + n + 1, 0ll);
    }

    // the number of fractions to less than a/b (Frey sequences < a/b, a/b < 1)
    // O(N)
    long long rankFarey(int a, int b) {
        long long res = 0;
        for (int i = 1; i <= maxDenom; i++)
            res += (1ll * i * a / b) * cnt[i];
        return res;
    }
};

struct SternBrocotSequence {
    //--- kth Stern-Brocot sequence - naive solutions

    // 0 < kth, O(N^2)
    static pair<int,int> kthNaive(int maxDenom, int kth) {
        auto seqLen = getFareySequenceLength(maxDenom);
        if (kth < seqLen)
            return kthNaiveFarey(maxDenom, kth);
        else if (kth == seqLen)
            return make_pair(1, 1);
        else {
            auto res = kthNaiveFarey(maxDenom, int(seqLen * 2 - kth));
            swap(res.first, res.second);
            return res;
        }
    }

    // 0 < kth, O(N^2)
    static pair<int,int> kthNaiveRev(int maxDenom, int kth) {
        auto seqLen = getFareySequenceLength(maxDenom);
        if (kth < seqLen)
            return kthNaiveRevFarey(maxDenom, kth);
        else if (kth == seqLen)
            return make_pair(1, 1);
        else {
            auto res = kthNaiveRevFarey(maxDenom, int(seqLen * 2 - kth));
            swap(res.first, res.second);
            return res;
        }
    }

    //--- kth Stern-Brocot sequence - fast solutions

    // the number of fractions to less than a/b (values < a/b)
    // O(N*logN)
    static long long rank(int a, int b, int maxDenom) {
        auto seqLen = getFareySequenceLength(maxDenom);
        if (a <= b)
            return rankFarey(a, b, maxDenom);
        else
            return seqLen * 2 - 2 - rankFarey(b, a, maxDenom);
    }

    // 0 < kth, O(N*(logN)^2)
    static pair<int,int> kthFast(int maxDenom, long long kth) {
        auto seqLen = getFareySequenceLength(maxDenom);
        if (kth < seqLen)
            return kthFastFarey(maxDenom, kth);
        else if (kth == seqLen)
            return make_pair(1, 1);
        else {
            auto res = kthFastFarey(maxDenom, seqLen * 2 - kth);
            swap(res.first, res.second);
            return res;
        }
    }

    // 0 < kth, O(N*logN)
    static pair<int,int> kthFast(SternBrocotRank& rank, int maxDenom, long long kth) {
        auto seqLen = rank.fareySequenceLength;
        if (kth < seqLen)
            return kthFastFarey(rank, maxDenom, kth);
        else if (kth == seqLen)
            return make_pair(1, 1);
        else {
            auto res = kthFastFarey(rank, maxDenom, seqLen * 2 - kth);
            swap(res.first, res.second);
            return res;
        }
    }

    //--- sequence length
    // return |f(n)|
    static long long getSequenceLength(int n) {
        return getFareySequenceLength(n) * 2 - 1;
    }

private:
    //--- kth Farey sequence - naive solutions

    // 0 < kth, O(N^2)
    static pair<int,int> kthNaiveFarey(int maxDenom, int kth) {
        pair<int,int> ab(0, 1);             // first
        pair<int,int> cd(1, maxDenom);      // second

        while (--kth > 0) {
            int k = int((maxDenom + ab.second) / cd.second);
            auto t = ab;
            ab = cd;
            cd.first = k * cd.first - t.first;
            cd.second = k * cd.second - t.second;
        }

        return ab;
    }

    // 0 < kth, O(N^2)
    static pair<int,int> kthNaiveRevFarey(int maxDenom, int kth) {
        pair<int,int> ab(1, 1);                     // last
        pair<int,int> cd(maxDenom - 1, maxDenom);   // before last

        while (--kth > 0) {
            int k = int((maxDenom + ab.second) / cd.second);
            auto t = ab;
            ab = cd;
            cd.first = k * cd.first - t.first;
            cd.second = k * cd.second - t.second;
        }

        return ab;
    }

    //--- kth Farey sequence - fast solutions

    // the number of fractions to less than a/b (Frey sequences < a/b)
    // O(N*logN)
    static long long rankFarey(int a, int b, int maxDenom) {
        vector<long long> cnt(maxDenom + 1);
        for (int i = 0; i <= maxDenom; i++)
            cnt[i] = 1ll * i * a / b;

        for (int i = 1; i <= maxDenom; i++) {
            for (int j = i * 2; j <= maxDenom; j += i)
                cnt[j] -= cnt[i];   // remove duplicates
        }

        return accumulate(cnt.begin(), cnt.end(), 0ll);
    }

    // 0 < kth, O(N*(logN)^2)
    static pair<int,int> kthFastFarey(int maxDenom, long long kth) {
        if (kth == 1)
            return make_pair(0, 1);

        int lo = 0, hi = maxDenom;

        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            long long r = rankFarey(mid, maxDenom, maxDenom) + 1;   // rank + 1(= mid/maxDenom)
            if (r == kth) {
                int g = gcd(mid, maxDenom);
                return make_pair(mid / g, maxDenom / g);
            } else if (r < kth)
                lo = mid + 1;
            else
                hi = mid - 1;
        }

        kth -= rankFarey(lo - 1, maxDenom, maxDenom) + 1;

        vector<pair<int,int>> candidate;
        for (int q = 2; q < maxDenom; q++) {
            int p = int((1ll * lo * q - 1) / maxDenom);
            if (1ll * (lo - 1) * q < 1ll * p * maxDenom && gcd(p, q) == 1)
                candidate.emplace_back(p, q);
        }
        sort(candidate.begin(), candidate.end(), [](const pair<int,int>& L, const pair<int,int>& R) {
            return 1ll * L.first * R.second < 1ll * R.first * L.second;
        });
        return candidate[kth - 1];
    }

    // 0 < kth, O(N*logN)
    static pair<int,int> kthFastFarey(SternBrocotRank& rank, int maxDenom, long long kth) {
        if (kth == 1)
            return make_pair(0, 1);

        int lo = 0, hi = maxDenom;

        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            long long r = rank.rank(mid, maxDenom) + 1;    // rank + 1(= mid/maxDenom)
            if (r == kth) {
                int g = gcd(mid, maxDenom);
                return make_pair(mid / g, maxDenom / g);
            } else if (r < kth)
                lo = mid + 1;
            else
                hi = mid - 1;
        }

        kth -= rank.rank(lo - 1, maxDenom) + 1;

        vector<pair<int,int>> candidate;
        for (int q = 2; q < maxDenom; q++) {
            int p = int((1ll * lo * q - 1) / maxDenom);
            if (1ll * (lo - 1) * q < 1ll * p * maxDenom && gcd(p, q) == 1)
                candidate.emplace_back(p, q);
        }
        sort(candidate.begin(), candidate.end(), [](const pair<int,int>& L, const pair<int,int>& R) {
            return 1ll * L.first * R.second < 1ll * R.first * L.second;
        });
        return candidate[kth - 1];
    }

    //--- sequence length
    // return |f(n)|
    static long long getFareySequenceLength(int n) {
        vector<int> phi(n + 1);
        iota(phi.begin(), phi.end(), 0);
        phi[0] = 1;     // caution

        for (int p = 2; p <= n; p++) {
            if (phi[p] == p) {
                phi[p] = p - 1;
                for (int i = 2 * p; i <= n; i += p) {
                    phi[i] = (phi[i] / p) * (p - 1);
                }
            }
        }

        return accumulate(phi.begin(), phi.end(), 0ll);
    }

    template <typename T>
    static T gcd(T p, T q) {
        if (p < q)
            swap(p, q);

        T t;
        while (q != 0) {
            t = q;
            q = p % q;
            p = t;
        }

        return p;
    }
};
