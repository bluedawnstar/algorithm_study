#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreeLazyWithBase.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

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

    vector<vector<pair<int,int>>> qry(N); // qry[R] = { (L, query-index), ... }
    for (int i = 0; i < Q; i++)
        qry[queries[i].second].emplace_back(queries[i].first, i);

    SegmentTreeLazyWithBase<long long> solver(N, [](long long a, long long b) {
        return min(a, b);
    }, 0x3f3f3f3f3f3f3f3fll);

    long long edgeSum = 0;
    solver.setBase(0, edgeSum + vertexWeight[0]);
    for (int R = 1; R < N; R++) {
        edgeSum += edgeWeight[R - 1];
        solver.update(0, R - 1, vertexWeight[R] - edgeSum);

        for (auto& it : qry[R]) {
            int L = it.first;
            int idx = it.second;
            res[idx] = solver.query(L, R - 1);
        }

        solver.setBase(R, edgeSum + vertexWeight[R]);
    }

    return res;
}

void testSegmentTreeLazyWithBase() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Segment Tree with Lazy Propagation and Base Value -------------------------------------" << endl;
    {
        vector<int> vertexWeight{ 29, 25, 49, 9 };
        vector<int> edgeWeight{ 42, 11, 11 };

        vector<pair<int, int>> queries{ { 0, 2 }, { 0, 3 }  };

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

            vector<pair<int,int>> queries;
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
