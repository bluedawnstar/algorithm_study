#pragma once

/*
    <Speed test result>

    N = 10^6

    ------------+-------------------------+-------------------------+----------------------
                |      Fast solution      |     Simple solution     |  Simple solution #2
       Used     +-------------+-----------+-------------+-----------+----------------------
     characters | SA building | LCP array | SA building | LCP array | SA building
    ------------+-------------+-----------+-------------+-----------+----------------------
            26  | 0.322       | 0.031     | 0.438       | 0.026     | 0.495
            25  | 0.313       | 0.032     | 0.436       | 0.027     | 0.492
            24  | 0.304       | 0.031     | 0.437       | 0.029     | 0.500
            23  | 0.302       | 0.031     | 0.439       | 0.026     | 0.493
            22  | 0.295       | 0.032     | 0.437       | 0.025     | 0.488
            21  | 0.284       | 0.031     | 0.448       | 0.026     | 0.490
            20  | 0.282       | 0.031     | 0.445       | 0.026     | 0.490
            19  | 0.283       | 0.031     | 0.455       | 0.027     | 0.487
            18  | 0.274       | 0.032     | 0.455       | 0.027     | 0.479
            17  | 0.271       | 0.031     | 0.459       | 0.028     | 0.480
            16  | 0.266       | 0.033     | 0.467       | 0.028     | 0.488
            15  | 0.267       | 0.033     | 0.472       | 0.029     | 0.490
            14  | 0.268       | 0.033     | 0.480       | 0.033     | 0.471
            13  | 0.259       | 0.033     | 0.488       | 0.030     | 0.470
            12  | 0.255       | 0.032     | 0.497       | 0.030     | 0.479
            11  | 0.252       | 0.031     | 0.504 @     | 0.031     | 0.474
            10  | 0.249       | 0.033     | 0.517 @     | 0.031     | 0.466
             9  | 0.247       | 0.033     | 0.529 @     | 0.033     | 0.471
             8  | 0.242       | 0.031     | 0.546 @     | 0.035     | 0.471
             7  | 0.235       | 0.033     | 0.570 @     | 0.041     | 0.472
             6  | 0.225       | 0.034     | 0.598 @     | 0.048     | 0.484
             5  | 0.330       | 0.033     | 0.640 #     | 0.044     | 0.485
             4  | 0.282       | 0.034     | 0.701 #     | 0.050     | 0.466
             3  | 0.255       | 0.033     | 0.830 #     | 0.052     | 0.466
             2  | 0.294       | 0.033     | 1.120 *     | 0.065     | 0.464
             1  | 0.143       | 0.005     | 0.066       | 0.003     | 0.147
    ------------+-------------+-----------+-------------+-----------+----------------------

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

    SimpleSuffixArray() {
    }

    explicit SimpleSuffixArray(const string& s) {
        buildNaive(s);
    }

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

struct SimpleSuffixArray2 {
    // O(N*(logN)^2)
    static vector<int> build(const string& s) {
        int N = int(s.length());

        vector<int> group(N + 1);
        for (int i = 0; i < N; i++)
            group[i] = s[i];
        group[N] = -1;

        vector<int> SA(N);
        iota(SA.begin(), SA.end(), 0);

        int t = 1;
        while (t < N) {
            if (t == 1) {
                sort(SA.begin(), SA.end(), [t, &group](int a, int b) {
                    if (group[a] != group[b])
                        return group[a] < group[b];
                    return group[a + t] < group[b + t];
                });
            } else {
                auto op = [t, &group](int a, int b) {
                    return group[a + t] < group[b + t];
                };
                int start = 0;
                for (int i = 1; i < N; i++) {
                    if (group[SA[i - 1]] != group[SA[i]]) {
                        if (start + 1 < i)
                            sort(SA.begin() + start, SA.begin() + i, op);
                        start = i;
                    }
                }
                sort(SA.begin() + start, SA.end(), op);
            }

            if ((t << 1) >= N)
                break;

            vector<int> g2(N + 1);
            g2[N] = -1;
            g2[SA[0]] = 0;
            for (int i = 1; i < N; i++) {
                if ((group[SA[i - 1]] != group[SA[i]]) ? (group[SA[i - 1]    ] < group[SA[i]    ])
                                                       : (group[SA[i - 1] + t] < group[SA[i] + t]))
                    g2[SA[i]] = g2[SA[i - 1]] + 1;
                else
                    g2[SA[i]] = g2[SA[i - 1]];
            }

            swap(group, g2);
            t <<= 1;
        }

        return SA;
    }
};
