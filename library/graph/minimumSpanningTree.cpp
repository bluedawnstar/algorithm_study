#include <climits>
#include <limits>
#include <numeric>
#include <tuple>
#include <queue>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

#include "minimumSpanningTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static bool isEqual(vector<pair<int, int>>& v1, vector<pair<int, int>>& v2) {
    for (auto& it : v1) {
        if (it.first > it.second)
            swap(it.first, it.second);
    }
    for (auto& it : v2) {
        if (it.first > it.second)
            swap(it.first, it.second);
    }
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    return v1 == v2;
}

static vector<pair<int, int>> makeEdgeList(vector<int>& parent) {
    vector<pair<int, int>> res;
    for (int i = 0; i < int(parent.size()); i++)
        if (parent[i] >= 0)
            res.emplace_back(i, parent[i]);
    return res;
}

void testMinimumSpanningTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Minimum Spanning Tree ---------" << endl;
    {
        vector<tuple<int, int, int>> edges{
            { 0, 1, 2 },
            { 0, 3, 6 },
            { 1, 2, 3 },
            { 2, 4, 7 },
            { 3, 4, 9 }
        };

        MinimumSpanningTree<int> graph(5);
        for (auto& it : edges)
            graph.addEdge(get<0>(it), get<1>(it), get<2>(it));

        vector<pair<int, int>> selected1, selected2;
        vector<int> parent;
        auto ans1 = graph.kruskal(selected1);
        auto ans2 = graph.sollin(selected2);
        auto ans3 = graph.prim(parent);
        auto selected3 = makeEdgeList(parent);

        if (ans1 != ans2 || ans1 != ans3)
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << ans2 << ", " << ans3 << endl;
        if (!isEqual(selected1, selected2) || !isEqual(selected1, selected3))
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << ans2 << ", " << ans3 << endl;
        assert(ans1 == 18);
        assert(ans2 == 18);
        assert(ans3 == 18);
        assert(isEqual(selected1, selected2));
        assert(isEqual(selected1, selected3));

        //---

        vector<int> selected11, selected22;
        auto ans11 = MinimumSpanningTree<int>::kruskal(5, edges, selected11);
        auto ans22 = MinimumSpanningTree<int>::sollin(5, edges, selected22);
        if (ans11 != ans22)
            cout << "Mismatched at " << __LINE__ << " : " << ans11 << ", " << ans22 << endl;
        assert(ans11 == 18);
        assert(ans22 == 18);
    }
    cout << "OK" << endl;
    {
        vector<tuple<int, int, int>> edges{
            { 0, 1, 10 },
            { 0, 2,  6 },
            { 0, 3,  5 },
            { 1, 3, 15 },
            { 2, 3,  4 }
        };

        MinimumSpanningTree<int> graph(4);
        for (auto& it : edges)
            graph.addEdge(get<0>(it), get<1>(it), get<2>(it));

        vector<pair<int, int>> selected1;
        vector<pair<int, int>> selected2;
        vector<int> parent;
        auto ans1 = graph.kruskal(selected1);
        auto ans2 = graph.sollin(selected2);
        auto ans3 = graph.prim(parent);
        auto selected3 = makeEdgeList(parent);

        if (ans1 != ans2 || ans1 != ans3)
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << ans2 << ", " << ans3 << endl;
        if (!isEqual(selected1, selected2) || !isEqual(selected1, selected3))
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << ans2 << ", " << ans3 << endl;
        assert(ans1 == 19);
        assert(ans2 == 19);
        assert(ans3 == 19);
        assert(isEqual(selected1, selected2));
        assert(isEqual(selected1, selected3));

        //---

        vector<int> selected11, selected22;
        auto ans11 = MinimumSpanningTree<int>::kruskal(4, edges, selected11);
        auto ans22 = MinimumSpanningTree<int>::sollin(4, edges, selected22);
        if (ans11 != ans22)
            cout << "Mismatched at " << __LINE__ << " : " << ans11 << ", " << ans22 << endl;
        assert(ans11 == 19);
        assert(ans22 == 19);
    }
    cout << "OK" << endl;
    {
        int T = 50;
#ifdef _DEBUG
        const int N = 200;
#else
        const int N = 2000;
#endif
        
        while (T-- > 0) {
            MinimumSpanningTree<int> graph(N);
            vector<tuple<int, int, int>> edges;

            for (int i = 1; i < N; i++) {
                int n = rand() % i + 1;
                vector<int> to(i);
                for (int j = 0; j < i; j++)
                    to[j] = j;
                for (int j = 0; j < n; j++)
                    edges.emplace_back(i, to[j], rand() % 100);
            }
            random_shuffle(edges.begin(), edges.end());
            for (auto& it : edges)
                graph.addEdge(get<0>(it), get<1>(it), get<2>(it));

            vector<pair<int, int>> s1, s2;
            vector<int> s3;
            auto ans1 = graph.kruskal(s1);
            auto ans2 = graph.sollin(s2);
            auto ans3 = graph.prim(s3);
            if (ans1 != ans2 || ans1 != ans3)
                cout << "Mismatch: " << ans1 << ", " << ans2 << ", " << ans3 << endl;
            assert(ans1 == ans2);
            assert(ans1 == ans3);

            //---

            vector<int> selected11, selected22;
            auto ans11 = MinimumSpanningTree<int>::kruskal(N, edges, selected11);
            auto ans22 = MinimumSpanningTree<int>::sollin(N, edges, selected22);
            if (ans11 != ans22)
                cout << "Mismatched at " << __LINE__ << " : " << ans11 << ", " << ans22 << endl;
            assert(ans11 == ans22);
        }
    }
    cout << "OK" << endl;
}
