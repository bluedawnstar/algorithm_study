#pragma once

#include "../rangeQuery/dynamicSegmentTreeForest.h"
#include "../set/unionFindWithValues.h"
#include "suffixArray.h"

#include "../rangeQuery/fenwickTreeMod.h"
#include "suffixAutomaton.h"

/*
  https://www.codechef.com/problems/EXPREP
  https://discuss.codechef.com/t/exprep-editorial/70844

  Find the sum of WEIGHT of all substrings of a given string S.
  WEIGHT is defined as the sum of weights of all its periodic substrings.

  "abab", W={1,2,...}
    - "a"    : W(a) = 1
    - "ab"   : W(ab) = 3
    - "aba"  : W(aba) + W(ab) = 7
    - "abab" : W(abab) + (ab) = 9
    -  "b"   : W(b) = 2
    -  "ba"  : W(ba) = 3
    -  "bab" : W(bab) + W(ba) = 8
    -   "a"  : W(a) = 1
    -   "ab" : W(ab) = 3
    -    "b" : W(b) = 2
    => 39

    1) Let Pref[i] denote weight of prefix of S till i.
       For each distinct substring T of S, let endpos(T) = { i_1, i_2, ..., i_k }.
       The contribution of TT to the total answer would be

           k    k
          SUM  SUM   W(S[i_p : i_q])
          p=1  q=p+1

           k    k
        = SUM  SUM  (Pref[i_q] - Pref[i_p])
          p=1  q=p+1

           k                         k
        = SUM Pref[i_q] * (q - 1) - SUM Pref[i_p] * (k - p)
          q=1                       p=1

        -----

           k
        = SUM { Pref[i_p] * (p - 1) - Pref[i_p] * (k - p) }
          p=1

           k
        = SUM Pref[i_p] * { (p - 1) - (k - p) }
          p=1

           k
        = SUM Pref[i_p] * { 2 * p - 1 - k }
          p=1
*/
template <int mod = 1'000'000'007>
struct PeriodicSubstringWeightSumOfAllSubstrings {
    /*
        s : given string
        W : weights of alphabets
    */
    // O(N*(logN)^2)
    static int solve(const string& s, const vector<int>& W) {
        // (the number of suffixes in the range, the sum of prefix sums in the range)
        DynamicSegmentTreeForest<pair<int, int>> segTrees([](const pair<int, int>& a, const pair<int, int>& b) {
            pair<int, int> res(a.first + b.first, a.second + b.second);
            if (res.first >= mod)
                res.first -= mod;
            if (res.second >= mod)
                res.second -= mod;
            return res;
        }, make_pair(0, 0));
        UnionFindWithValues<int> dsu;   // [suffix array index] = { positions, }

        int N = int(s.length());

        SuffixArray<> sa(s);
        vector<int> prefixSum(N + 1);

        long long res = 0;

        for (int i = 0; i < N; i++) {
            int ch = s[i] - 'a';

            // weight sum of all substring itselves
            long long cnt = 1ll * (i + 1) * (N - i) % mod;
            res = (res + W[ch] * cnt) % mod;

            prefixSum[i + 1] = prefixSum[i] + W[ch];
            if (prefixSum[i + 1] >= mod)
                prefixSum[i + 1] -= mod;
        }

        dsu.init(N);
        segTrees.init(N);
        vector<int> segTreeRoots(N, -1);

        for (int i = 0; i < N; i++) {
            dsu.values[i].push_back(sa.suffixArray[i]);
            segTreeRoots[i] = segTrees.update(segTreeRoots[i], sa.suffixArray[i], make_pair(1, prefixSum[sa.suffixArray[i]]));
        }

        vector<pair<int, int>> events;
        for (int i = 0; i + 1 < N; i++)
            events.emplace_back(sa.lcpArray[i + 1], i);
        sort(events.begin(), events.end(), greater<pair<int, int>>());

        long long cur = 0;
        int pt = 0;

        /*
            1) LCP가 큰 것부터 작은 것 순서로 계산
            2) 이전 LCP 길이에서 계산한 weight-sum에 변화가 생기는 것들만 더해서 각 단계별 weight-sum을 만듦.
               그 이유는 LCP가 n인 것들의 weight-sum은 n보다 큰 것들의 weight-sum을 포함하기 때문이다.
        */
        for (int lcpLen = N; lcpLen >= 1; lcpLen--) {
            while (pt < int(events.size()) && events[pt].first == lcpLen) {
#if 0
                dsu.merge(events[pt].second, events[pt].second + 1, [&](int v, int u) {
                    for (int j : dsu.values[u]) {
                        auto p1 = segTrees.query(segTreeRoots[v], j + 1, N - 1);
                        auto p2 = segTrees.query(segTreeRoots[v], 0, N - 1);

                        /*
                                  (part 1)                  (part 2)
                             k                         k
                            SUM Pref[i_q] * (q - 1) - SUM Pref[i_p] * (k - p)
                            q=1                       p=1

                            1) + j가 추가되면서 v의 뒤쪽에 있는 것들의 위치가 1증가하면서 Pref 합이 증가하는 양 (part 1)
                               - j의 뒤쪽에 숫자가 추가되면서 Pref[j] 합이 감소하는 양 (part 2)
                        */
                        cur = (cur + p1.second - 1ll * p1.first * prefixSum[j]) % mod;
                        if (cur < 0)
                            cur += mod;

                        p2.first -= p1.first;
                        p2.second -= p1.second;
                        if (p2.second < 0)
                            p2.second += mod;

                        /*
                            2) + j가 추가되면서 j의 위치 조정으로 인해 Pref[j]합이 증가하는 양 (part 1)
                               - j가 추가되면서 v의 앞쪽에 있는 것들의 뒤에서 부터의 위치가 1증가하면서 Pref 합이 감소하는 양 (part 2)
                        */
                        cur = (cur + 1ll * p2.first * prefixSum[j] - p2.second) % mod;
                        if (cur < 0)
                            cur += mod;
                    }
                    segTreeRoots[v] = segTrees.merge(segTreeRoots[v], segTreeRoots[u]);
                });
#else
                dsu.merge(events[pt].second, events[pt].second + 1, [&](int v, int u) {
                    for (int j : dsu.values[u]) {
                        auto p1 = segTrees.query(segTreeRoots[v], j + 1, N - 1);
                        auto p2 = segTrees.query(segTreeRoots[v], 0, N - 1);

                        /*
                             k
                            SUM Pref[i_p] * { 2 * p - 1 - k }
                            p=1

                            1) + j가 추가되면서 v의 뒤쪽에 있는 것들의 위치가 1증가하면서 Pref 합이 증가하는 양 (위 식에서 2 * p 부분)
                               - j가 추가되면사 k가 증가함에 따라 v 전체의 Pref 합이 감소하는 양 (위 식에서 -k 부분)
                        */
                        cur = (cur + p1.second * 2 - p2.second) % mod;
                        if (cur < 0)
                            cur += mod;

                        /*
                             k
                            SUM Pref[i_p] * { 2 * p - 1 - k }
                            p=1

                            2) + j가 추가되면서 j의 위치가 증가함에 따라 증가하는 Pref[j]의 합 (위 식에서 2 * p 부분)
                               - j가 추가되면사 j 입장에서 전체 개수가 증가하는 양에 따른 Pref 합이 감소하는 양 (위 식에서 -k 부분)

                                => delta_p * 2 - delta_k
                               <=> (p2.first - p1.first) * 2 - p2.first
                               <=> p2.first - p1.first * 2
                        */
                        cur = (cur + 1ll * (p2.first - p1.first * 2) * prefixSum[j]) % mod;
                        if (cur < 0)
                            cur += mod;
                    }
                    segTreeRoots[v] = segTrees.merge(segTreeRoots[v], segTreeRoots[u]);
                });
#endif
                pt++;
            }
            res += cur;
            if (res >= mod)
                res -= mod;
        }

        return int(res);
    }

