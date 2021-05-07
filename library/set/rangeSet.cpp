#include <numeric>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

#include "rangeSet.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <queue>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.codechef.com/problems/DENSEGRP

template <typename T, T INF = 0x3f3f3f3f>
struct DenseGraph {
    // shortest path from src to dst
    //  - edges : from[u] -> to[u]
    //  - src : source
    //  - dst : destination
    // return shortest distance from src to dst, -1 if there is no path
    // O(N*logN)
    static T shortestPath(const vector<pair<T, T>>& from, const vector<pair<T, T>>& to, T src, T dst) {
        if (src == dst)
            return 0;

        int M = int(from.size());

        int termCount = 0;
        vector<bool> termF(M);
        vector<bool> vis(M);
        queue<pair<T, int>> Q;  // (distance, index)

        for (int i = 0; i < M; i++) {
            if (to[i].first <= dst && dst <= to[i].second) {
                termCount++;
                termF[i] = true;
            }
            if (from[i].first <= src && src <= from[i].second) {
                Q.emplace(1, i);
                vis[i] = true;
                if (termF[i])
                    return 1;
            }
        }
        if (Q.empty() || termCount <= 0)
            return -1;

        RangeSet<T, INF> rs;
        rs.build(from);

        while (!Q.empty()) {
            T w = Q.front().first;
            int u = Q.front().second;
            Q.pop();

            int L = to[u].first;
            int R = to[u].second;

            vector<int> selected;
            rs.popOverlapped(L, R, selected);
            for (auto v : selected) {
                if (!vis[v]) {
                    Q.emplace(w + 1, v);
                    vis[v] = true;

                    if (termF[v])
                        return w + 1;
                }
            }
        }

        return -1;
    }
};

void testRangeSet() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Range Set ------------------------" << endl;
    {
        vector<pair<int, int>> from{ { 1, 2 }, { 5, 5 } };
        vector<pair<int, int>> to{ { 4, 5 }, { 2, 3 } };
        int ans = DenseGraph<int>::shortestPath(from, to, 1, 2);
        int gt = 2;
        if (ans != gt)
            cout << "Mismatched : " << ans << ", " << gt;
        assert(ans == gt);
    }
    {
        vector<pair<int, int>> from{ { 1, 6 } };
        vector<pair<int, int>> to{ { 3, 3 } };
        int ans = DenseGraph<int>::shortestPath(from, to, 3, 4);
        int gt = -1;
        if (ans != gt)
            cout << "Mismatched : " << ans << ", " << gt;
        assert(ans == gt);
    }
    {
        vector<pair<int, int>> from{ { 1, 1 } };
        vector<pair<int, int>> to{ { 1, 1 } };
        int ans = DenseGraph<int>::shortestPath(from, to, 2, 2);
        int gt = 0;
        if (ans != gt)
            cout << "Mismatched : " << ans << ", " << gt;
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
