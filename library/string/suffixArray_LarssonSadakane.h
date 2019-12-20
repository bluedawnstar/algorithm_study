#pragma once

// http://www.larsson.dogma.net/ssrev-tr.pdf
// https://www.sciencedirect.com/science/article/pii/S0304397507005257

// from https://github.com/carrotsearch/jsuffixarrays/blob/master/src/main/java/org/jsuffixarrays/QSufSort.java

// "Faster Suffix Sorting", Larsson, N. Jesper and Sadakane, Kunihiko. 
struct SuffixArrayLarssonSadakane {
    vector<int> sa;             // sa
    vector<int> S;              // 

    vector<int> build(const char* str, int n, int charMin = 'a', int charMax = 'z') {
        sa.assign(n + 1, 0);
        S.assign(n + 1, 0);

        for (int i = 0; i < n; i++)
            S[i] = str[i];

        suffixsort(n, charMin, charMax);

        sa.pop_back();
        return sa;
    }

    vector<int> build(const string& s, int charMin = 'a', int charMax = 'z') {
        return build(s.c_str(), int(s.length()), charMin, charMax);
    }

private:
    int currPosition;           // length of already-sorted prefixes

    static int clz(int x) {
#ifndef __GNUC__
        return int(_lzcnt_u32(static_cast<unsigned>(x)));
#else
        return __builtin_clz(static_cast<unsigned>(x));
#endif
    }

    // return (new alphabet size, the number of symbol)
    pair<int,int> transform(int n, int charMin, int charMax, int chunkMaxValue) {
        int size = 0;
        int count = 0;

        int scale = 32 - clz(charMax - charMin + 1);
        int chunkLo = 0;
        int chunkHi = 0;
        for (int e = numeric_limits<int>::max() >> scale; count < n && chunkHi <= e; count++) {
            int c = (chunkHi << scale) | (charMax - charMin + 1);
            if (c > chunkMaxValue)
                break;
            chunkLo = (chunkLo << scale) | (S[count] - charMin + 1);
            chunkHi = c;
        }
        int mask = (1 << count * scale) - 1;
        S[n] = charMin - 1;

        if (chunkHi <= n) {
            fill(sa.begin(), sa.begin() + chunkHi + 1, 0);
            {
                int c = chunkLo;
                for (int i = count; i <= n; i++) {
                    sa[c & mask] = 1;
                    c = (c << scale) | (S[i] - charMin + 1);
                }
                for (int i = 1; i < count; i++) {
                    sa[c & mask] = 1;
                    c <<= scale;
                }
                size = 1;
                for (int i = 0; i <= chunkHi; ++i) {
                    if (sa[i] != 0)
                        sa[i] = size++;
                }
            }
            {
                int i = 0, c = chunkLo;
                for (int j = count; j <= n; i++, j++) {
                    S[i] = sa[c & mask];
                    c = (c << scale) | (S[j] - charMin + 1);
                }
                while (i < n) {
                    S[i++] = sa[c & mask];
                    c <<= scale;
                }
            }
        } else {
            int i = 0, c = chunkLo;
            for (int j = count; j <= n; i++, j++) {
                S[i] = c & mask;
                c = (c << scale) | (S[j] - charMin + 1);
            }
            while (i < n) {
                S[i++] = c & mask;
                c <<= scale;
            }
            size = chunkHi + 1;
        }
        S[n] = 0;
        return{ size, count };
    }

    //---

    int KEY(int idx) const {
        return S[currPosition + sa[idx]];
    }

    int MED3(int a, int b, int c) const {
        return (KEY(a) < KEY(b) ? (KEY(b) < KEY(c) ? (b) : KEY(a) < KEY(c) ? (c) : (a))
            : (KEY(b) > KEY(c) ? (b) : KEY(a) > KEY(c) ? (c) : (a)));
    }

    int choosePivot(int start, int n) {
        int mid = start + (n >> 1);
        if (n > 7) {
            int lo = start;
            int hi = start + n - 1;
            if (n > 40) {
                int s = n >> 3;
                lo = MED3(lo, lo + s, lo + s + s);
                mid = MED3(mid - s, mid, mid + s);
                hi = MED3(hi - s - s, hi - s, hi);
            }
            mid = MED3(lo, mid, hi);
        }
        return KEY(mid);
    }

