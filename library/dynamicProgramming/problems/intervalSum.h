#pragma once

/*
  https://www.codechef.com/problems/BOOLGAME

  1. input
     N = the number of variables
     K = top K
     values = { (L0, R0, d0), (L1, R1, d1), ... }

  2. output
                                     
       F(x[0],x[1],x[2],...,x[n-1]) =  SUM d[i]
                                       i=0
     - top-K values of F()

     - ex) 
        N = 4, K=3
        values = {
            (0, 0, -4),
            (0, 2,  0),
            (0, 3, -3),
            (1, 1, -2),
            (2, 2,  5),
            (3, 3,  2)
        }

        - 1st : x[0..3] = 0011 -> 7 (5 + 2)
        - 2nd : x[0..3] = 0010 -> 5 (5)
        - 3rd : x[0..3] = 0111 -> 5 (-2 + 5 + 2)
*/
struct IntervalSum {
    // values = { (L, R, score), ... }
    // O(N*(N+M)*K)
    static vector<long long> solveDP(int N, int K, const vector<tuple<int, int, int>>& values) {
        vector<vector<pair<int, int>>> cost(N); // [R] = { (L0, cost0), (L1, cost1), ... }
        for (auto& it : values)
            cost[get<1>(it)].emplace_back(get<0>(it), get<2>(it));

        for (int i = 0; i < N; i++)
            sort(cost[i].begin(), cost[i].end());

        vector<vector<vector<long long>>> dp(N + 1, vector<vector<long long>>(N + 1));
        dp[0][0].push_back(0ll);
        for (int i = 1; i <= N; i++) {
            long long add = 0;
            for (auto& r : cost[i - 1])
                add += r.second;

            for (int j = 0, k = 0; j < i; j++) {
                while (k < int(cost[i - 1].size()) && cost[i - 1][k].first < j) {
                    add -= cost[i - 1][k].second;
                    k++;
                }
                for (auto r : dp[j][i - 1]) {
                    dp[j][i].push_back(add + r);    // x[i] == 1
                    dp[i][i].push_back(r);          // x[i] == 0
                }
            }
            sort(dp[i][i].begin(), dp[i][i].end(), greater<long long>());
            if (int(dp[i][i].size()) > K)
                dp[i][i].resize(K);
        }

        vector<long long> res;
        for (int i = 0; i <= N; i++) {
            for (auto r : dp[i][N]) {
                res.push_back(r);
            }
        }

        sort(res.begin(), res.end(), greater<long long>());
        if (int(res.size()) > K)
            res.resize(K);

        return res;
    }

    // values = { (L, R, score), ... }
    static vector<long long> solve(int N, int K, const vector<tuple<int, int, int>>& values) {
        vector<vector<pair<long long, int>>> cost(N);   // [R] = { (bits, cost0), (bits, cost1), ... }
        for (auto& it : values) {
            int L = get<0>(it);
            int R = get<1>(it);
            cost[get<1>(it)].emplace_back(((1ll << (R + 1)) - 1) - ((1ll << L) - 1), get<2>(it));
        }

        for (int i = 0; i < N; i++)
            sort(cost[i].begin(), cost[i].end());

        vector<pair<long long, long long>> vec;
        vec.emplace_back(0ll, 0ll);
        for (int i = 0; i < N; i++) {
            vector<pair<long long, long long>> vec2 = vec;  // x[i] == 0
            if (int(vec2.size()) > K)
                vec2.resize(K);

            for (auto& it1 : vec) {
                long long score = it1.first;
                long long comb = it1.second | (1ll << i);
                for (auto it2 : cost[i]) {
                    if ((comb & it2.first) != it2.first)
                        break;
                    score += it2.second;
                }
                vec2.emplace_back(score, comb);             // x[i] == 1
            }
            sort(vec2.begin(), vec2.end(), greater<pair<long long, long long>>());
            swap(vec, vec2);
        }

        vector<long long> res;
        for (int i = 0; i < K && i < int(vec.size()); i++)
            res.push_back(vec[i].first);

        return res;
    }
};
