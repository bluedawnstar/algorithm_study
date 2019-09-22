#pragma once

// https://leetcode.com/problems/minimum-time-to-build-cost/

class BlockBuilder {
public:
    // Huffman's algorithm, O(N*logN)
    static int minBuildTime(vector<int>& cost, int splitCost) {
        priority_queue<int> Q;
        for (int c : cost)
            Q.push(-c);

        while (Q.size() > 1) {
            int a = -Q.top(); Q.pop();
            int b = -Q.top(); Q.pop();
            Q.push(-(splitCost + b));
        }
        return -Q.top();
    }

    // Binary search, O(N*logMAXCOST) 
    static bool possible(vector<pair<int,int>>& C, int N, int splitCost, int maxCost) {
        int workerN = 1;
        for (int i = 0; i < int(C.size()); i++) {
            int maxCnt = (maxCost - C[i].first) / splitCost;
            for (int j = 0; j < maxCnt && workerN < N; j++) {
                workerN <<= 1;
                maxCost -= splitCost;
            }

            if (C[i].first > maxCost || workerN < C[i].second)
                return false;
            else if (workerN >= N)
                break;

            N -= C[i].second;
            workerN -= C[i].second;
        }

        return true;
    }

    static int minBuildTimeWithBinarySearch(vector<int>& cost, int splitCost) {
        int N = int(cost.size());

        sort(cost.begin(), cost.end(), greater<int>());
        vector<pair<int, int>> C;   // (cost, count)
        C.emplace_back(cost[0], 1);
        for (int i = 1; i < N; i++) {
            if (C.back().first == cost[i])
                ++C.back().second;
            else
                C.emplace_back(cost[i], 1);
        }

        int lo = cost[0], hi = cost[0] + N * splitCost;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (possible(C, N, splitCost, mid))
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        return lo;
    }
};