    void updateGroup(int start, int end) {
        int g = end;
        S[sa[start]] = g;
        if (start == end)
            sa[start] = -1;
        else {
            do
                S[sa[++start]] = g;
            while (start < end);
        }
    }

    void selectSortSplit(int start, int n) {
        int lo = start;
        int hi = start + n - 1;
        while (lo < hi) {
            int j = lo + 1;
            for (int i = j, pivot = KEY(lo); i <= hi; i++) {
                int x = KEY(i);
                if (x < pivot) {
                    pivot = x;
                    swap(sa[i], sa[lo]);
                    j = lo + 1;
                } else if (x == pivot) {
                    swap(sa[i], sa[j]);
                    j++;
                }
            }
            updateGroup(lo, j - 1);
            lo = j;
        }
        if (lo == hi) {
            S[sa[lo]] = lo;
            sa[lo] = -1;
        }
    }

    void sortSplit(int start, int n) {
        if (n < 7) {
            selectSortSplit(start, n);
            return;
        }

        int pivot = choosePivot(start, n);

        int firstL = start;
        int firstR = start;
        int secondL = start + n - 1;
        int secondR = start + n - 1;
        while (true) {
            int f;
            while (firstR <= secondL && (f = KEY(firstR)) <= pivot) {
                if (f == pivot)
                    swap(sa[firstL++], sa[firstR]);
                ++firstR;
            }
            while (secondL >= firstR && (f = KEY(secondL)) >= pivot) {
                if (f == pivot)
                    swap(sa[secondL], sa[secondR--]);
                --secondL;
            }
            if (firstR > secondL)
                break;
            swap(sa[firstR++], sa[secondL--]);
        }

        for (int L = start, R = max(start, firstL); R < firstR; L++, R++)
            swap(sa[L], sa[R]);
        
        int end = start + n;
        for (int L = firstR, R = max(end - (secondR - secondL), secondR + 1); R < end; L++, R++)
            swap(sa[L], sa[R]);

        int firstN = firstR - firstL;
        int secondN = secondR - secondL;
        if (firstN > 0)
            sortSplit(start, firstN);
        updateGroup(start + firstN, start + n - secondN - 1);
        if (secondN > 0)
            sortSplit(start + n - secondN, secondN);
    }

    void bucketsort(int n, int size) {
        fill(sa.begin(), sa.begin() + size, -1);

        for (int i = 0; i <= n; i++) {
            int c = S[i];
            S[i] = sa[c];
            sa[c] = i;
        }
        for (int i = n, j = size - 1; j >= 0; j--) {
            int g = i;

            int c = sa[j];
            int d = S[c];
            S[c] = i;

            if (d >= 0) {
                sa[i--] = c;
                do {
                    c = d;
                    d = S[c];
                    S[c] = g;
                    sa[i--] = c;
                } while (d >= 0);
            } else {
                sa[i--] = -1;
            }
        }
    }

    void suffixsort(int n, int charMin, int charMax) {
        currPosition = 0;

        int size, count;

        if (n >= charMax - charMin + 1) {
            tie(size, count) = transform(n, charMin, charMax, n);
            bucketsort(n, size);
        } else {
            tie(size, count) = transform(n, charMin, charMax, numeric_limits<int>::max());
            for (int i = 0; i <= n; i++)
                sa[i] = i;
            currPosition = 0;
            sortSplit(0, n + 1);
        }
        currPosition = count;
        while (sa[0] >= -n) {
            int i = 0;
            int sl = 0;
            do {
                int s = sa[i];
                if (s < 0) {
                    i -= s;
                    sl += s;
                } else {
                    if (sl != 0) {
                        sa[i + sl] = sl;
                        sl = 0;
                    }
                    int pk = S[s] + 1;
                    sortSplit(i, pk - i);
                    i = pk;
                }
            } while (i <= n);
            if (sl != 0)
                sa[i + sl] = sl;
            currPosition = currPosition << 1;
        }

        for (int i = 0; i <= n; i++) {
            if (S[i] > 0)
                sa[S[i] - 1] = i;
        }
    }
};
