#pragma once

#include "prefixFunctionTree.h"

/*
  The number of substrings in S that can be made with prefix(non-empty) + suffix(non-empty) of P.

  https://www.codechef.com/problems/SUBPROB
  https://www.codechef.com/submit/SUBPROB
*/
struct PrefixSuffixSubstringCounter {
    int N;          // length of P
    string P;

    int M;          // length of S
    string S;

    PrefixFunctionTree forward;
    PrefixFunctionTree backward;

    // O(N*logN)
    void build(const string& prefixSuffixPattern, const string& s) {
        N = int(prefixSuffixPattern.length());
        M = int(s.length());

        P = prefixSuffixPattern;
        S = s;

        auto revP = prefixSuffixPattern;
        auto revS = s;
        reverse(revP.begin(), revP.end());
        reverse(revS.begin(), revS.end());

        forward.build(prefixSuffixPattern, s);
        backward.build(revP, revS);
    }

    // The number of substrings in S that can be made with prefix(non-empty) + suffix(non-empty) of P.
    // O(N*(logN)^2)
    long long count() {
        vector<long long> dp(N + 1);
        vector<set<pair<int, int>>> forwardToBackward(N + 1);  // [forwardU] = { (backward.visTime[backwardU], backwardU), ... }

        for (int i = 0; i + 1 < M; i++) {
            int u = N + 1 + i;
            int v = N + M - 1 - i;

            int pu = forward.P[0][u];
            int pv = backward.P[0][v];

            if (pu > 0 && pv > 0)
                dp[pu] += update(forwardToBackward[pu], pv);
        }

        // sort by level(depth) of forward
        vector<pair<int, int>> orderF;
        orderF.reserve(N);
        for (int i = 1; i <= N; i++)
            orderF.emplace_back(forward.level[i], i);
        sort(orderF.begin(), orderF.end(), greater<pair<int, int>>());

        long long ans = 0;
        for (auto& it : orderF) {
            int u = it.second;
            int pu = forward.P[0][u];

            // update answer
            ans += dp[u];
            if (forwardToBackward[u].empty())
                continue;

            // merge u to parent of u with small-to-large technique
            if (forwardToBackward[pu].size() < forwardToBackward[u].size()) {
                swap(dp[pu], dp[u]);
                swap(forwardToBackward[pu], forwardToBackward[u]);
            }
            for (auto& it : forwardToBackward[u])
                dp[pu] += update(forwardToBackward[pu], it.second);
        }

        return ans;
    }

private:
    int update(set<pair<int, int>>& backS, int v) {
        int visTimeV = backward.visTime[v].first;

        // find nearest lca between v and previous nodes
        int lca = -1;

        auto it = backS.lower_bound({ visTimeV, 0 });
        if (it != backS.end())
            lca = backward.findLCA(it->second, v);

        if (it != backS.begin()) {
            --it;

            int tmp = backward.findLCA(it->second, v);
            if (lca == -1 || backward.level[tmp] > backward.level[lca])
                lca = tmp;
        }

        // update
        backS.emplace(visTimeV, v);

        // distance from v to paths of previous nodes
        if (lca == -1)
            return backward.level[v];
        else
            return backward.level[v] - backward.level[lca];
    }
};