    /*
        s : given string
        W : weights of alphabets
    */
    // O(N*(logN)^2)
    static int solveWithSuffixAutomaton(const string& s, const vector<int>& W) {
        int N = int(s.length());

        FenwickTreeMod<int, mod> bit1(N + 1), bit2(N + 1);
        vector<int> prefixSum(N + 1);

        long long ans = 0;

        for (int i = 0; i < N; i++) {
            int ch = s[i] - 'a';

            // weight sum of all substring itselves
            long long cnt = 1ll * (i + 1) * (N - i) % mod;
            ans = (ans + W[ch] * cnt) % mod;

            prefixSum[i + 1] = prefixSum[i] + W[ch];
            if (prefixSum[i + 1] >= mod)
                prefixSum[i + 1] -= mod;
        }

        long long cur = 0;
        auto add = [&](int u, int x) {  // x in { -1, 1 }
#if 0
            /*
                        (part 1)                  (part 2)
                 k                         k
                SUM Pref[i_q] * (q - 1) - SUM Pref[i_p] * (k - p)
                q=1                       p=1

                1) + u가 추가되면서 u의 뒤쪽에 있는 것들의 위치가 1증가하면서 Pref 합이 증가하는 양 (part 1)
                   - u가 추가되면서 u관점에서 뒤쪽에 늘어나는 개수만큼 Pref[u] 합이 감소하는 양 (part 2)
            */
            long long q = (bit2.sumRange(u, N) - 1ll * bit1.sumRange(u, N) * prefixSum[u]) % mod;
            if (q < 0)
                q += mod;

            /*
                2) + u가 추가되면서 u관점에서 앞쪽에 늘어나는 개수만큼 Pref[u] 합이 증가하는 양 (part 1)
                   - u가 추가되면서 u의 뒤쪽에 있는 것들의 위치가 1증가하면서 Pref 합이 감소하는 양 (part 2)
            */
            q = (q + 1ll * bit1.sumRange(0, u - 1) * prefixSum[u] - bit2.sumRange(0, u - 1)) % mod;
            if (q < 0)
                q += mod;
#else
            /*
                        (part 1)                  (part 2)
                 k                         k
                SUM Pref[i_q] * (q - 1) - SUM Pref[i_p] * (k - p)
                q=1                       p=1

                1) + u가 추가되면서 u의 뒤쪽에 있는 것들의 위치가 1증가하면서 Pref 합이 증가하는 양 (part 1)
                   - u가 추가되면서 u의 뒤쪽에 있는 것들의 위치가 1증가하면서 Pref 합이 감소하는 양 (part 2)
            */
            long long q = bit2.sumRange(u, N) - bit2.sumRange(0, u - 1);
            if (q < 0)
                q += mod;

            /*
                2) + u가 추가되면서 u관점에서 앞쪽에 늘어나는 개수만큼 Pref[u] 합이 증가하는 양 (part 1)
                   - u가 추가되면서 u관점에서 뒤쪽에 늘어나는 개수만큼 Pref[u] 합이 감소하는 양 (part 2)
            */
            q = (q + 1ll * (bit1.sumRange(0, u - 1) - 1ll * bit1.sumRange(u, N)) * prefixSum[u]) % mod;
            if (q < 0)
                q += mod;
#endif

            cur = (cur + x * q) % mod;

            // update the BITs
            bit1.add(u, x);
            bit2.add(u, int(1ll * x * prefixSum[u] % mod));
        };

        //---

        SuffixAutomaton<> sa(N);
        sa.extend(s);

        // build tree
        vector<vector<int>> children(sa.N);
        vector<int> size(sa.N);
        int currTime = 0;
        vector<int> timeToNode(sa.N);
        vector<pair<int, int>> visitTime(sa.N);

        for (int u = 1; u < sa.N; u++) {
            children[sa.state[u].suffixLink].push_back(u);
        }

        function<void(int)> dfs1;
        dfs1 = [&](int u) {
            size[u] = 1;
            timeToNode[currTime] = u;
            visitTime[u].first = currTime++;
            for (int v : children[u]) {
                dfs1(v);
                size[u] += size[v];
            }
            visitTime[u].second = currTime;
        };
        dfs1(0);

        // calculate
        function<void(int, bool)> dfs2;
        dfs2 = [&](int u, bool keep) { // dsu on tree trick
            int bigSize = 0, bigChild = -1;
            for (auto v : children[u]) {
                if (bigSize < size[v]) {
                    bigChild = v;
                    bigSize = size[v];
                }
            }
            for (auto v : children[u]) {
                if (v != bigChild)
                    dfs2(v, false);
            }
            if (bigChild != -1)
                dfs2(bigChild, true);

            for (auto v : children[u]) {
                if (v != bigChild) {
                    for (int t = visitTime[v].first; t < visitTime[v].second; t++) {
                        if (!sa.state[timeToNode[t]].cloned) {
                            add(sa.state[timeToNode[t]].rightEnd + 1, 1);
                        }
                    }
                }
            }

            if (u) {
                if (!sa.state[u].cloned)
                    add(sa.state[u].rightEnd + 1, 1);

                ans = (ans + (sa.state[u].len - sa.state[sa.state[u].suffixLink].len) * cur) % mod;
                if (ans < 0)
                    ans += mod;
            }

            if (!keep) {
                for (int t = visitTime[u].first; t < visitTime[u].second; t++) {
                    if (!sa.state[timeToNode[t]].cloned)
                        add(sa.state[timeToNode[t]].rightEnd + 1, mod - 1);
                }
            }
        };
        dfs2(0, false);

        return int(ans);
    }


