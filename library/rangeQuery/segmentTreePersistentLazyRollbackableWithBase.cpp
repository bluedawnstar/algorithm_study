#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePersistentLazyRollbackableWithBase.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerearth.com/practice/algorithms/graphs/shortest-path-algorithms/practice-problems/algorithm/two-trees-ee18cad2/
/*
    queries = { (u, v), ... }
*/
static vector<long long> solveProblem(const vector<int>& W, const vector<tuple<int, int, int>>& edges,
        const vector<pair<int, int>>& queries) {
    int N = int(W.size());
    vector<vector<pair<int, int>>> E(N);
    for (auto& it : edges) {
        E[get<0>(it)].emplace_back(get<1>(it), get<2>(it));
        E[get<1>(it)].emplace_back(get<0>(it), get<2>(it));
    }

    int maxLevel = 0;
    vector<int> level(N);
    vector<long long> S1(N + 1);
    vector<long long> S2(N + 1);
    {
        function<void(int, int)> dfs;
        dfs = [&](int u, int parent) {
            maxLevel = max(maxLevel, level[u]);
            for (auto& it : E[u]) {
                if (it.first == parent)
                    continue;

                int v = it.first;
                int w = it.second;

                S1[v] = S1[u] + w;
                S2[v] = S2[u] + w * 2;
                level[v] = level[u] + 1;
                dfs(v, u);
            }
        };
        dfs(0, -1);
    }


    RollbackablePersistentSegmentTreeLazyWithBase<long long> segTree(
        [](long long a, long long b) { return min(a, b); },
        0x3f3f3f3f3f3f3f3fll
    );
    segTree.build(maxLevel + 1);

    int Q = int(queries.size());
    vector<vector<pair<int, int>>> queriesByNode(N);
    for (int i = 0; i < Q; i++)
        queriesByNode[queries[i].second].emplace_back(queries[i].first, i);

    vector<long long> res(Q);
    {
        /*
          minimize
            W[u] - (S[v] - S[u]) + W[v]
          = W[u] - S[v] + S[u] + W[v]
          = S[u] + W[u] + (W[v] - S[v])
        */
        function<void(int, int, int)> dfsSolve;
        dfsSolve = [&](int root, int u, int parent) {
            auto chk = segTree.checkPoint();

            root = segTree.setBase(root, level[u], S1[u] + W[u]);

            if (parent >= 0) {
                long long x = W[u] - S1[u];
                root = segTree.update(root, 0, level[parent], x);

                if (!queriesByNode[u].empty()) {
                    auto chk2 = segTree.checkPoint();

                    int root2 = root;
                    for (auto& it : queriesByNode[u]) {
                        auto ret = segTree.query(root2, level[it.first], level[parent]);
                        root2 = ret.second;
                        auto y = min(0ll, ret.first);
                        res[it.second] = y + S2[u] - S2[it.first];
                    }

                    segTree.rollback(chk2);
                }
            }

            for (auto& it : E[u]) {
                if (it.first == parent)
                    continue;

                int v = it.first;
                int w = it.second;
                dfsSolve(root, v, u);
            }

            segTree.rollback(chk);
        };
        dfsSolve(segTree.getInitRoot(), 0, -1);
    }

    return res;
}

//-----------------------------------------------------------------------------

/*
    query(L, R) =   argmin { vertexWeight[i] + vertexWeight[j] - sum(edgeWeight[i][i+1]) }
                 L<=i<j<= R                                      k=i,i+1,...,(j-1)


    O(Q * N^2)
*/
static vector<long long> slowSolver(const vector<int>& vertexWeight, const vector<int>& edgeWeight,
    const vector<pair<int, int>>& queries) {
    int N = int(vertexWeight.size());
    vector<long long> S(N);
    for (int i = 1; i < N; i++)
        S[i] = S[i - 1] + edgeWeight[i - 1];

    vector<long long> res;
    for (auto& q : queries) {
        int L = q.first;
        int R = q.second;

        long long ans = 0x3f3f3f3f3f3f3f3fll;
        for (int j = L + 1; j <= R; j++) {
            for (int i = L; i < j; i++) {
                long long x = vertexWeight[i] + vertexWeight[j] - (S[j] - S[i]);
                ans = min(ans, x);
            }
        }

        res.push_back(ans);
    }

    return res;
}

