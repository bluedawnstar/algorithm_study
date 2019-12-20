#pragma once

// "Suffix arrays: A new method for on-line string searches", Udi Manber and Gene Myers
// Doubling algorithm (Manber, Myers)

#if 1
//--- type #1
// modified version of https://algs4.cs.princeton.edu/63suffix/Manber.java.html
struct SuffixArrayManberMyers {
    // O(NlogN)
    static vector<int> build(const char* str, int n, int charMin = 'a', int charMax = 'z') {
        vector<int> sa(n + 1);
        vector<int> rank(n + 1);

        radixSort(str, n, charMin, charMax, sa.data(), rank.data());

        // sentinels
        sa[n] = n;
        rank[n] = -1;

        unique_ptr<int[]> newRank(new int[n + 1]);
        for (int offset = 1; offset < n; offset <<= 1) {
            int count = 0;
            for (int i = 1; i <= n; i++) {
                if (rank[sa[i]] == rank[sa[i - 1]])
                    count++;
                else if (count > 0) {
                    int left = i - 1 - count;
                    int right = i - 1;
                    quicksort(sa.data(), rank.data() + offset, left, right);

                    int rankBase = rank[sa[left]];
                    int r = rankBase;
                    for (int j = left + 1; j <= right; j++) {
                        if (rank[sa[j - 1] + offset] < rank[sa[j] + offset])
                            r = rankBase + j - left;
                        newRank[sa[j]] = r;
                    }

                    for (int j = left + 1; j <= right; j++)
                        rank[sa[j]] = newRank[sa[j]];

                    count = 0;
                }
            }
        }

        sa.pop_back();
        return sa;
    }

    static vector<int> build(const string& s, int charMin = 'a', int charMax = 'z') {
        return build(s.c_str(), int(s.length()), charMin, charMax);
    }

private:
    // MSD(most-significant-digit-first) radix sort
    static void radixSort(const char* s, int n, int charMin, int charMax, int sa[], int rank[]) {
        int range = charMax - charMin + 1;

        vector<int> freq(range + 1);
        for (int i = 0; i < n; i++)
            freq[s[i] - charMin + 1]++;
        for (int i = 1; i < range; i++)
            freq[i] += freq[i - 1];

        for (int i = 0; i < n; i++)
            rank[i] = freq[s[i] - charMin];

        for (int i = 0; i < n; i++)
            sa[freq[s[i] - charMin]++] = i;
    }

    static int choosePivot(int sa[], int rank[], int left, int right) {
#define KEY(idx)    rank[sa[idx]]
#define MED3(a,b,c) (KEY(a) < KEY(b) ? (KEY(b) < KEY(c) ? (b) : KEY(a) < KEY(c) ? (c) : (a)) \
                                     : (KEY(b) > KEY(c) ? (b) : KEY(a) > KEY(c) ? (c) : (a)))
        int n = right - left + 1;
        int mid = (left + right) >> 1;
        if (n > 7) {
            int lo = left;
            int hi = left + n - 1;
            if (n > 40) {
                int s = n >> 3;
                lo = MED3(lo, lo + s, lo + s + s);
                mid = MED3(mid - s, mid, mid + s);
                hi = MED3(hi - s - s, hi - s, hi);
            }
            mid = MED3(lo, mid, hi);
        }
        return mid;
#undef KEY
#undef MED3
    }

    // 3-way quicksort
    static void quicksort(int sa[], int rank[], int left, int right) {
        if (right <= left)
            return;

        int pivot = choosePivot(sa, rank, left, right);
        if (pivot != right)
            swap(sa[pivot], sa[right]);
        int v = sa[right];

        int i = left - 1, j = right;
        int p = left - 1, q = right;
        while(true) {
            while (rank[sa[++i]] < rank[v])
                ;
            while (rank[v] < rank[sa[--j]]) {
                if (j == left)
                    break;
            }
            if (i >= j)
                break;
            swap(sa[i], sa[j]);
            if (rank[sa[i]] == rank[v])
                swap(sa[++p], sa[i]);
            if (rank[sa[j]] == rank[v])
                swap(sa[--q], sa[j]);
        }
        swap(sa[i], sa[right]);
        j = i - 1;
        i = i + 1;
        for (int k = left; k < p; k++)
            swap(sa[k], sa[j--]);
        for (int k = right - 1; k > q; k--)
            swap(sa[k], sa[i++]);
        quicksort(sa, rank, left, j);
        quicksort(sa, rank, i, right);
    }
};
#else
//--- type #2
// from https://apps.topcoder.com/forums/?module=RevisionHistory&messageID=1171511
struct SuffixArrayManberMyers {
    // O(NlogN)
    static vector<int> build(const char* str, int n) {
        vector<int> sa(n), saInv(n);
        vector<int> cnt(n), next(n), bh(n), b2h(n);

        // sort suffixes according to their first characters
        for (int i = 0; i < n; i++)
            sa[i] = i;
        sort(sa.begin(), sa.end(), [str](int a, int b) { return str[a] < str[b]; });

        for (int i = 0; i < n; i++) {
            bh[i] = (i == 0 || str[sa[i]] != str[sa[i - 1]]);
            b2h[i] = false;
        }

        for (int h = 1; h < n; h <<= 1){
            int buckets = 0;
            for (int i=0, j; i < n; i = j) {
                j = i + 1;
                while (j < n && !bh[j])
                    j++;
                next[i] = j;
                buckets++;
            }
            if (buckets == n)
                break;

            for (int i = 0; i < n; i = next[i]) {
                cnt[i] = 0;
                for (int j = i; j < next[i]; j++)
                    saInv[sa[j]] = i;
            }

            cnt[saInv[n - h]]++;
            b2h[saInv[n - h]] = true;
            for (int i = 0; i < n; i = next[i]) {
                for (int j = i; j < next[i]; j++) {
                    int s = sa[j] - h;
                    if (s >= 0) {
                        int head = saInv[s];
                        saInv[s] = head + cnt[head]++;
                        b2h[saInv[s]] = true;
                    }
                }
                for (int j = i; j < next[i]; j++) {
                    int s = sa[j] - h;
                    if (s >= 0 && b2h[saInv[s]]) {
                        for (int k = saInv[s]+1; k < n && !bh[k] && b2h[k]; k++)
                            b2h[k] = false;
                    }
                }
            }
            for (int i = 0; i < n; i++) {
                sa[saInv[i]] = i;
                bh[i] |= b2h[i];
            }
        }

        return sa;
    }

    static vector<int> build(const string& s) {
        return build(s.c_str(), int(s.length()));
    }
};
#endif
