#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "parallelBinarySearch.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <numeric>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "../rangeQuery/fenwickTree.h"

// https://codeforces.com/blog/entry/45578
static vector<int> solve(int N, int M, const vector<vector<int>>& group, const vector<int>& req, const vector<tuple<int, int, int>>& qry) {
    int K = int(qry.size());

    ParallelBinarySearch<int> solver(N, K - 1);

    while (true) {
        FenwickTree<int> tree(M);

        if (!solver.updateRange())
            break;
        solver.updateValue(
            [&qry, &tree, M](int midValue) {
                int l, r, a;
                tie(l, r, a) = qry[midValue];

                if (l <= r)
                    tree.addRange(l, r, a);
                else {
                    tree.addRange(0, r, a);
                    tree.addRange(l, M - 1, a);
                }
            },
            [&group, &tree, &req](int index, int midValue) -> bool {
                int sum = 0;
                for (int v : group[index]) {
                    sum += tree.sum(v);
                    if (sum >= req[index])
                        return true;
                }
                return false;
            }
        );
    }

    vector<int> ans(N, -1);
    for (int i = 0; i < N; i++) {
        if (solver.get(i) < K)
            ans[i] = solver.get(i);
    }

    return ans;
}

// https://www.hackerrank.com/contests/may-world-codesprint/challenges/travel-in-hackerland/problem
namespace TravelInHackerLand {
    int N, M, Q;
    vector<int> gV;
    vector<tuple<int, int, int>> gE;
    vector<tuple<int, int, int>> gQry;

    struct UnionFindWithCount {
        vector<int> parent;
        vector<int> rank;
        vector<set<int>> count;

        UnionFindWithCount() {
        }

        explicit UnionFindWithCount(int N) {
            init(N);
        }

        void init(int N) {
            parent.resize(N);
            rank.assign(N, 0);
            count.resize(N);
            for (int i = 0; i < N; i++) {
                parent[i] = i;
                count[i].insert(gV[i]);
            }
        }

        int find(int x) {
            if (parent[x] == x)
                return x;
            return parent[x] = find(parent[x]);
        }

        int merge(int x, int y) {
            int xset = find(x);
            int yset = find(y);
            if (xset == yset)
                return xset;

            if (rank[xset] < rank[yset]) {
                parent[xset] = yset;
                for (auto it : count[xset])
                    count[yset].insert(it);
                count[xset].clear();
                return yset;
            } else {
                parent[yset] = xset;
                if (rank[xset] == rank[yset])
                    rank[xset]++;
                for (auto it : count[yset])
                    count[xset].insert(it);
                count[yset].clear();
                return xset;
            }
        }

        int getCount(int x) {
            return int(count[find(x)].size());
        }
    };

    vector<int> solve() {
        sort(gE.begin(), gE.end(), [](const tuple<int, int, int>& e1, const tuple<int, int, int>& e2) {
            return get<2>(e1) < get<2>(e2);
        });

        ParallelBinarySearch<int> solver(Q, M - 1);

        while (true) {
            UnionFindWithCount uf(N);

            if (!solver.updateRange())
                break;

            solver.updateValue(
                [&uf](int midValue) {
                    uf.merge(get<0>(gE[midValue]), get<1>(gE[midValue]));
                },
                [&uf](int index, int midValue) {
                    int x, y, k;
                    tie(x, y, k) = gQry[index];
                    if (uf.find(x) != uf.find(y))
                        return false;
                    return uf.getCount(x) >= k;
                }
            );
        }

        vector<int> ans(Q, -1);
        for (int i = 0; i < Q; i++) {
            int j = solver.get(i);
            if (0 <= j && j < M)
                ans[i] = get<2>(gE[j]);
        }

        return ans;
    }
}

// https://www.acmicpc.net/problem/1396
namespace BOJ1396 {
    int N, M, Q;
    vector<tuple<int, int, int>> gE;
    vector<pair<int, int>> gQry;

    struct UnionFindWithCount {
        vector<int> parent;
        vector<int> rank;
        vector<int> count;

