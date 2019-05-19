#pragma once

// https://leetcode.com/problems/longest-duplicate-substring/

struct LongestDuplicatedSubstring {
    static const int P1 = 31;
    static const int P2 = 41;

    // O(NlogN), binary search + hashing
    static string solve(const string& S) {
        int N = int(S.length());

        // power table
        vector<unsigned long long> pow1(N), pow2(N);
        pow1[0] = P1;
        pow2[0] = P2;
        for (int i = 1; i < N; i++) {
            pow1[i] = pow1[i - 1] * P1;
            pow2[i] = pow2[i - 1] * P2;
        }

        // hashing
        vector<unsigned long long> hash1(N + 1), hash2(N + 1);
        for (int i = 0; i < N; i++) {
            hash1[i + 1] = hash1[i] + (S[i] - 'a' + 1) * pow1[i];
            hash2[i + 1] = hash2[i] + (S[i] - 'a' + 1) * pow2[i];
        }

        auto isPossible = [N, &pow1, &pow2, &hash1, &hash2](int length) {
            unordered_set<unsigned long long> S;
            for (int i = length; i <= N; i++) {
                int j = i - length;
                auto h1 = (hash1[i] - hash1[j]) * pow1[N - 1 - j];
                auto h2 = (hash2[i] - hash2[j]) * pow2[N - 1 - j];
                if (S.find((h1 << 1) ^ h2) != S.end())
                    return make_pair(j, length);
                S.insert((h1 << 1) ^ h2);
            }
            return make_pair(-1, 0);
        };

        int start = 0;
        int length = 0;
        {
            int lo = 1, hi = N;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;

                auto r = isPossible(mid);
                if (r.second > 0) {
                    if (length < r.second) {
                        start = r.first;
                        length = r.second;
                    }
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
        }

        return S.substr(start, length);
    }

    //--- with Suffix Array

    struct SimpleSuffixArray {
        string S;

        vector<pair<int,int>> repGroup; // (start, count)
        vector<int> saToGroup;
        vector<int> sa;

        explicit SimpleSuffixArray(const string& s) {
            build(s);
        }

        void build(const string& s) {
            S = s;
            int N = int(s.length());

            repGroup.clear();
            saToGroup.assign(N, 0);
            sa.assign(N, 0);

            // build repeat groups
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
            iota(sa.begin(), sa.end(), 0);
            sort(sa.begin(), sa.end(), [N,&s,this](int a, int b) {
                int ga = saToGroup[a];
                int gb = saToGroup[b];
                if (ga == gb) {
                    int i = repGroup[ga].first;
                    int j = i + repGroup[ga].second;
                    int d = j - max(a, b);
                    a += d;
                    b += d;
                }
                for (; a < N && b < N; a++, b++) {
                    if (s[a] < s[b])
                        return true;
                    else if (s[a] > s[b])
                        return false;
                }
                return b < N;
            });
        }

        int lcp(int sa1, int sa2) {
            int N = int(S.length());

            int res = 0;

            int ga = saToGroup[sa1];
            int gb = saToGroup[sa2];
            if (ga == gb) {
                int i = repGroup[ga].first;
                int j = i + repGroup[ga].second;
                int d = j - max(sa1, sa2);
                sa1 += d;
                sa2 += d;
                res += d;
            }

            for (; sa1 < N && sa2 < N; sa1++, sa2++) {
                if (S[sa1] != S[sa2])
                    break;
                res++;
            }

            return res;
        }
    };

    // O(N^2) or O(NlogN)
    static string solveWithSA(const string& S) {
        SimpleSuffixArray sa(S);

        int N = int(S.length());

        int start = 0;
        int length = 0;
        for (int i = 1; i < N; i++) {
            int len = sa.lcp(sa.sa[i - 1], sa.sa[i]);
            if (length < len) {
                start = sa.sa[i];
                length = len;
            }
        }

        return S.substr(start, length);
    }
};
