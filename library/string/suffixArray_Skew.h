#pragma once

// "Simple linear work suffix array construction", J. Karkkainen and P. Sanders., 2003
// Skew method, O(N)

template <typename T = char, int MaxCharN = 26, int BaseChar = 'a'>
struct SuffixArraySkew {
    // O(N), but slower than Larsson-Sadakane
    static vector<int> build(const T s[], int n) {
        if (n < 2)
            return{};

        int size = MaxCharN;
        vector<int> sa(n + 3);

        vector<int> in(n + 3);
        for (int i = 0; i < n; i++)
            in[i] = s[i] - BaseChar + 1;

        buildSub(in, sa, n, size);
        sa.resize(n);

        return sa;
    }

    static vector<int> build(const vector<T>& s) {
        return build(s.data(), int(s.size()));
    }

    static vector<int> build(const string& s) {
        vector<T> s2;
        s2.reserve(s.size());
        for (auto c : s)
            s2.push_back(c);
        return build(s2.data(), int(s2.size()));
    }

private:
    // stably sort src[0..n-1] to dst[0..n-1] with keys in 0..K from s
    static void radixPass(const vector<int>& src, vector<int>& dst, vector<int>& s, int offset, int n, int K) {
        vector<int> cnt(K + 1);
        for (int i = 0; i < n; i++)
            cnt[s[src[i] + offset]]++;

        for (int i = 0, sum = 0; i <= K; i++) {
            int t = cnt[i];
            cnt[i] = sum;
            sum += t;
        }
        for (int i = 0; i < n; i++)
            dst[cnt[s[src[i] + offset]]++] = src[i];
    }

    static bool leq(int a1, int a2, int b1, int b2) {
        return (a1 < b1) || (a1 == b1 && a2 <= b2);
    }

    static bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
        return (a1 < b1) || (a1 == b1 && leq(a2, a3, b2, b3));
    }

    // building suffix array O(N)
    // find the suffix array SA of s[0..n-1] in {1..K}^n
    // require s[n] = s[n+1] = s[n+2] = 0, n >= 2
    static void buildSub(vector<int>& s, vector<int>& sa, int n, int K) {
        int n0 = (n + 2) / 3;
        int n1 = (n + 1) / 3;
        int n2 = n / 3;
        int n02 = n0 + n2;
        vector<int> s12(n02 + 3);
        vector<int> sa12(n02 + 3);
        vector<int> s0(n0);
        vector<int> sa0(n0);

        s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0;
        sa12[n02] = sa12[n02 + 1] = sa12[n02 + 2] = 0;

        // generate positions of mod 1 and mod  2 suffixes
        // the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
        for (int i = 0, j = 0; i < n + (n0 - n1); i++) {
            if (i % 3 != 0)
                s12[j++] = i;
        }

        // LSB radix sort the mod 1 and mod 2 triples
        radixPass(s12, sa12, s, 2, n02, K);
        radixPass(sa12, s12, s, 1, n02, K);
        radixPass(s12, sa12, s, 0, n02, K);

        // find lexicographic names of triples
        int name = 0, c0 = -1, c1 = -1, c2 = -1;
        for (int i = 0; i < n02; i++) {
            if (s[sa12[i]] != c0 || s[sa12[i] + 1] != c1 || s[sa12[i] + 2] != c2) {
                name++;
                c0 = s[sa12[i]];
                c1 = s[sa12[i] + 1];
                c2 = s[sa12[i] + 2];
            }
            if (sa12[i] % 3 == 1)
                s12[sa12[i] / 3] = name;        // left half
            else
                s12[sa12[i] / 3 + n0] = name;   // right half
        }

        // recurse if names are not yet unique
        if (name < n02) {
            buildSub(s12, sa12, n02, name);
            // store unique names in s12 using the suffix array 
            for (int i = 0; i < n02; i++)
                s12[sa12[i]] = i + 1;
        } else {
            // generate the suffix array of s12 directly
            for (int i = 0; i < n02; i++)
                sa12[s12[i] - 1] = i;
        }

        // stably sort the mod 0 suffixes from sa12 by their first character
        for (int i = 0, j = 0; i < n02; i++) {
            if (sa12[i] < n0)
                s0[j++] = 3 * sa12[i];
        }
        radixPass(s0, sa0, s, 0, n0, K);

        // merge sorted sa0 suffixes and sorted sa12 suffixes
        for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
#define getI() (sa12[t] < n0 ? sa12[t] * 3 + 1 : (sa12[t] - n0) * 3 + 2)
            int i = getI(); // pos of current offset 12 suffix
            int j = sa0[p]; // pos of current offset 0  suffix
            if (sa12[t] < n0 ? leq(s[i], s12[sa12[t] + n0], s[j], s12[j / 3])
                             : leq(s[i], s[i + 1], s12[sa12[t] - n0 + 1], s[j], s[j + 1], s12[j / 3 + n0])) {
                // suffix from sa12 is smaller
                sa[k] = i;
                if (++t == n02) {
                    // done - only sa0 suffixes left
                    for (k++; p < n0; p++, k++)
                        sa[k] = sa0[p];
                }
            } else {
                sa[k] = j;
                if (++p == n0) {
                    // done - only sa12 suffixes left
                    for (k++; t < n02; t++, k++)
                        sa[k] = getI();
                }
            }
#undef getI
        }
    }
};