/*
          Se(i) =   sum { edgeWeight[j][j+1] }
                j=0..i-1

    query(L, R) =   argmin { vertexWeight[i] + vertexWeight[j] - sum(edgeWeight[i][i+1]) }
                 L<=i<j<= R                                      k=i,i+1,...,(j-1)

                =   argmin { vertexWeight[i] + Se(i) + vertexWeight[j] - Se(j) }
                 L<=i<j<= R

                =   argmin { vertexWeight[i] + Se(i) + vertexWeight[j] - Se(j) }
                 L<=i<j<= R

    O(Q * N*logN)
*/
static vector<long long> fastSolver(const vector<int>& vertexWeight, const vector<int>& edgeWeight,
    const vector<pair<int, int>>& queries) {
    int N = int(vertexWeight.size());
    int Q = int(queries.size());
    vector<long long> res(Q);

    vector<vector<pair<int, int>>> qry(N); // qry[R] = { (L, query-index), ... }
    for (int i = 0; i < Q; i++)
        qry[queries[i].second].emplace_back(queries[i].first, i);

    RollbackablePersistentSegmentTreeLazyWithBase<long long> solver(N, [](long long a, long long b) {
        return min(a, b);
        }, 0x3f3f3f3f3f3f3f3fll);

    int root = solver.getInitRoot();

    long long edgeSum = 0;
    root = solver.setBase(root, 0, edgeSum + vertexWeight[0]);
    for (int R = 1; R < N; R++) {
        edgeSum += edgeWeight[R - 1];
        root = solver.update(root, 0, R - 1, vertexWeight[R] - edgeSum);

        for (auto& it : qry[R]) {
            int L = it.first;
            int idx = it.second;
            tie(res[idx], root) = solver.query(root, L, R - 1);
        }

        root = solver.setBase(root, R, edgeSum + vertexWeight[R]);
    }

    return res;
}

void testRollbackablePersistentSegmentTreeLazyWithBase() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Rollbackable Persistent Segment Tree with Lazy Propagation and Base Value -----------------" << endl;
    {
        vector<int> W{ 29, 25, 8, 49, 13, 9 };
        vector<tuple<int, int, int>> edges{
            { 0, 1, 42 },
            { 1, 3, 11 },
            { 3, 5, 11 },
            { 0, 2, 50 },
            { 2, 4, 39 }
        };
        vector<pair<int, int>> queries{
            { 2, 4 },
            { 1, 5 },
            { 0, 5 }
        };
        vector<long long> gt{
            60, 44, 102
        };

        auto ans = solveProblem(W, edges, queries);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    cout << "OK!" << endl;
    {
        vector<int> vertexWeight{ 29, 25, 49, 9 };
        vector<int> edgeWeight{ 42, 11, 11 };

        vector<pair<int, int>> queries{ { 0, 2 }, { 0, 3 } };

        auto gt = slowSolver(vertexWeight, edgeWeight, queries);
        auto ans = fastSolver(vertexWeight, edgeWeight, queries);

        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        const int T = 100;
#ifdef _DEBUG
        const int N = 300;
        const int Q = 300;
#else
        const int N = 1000;
        const int Q = 1000;
#endif
        const int MAXV = 100000;
        const int MAXE = 10000;

        for (int tt = 0; tt < T; tt++) {
            vector<int> vertexWeight(N);
            vector<int> edgeWeight(N - 1);
            for (int i = 0; i < N; i++)
                vertexWeight[i] = RandInt32::get() % MAXV + 1;
            for (int i = 1; i < N; i++)
                edgeWeight[i - 1] = RandInt32::get() % MAXE + 1;

            vector<pair<int, int>> queries;
            queries.reserve(Q);
            for (int i = 0; i < Q; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                while (R == L)
                    R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);
                queries.emplace_back(L, R);
            }

            auto gt = slowSolver(vertexWeight, edgeWeight, queries);
            auto ans = fastSolver(vertexWeight, edgeWeight, queries);

            if (ans != gt)
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        const int T = 100;
#ifdef _DEBUG
        const int N = 300;
        const int Q = 300;
#else
        const int N = 1000;
        const int Q = 1000;
#endif
        const int MAXV = 10000;
        const int MAXE = 10000;

        for (int tt = 0; tt < T; tt++) {
            vector<int> vertexWeight(N);
            vector<int> edgeWeight(N - 1);
            for (int i = 0; i < N; i++)
                vertexWeight[i] = RandInt32::get() % MAXV + 1;
            for (int i = 1; i < N; i++)
                edgeWeight[i - 1] = RandInt32::get() % MAXE + 1;

            vector<pair<int, int>> queries;
            queries.reserve(Q);
            for (int i = 0; i < Q; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                while (R == L)
                    R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);
                queries.emplace_back(L, R);
            }

            auto gt = slowSolver(vertexWeight, edgeWeight, queries);
            auto ans = fastSolver(vertexWeight, edgeWeight, queries);

            if (ans != gt)
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
}
