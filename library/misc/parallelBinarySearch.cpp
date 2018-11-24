#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "parallelBinarySearch.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "../rangeQuery/fenwickTree.h"

// https://www.acmicpc.net/problem/8217
// https://codeforces.com/blog/entry/45578

static vector<int> solve(int N, int M, const vector<vector<int>>& group, const vector<int>& req, const vector<tuple<int, int, int>>& qry) {
    int K = int(qry.size());

    ParallelBinarySearch solver(N, K - 1);

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

    vector<int> ans(N);
    for (int i = 0; i < N; i++) {
        if (solver.get(i) < K)
            ans[i] = solver.get(i);
        else
            ans[i] = -1;
    }

    return ans;
}

void testParallelBinarySearch() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Parallel Binary Search ------------------------" << endl;
    {
        int N = 3;
        int M = 5;
        vector<vector<int>> group{ { 0, 3 }, { 2 }, { 2, 4 } };
        vector<int> req{ 10, 5, 7 };
        vector<tuple<int, int, int>> qry{ { 3, 1, 4 }, { 0, 2, 1 }, { 2, 4, 2 } };

        vector<int> gt{ 3, -1, 1 };
        assert(solve(N, M, group, req, qry) == gt);
    }
    cout << "OK!" << endl;
}
