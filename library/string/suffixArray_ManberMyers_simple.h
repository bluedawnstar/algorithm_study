#pragma once

template <int MaxCharN = 26, int BaseChar = 'a'>
struct SuffixArrayManberMyersSimple {
    // O(N*logN)
    static vector<int> build(const char* str, int n) {
        vector<int> sa(n + 1);
        vector<int> rank(n + 1);

        radixSort(str, n, sa.data(), rank.data());

        // sentinels
        sa[n] = n;
        rank[n] = -1;

        vector<int> newRank(n + 1);
        for (int offset = 1; offset < n; offset <<= 1) {
            int count = 0;
            for (int i = 1; i <= n; i++) {
                if (rank[sa[i]] == rank[sa[i - 1]])
                    count++;
                else if (count > 0) {
                    int left = i - 1 - count;
                    int right = i - 1;
                    sort(sa.data() + left, sa.data() + right + 1, [&rank, offset](int a, int b) {
                        return rank[a + offset] < rank[b + offset];
                    });

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

    static vector<int> build(const string& s) {
        return build(s.c_str(), int(s.length()));
    }

private:
    // MSD(most-significant-digit-first) radix sort
    static void radixSort(const char* s, int n, int sa[], int rank[]) {
        int range = MaxCharN;

        vector<int> freq(range + 1);
        for (int i = 0; i < n; i++)
            freq[s[i] - BaseChar + 1]++;
        for (int i = 1; i < range; i++)
            freq[i] += freq[i - 1];

        for (int i = 0; i < n; i++)
            rank[i] = freq[s[i] - BaseChar];

        for (int i = 0; i < n; i++)
            sa[freq[s[i] - BaseChar]++] = i;
    }
};
