#pragma once

/*
    <Speed test result>

    N = 10^6

    ------------+-------------------------+-------------------------
                |      Fast solution      |     Simple solution
       Used     +-------------+-----------+-------------+-----------
     characters | SA building | LCP array | SA building | LCP array
    ------------+-------------+-----------+-------------+-----------
            26  | 0.45        | 0.039     | 0.515       | 0.039
            25  | 0.383       | 0.036     | 0.504       | 0.033
            24  | 0.372       | 0.035     | 0.507       | 0.035
            23  | 0.362       | 0.036     | 0.507       | 0.039
            22  | 0.372       | 0.035     | 0.512       | 0.036
            21  | 0.35        | 0.035     | 0.509       | 0.032
            20  | 0.35        | 0.035     | 0.528       | 0.032
            19  | 0.357       | 0.035     | 0.522       | 0.033
            18  | 0.346       | 0.035     | 0.524       | 0.035
            17  | 0.338       | 0.036     | 0.525       | 0.034
            16  | 0.344       | 0.037     | 0.555       | 0.038
            15  | 0.352       | 0.042     | 0.569       | 0.037
            14  | 0.343       | 0.038     | 0.552       | 0.036
            13  | 0.334       | 0.037     | 0.559       | 0.037
            12  | 0.324       | 0.036     | 0.575       | 0.037
            11  | 0.317       | 0.037     | 0.582       | 0.038
            10  | 0.325       | 0.037     | 0.59        | 0.039
             9  | 0.309       | 0.037     | 0.61        | 0.045
             8  | 0.305       | 0.038     | 0.624       | 0.048
             7  | 0.308       | 0.037     | 0.647       | 0.054
             6  | 0.288       | 0.037     | 0.688       | 0.057
             5  | 0.418       | 0.039     | 0.728       | 0.067
             4  | 0.359       | 0.04      | 0.788       | 0.064
             3  | 0.317       | 0.043     | 0.941*      | 0.074
             2  | 0.385       | 0.038     | 1.273*      | 0.084
             1  | 0.155       | 0.008     | 0.073       | 0.004
    ------------+-------------+-----------+-------------+-----------

    <Worst case>
      - "ababababab..."
      - "abcabcabcabc..."
      - ...
*/
// It is easy enough to use in a coding interview.
struct SimpleSuffixArray {
    string S;

    vector<pair<int,int>> repGroup; // (start, count)
    vector<int> saToGroup;
    vector<int> sa;

    explicit SimpleSuffixArray(const string& s) {
        buildNaive(s);
    }

    // O(N*(logN)^2), practically slower than buildNaive()
    static vector<int> build(const string& s) {
        int N = int(s.length());

        vector<int> group(N + 1);
        for (int i = 0; i < N; i++)
            group[i] = s[i];
        group[N] = -1;

        vector<int> perm(N);
        for (int i = 0; i < N; i++)
            perm[i] = i;

        int t = 1;
        while (t < N) {
            auto op = [t, &group](int a, int b) {
                if (group[a] != group[b])
                    return group[a] < group[b];
                return group[a + t] < group[b + t];
            };
            sort(perm.begin(), perm.end(), op);

            t <<= 1;
            if (t >= N)
                break;

            vector<int> g2(N + 1);
            g2[N] = -1;
            g2[perm[0]] = 0;
            for (int i = 1; i < int(perm.size()); i++) {
                if (op(perm[i - 1], perm[i]))
                    g2[perm[i]] = g2[perm[i - 1]] + 1;
                else
                    g2[perm[i]] = g2[perm[i - 1]];
            }
            group = g2;
        }

        return perm;
    }

    //---

    // O(N^2*logN), but fast enough in normal situation
    void buildNaive(const string& s) {
        S = s;
        int N = int(s.length());

        // build repeat groups
        repGroup.clear();
        repGroup.reserve(N);
        saToGroup.resize(N);

        saToGroup[0] = 0;
        repGroup.emplace_back(0, 1);
        for (int i = 1; i < N; i++) {
            if (S[i - 1] == S[i]) {
                saToGroup[i] = saToGroup[i - 1];
                repGroup.back().second++;
            } else {
                saToGroup[i] = int(repGroup.size());
                repGroup.emplace_back(i, 1);
            }
        }

        // sort suffix array
        sa.resize(N);
        iota(sa.begin(), sa.end(), 0);
        sort(sa.begin(), sa.end(), [N,this](int a, int b) {
            int ga = saToGroup[a];
            int gb = saToGroup[b];
            if (ga == gb) {
                int d = repGroup[ga].first + repGroup[ga].second - max(a, b);
                a += d;
                b += d;
            }
            for (; a < N && b < N; a++, b++) {
                if (S[a] < S[b])
                    return true;
                else if (S[a] > S[b])
                    return false;
            }
            return b < N;
        });
    }

    int lcp(int x, int y) const {
        int N = int(S.length());

        int res = 0;

        int ga = saToGroup[x];
        int gb = saToGroup[y];
        if (ga == gb) {
            int d = repGroup[ga].first + repGroup[ga].second - max(x, y);
            x += d;
            y += d;
            res += d;
        }

        for (; x < N && y < N; x++, y++) {
            if (S[x] != S[y])
                break;
            res++;
        }

        return res;
    }

    //---

    // O(N^2), but fast enough in normal situation
    vector<int> buildLcpArrayNaive() const {
        int N = int(S.length());

        vector<int> res(N);

        for (int i = 1; i < N; i++)
            res[i] = lcp(sa[i - 1], sa[i]);

        return res;
    }

    // Kasai algorithm - O(N)
    vector<int> buildLcpArrayFast() const {
        int N = int(S.length());
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