        UnionFindWithCount() {
        }

        explicit UnionFindWithCount(int N) {
            init(N);
        }

        void init(int N) {
            parent.resize(N);
            rank.assign(N, 0);
            count.assign(N, 1);
            for (int i = 0; i < N; i++)
                parent[i] = i;
        }

        int find(int x) {
            if (parent[x] == x)
                return x;
            return parent[x] = find(parent[x]);
        }

        int merge(int x, int y) {
            int xset = find(x);
            int yset = find(y);
            if (xset == yset)
                return xset;

            if (rank[xset] < rank[yset]) {
                parent[xset] = yset;
                count[yset] += count[xset];
                return yset;
            } else {
                parent[yset] = xset;
                if (rank[xset] == rank[yset])
                    rank[xset]++;
                count[xset] += count[yset];
                return xset;
            }
        }

        int getCount(int x) {
            return count[find(x)];
        }
    };

    vector<pair<int, int>> solve() {
        sort(gE.begin(), gE.end(), [](const tuple<int, int, int>& e1, const tuple<int, int, int>& e2) {
            return get<2>(e1) < get<2>(e2);
        });

        vector<pair<int, int>> ans(Q, make_pair(-1, -1));

        ParallelBinarySearch<int> solver(Q, M - 1);

        while (true) {
            UnionFindWithCount uf(N);

            if (!solver.updateRange())
                break;

            solver.updateValue(
                [&uf](int midValue) {
                    uf.merge(get<0>(gE[midValue]), get<1>(gE[midValue]));
                },
                [&uf, &ans](int index, int midValue) {
                    if (uf.find(gQry[index].first) != uf.find(gQry[index].second))
                        return false;
                    ans[index] = make_pair(get<2>(gE[midValue]), uf.getCount(gQry[index].first));
                    return true;
                }
            );
        }

        return ans;
    }
}

void testParallelBinarySearch() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Parallel Binary Search ------------------------" << endl;
    // https://codeforces.com/blog/entry/45578
    {
        int N = 3;
        int M = 5;
        vector<vector<int>> group{ { 0, 3 }, { 2 }, { 1, 4 } };
        vector<int> req{ 10, 5, 7 };
        vector<tuple<int, int, int>> qry{ { 3, 1, 4 }, { 0, 2, 1 }, { 2, 4, 2 } };

        vector<int> gt{ 2, -1, 0 };
        assert(solve(N, M, group, req, qry) == gt);
    }
    // https://www.hackerrank.com/contests/may-world-codesprint/challenges/travel-in-hackerland/problem
    {
        TravelInHackerLand::N = 7;
        TravelInHackerLand::M = 6;
        TravelInHackerLand::Q = 1;
        TravelInHackerLand::gV = vector<int>{ 1, 1, 4, 5, 1, 3, 2 };
        TravelInHackerLand::gE = vector<tuple<int, int, int>>{
            { 0, 1, 3 },
            { 1, 5, 2 },
            { 1, 2, 4 },
            { 2, 3, 3 },
            { 1, 3, 9 },
            { 4, 6, 9 },
        };
        TravelInHackerLand::gQry = vector<tuple<int, int, int>>{
            { 0, 1, 4 }
        };

        vector<int> gt{ 4 };
        assert(TravelInHackerLand::solve() == gt);
    }
    // https://www.acmicpc.net/problem/1396
    {
        BOJ1396::N = 5;
        BOJ1396::M = 4;
        BOJ1396::Q = 2;
        BOJ1396::gE = vector<tuple<int, int, int>>{
            { 0, 1, 1 },
            { 1, 2, 2 },
            { 2, 3, 4 },
            { 3, 4, 3 }
        };
        BOJ1396::gQry = vector<pair<int,int>>{
            { 0, 4 },
            { 1, 2 }
        };

        vector<pair<int, int>> gt{
            { 4, 5 },
            { 2, 3 }
        };
        assert(BOJ1396::solve() == gt);
    }
    cout << "OK!" << endl;
}
