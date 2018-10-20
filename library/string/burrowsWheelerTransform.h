#pragma once

// https://en.wikipedia.org/wiki/Burrows%E2%80%93Wheeler_transform

struct BurrowsWheelerTransform {
    static const char FirstChar = 'a';
    static const char LastChar = 'z';

    static const char StartMark = LastChar + 1; // '{'
    static const char EndMark = LastChar + 2;   // '|'

    // return transformed string with start & end mark, O(NlogN)
    static string bwt(const string& s) {
        string t;
        t.reserve(s.length() + 2);
        t += StartMark;
        t += s;
        t += EndMark;

        int N = int(t.length());
        vector<int> SA = buildSuffixArray(t.c_str(), N, FirstChar, EndMark);

        string res;

        res.reserve(N);
        for (int i = 0; i < N; i++) {
            // int j = (SA[i] - 1 + N) % N;
            int j = SA[i] - 1;
            if (j < 0)
                j = j + N;

            res.push_back(t[j]);
        }

        return res;
    }

    // return original string with start & end mark, O(NlogN)
    static string ibwt(const string& s) {
        int N = int(s.length());

        list<int> nodes[128];
        for (int i = 0; i < N; i++) {
            nodes[s[i]].push_back(i);
        }

        string t = s;
        sort(t.begin(), t.end());

        vector<int> l_shift(N);

        for (int i = 0; i < N; i++) {
            l_shift[i] = nodes[t[i]].front();
            nodes[t[i]].pop_front();
        }

        string res;
        res.reserve(N);
        for (int i = 0, x = N - 2; i < N; i++) {
            x = l_shift[x];
            res.push_back(s[x]);
        }

        return res;
    }

private:
    // O(NlogN)
    static vector<int> buildSuffixArray(const char* s, int n, int charMin, int charMax) {
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
};