    //---

    // O(N^2)
    static int solveNaive(const string& s, const vector<int>& W) {
        int N = int(s.length());

        long long res = 0;

        vector<int> prefixSum(N + 1);
        for (int i = 0; i < N; i++) {
            int ch = s[i] - 'a';

            long long cnt = 1ll * (i + 1) * (N - i) % mod;
            res = (res + W[ch] * cnt) % mod;

            prefixSum[i + 1] = prefixSum[i] + W[ch];
            if (prefixSum[i + 1] >= mod)
                prefixSum[i + 1] -= mod;
        }

        for (int i = 0; i < N; i++) {
            int suffixLen = N - i;

            //--- prefix function

            vector<int> dp(suffixLen + 1, -1);
            vector<int> p(suffixLen);
            int k = 0;
            for (int j = 1; j < suffixLen; j++) {
                while (k > 0 && s[k + i] != s[j + i])
                    k = p[k - 1];

                if (s[k + i] == s[j + i])
                    k++;

                p[j] = k;

                // process periodic substrings
                if (0 < k && k <= j) {
                    int repLen = j + 1 - k;
                    long long w = prefixSum[i + repLen] - prefixSum[i];

                    for (int t = p[k - 1]; t > 0; t = p[t - 1]) {
                        int repLen = j + 1 - t;
                        if (dp[repLen] >= 0) {
                            w += dp[repLen];
                            break;
                        }
                        w += prefixSum[i + repLen] - prefixSum[i];
                    }

                    w %= mod;
                    if (w < 0)
                        w += mod;

                    dp[repLen] = int(w);
                    res += int(w);
                    if (res >= mod)
                        res -= mod;
                }
            }
        }

        return int(res);
    }
};
