#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "parallelBinarySearchRecursive.h"

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

// https://codeforces.com/blog/entry/45578
static vector<int> solve(int N, int M, const vector<vector<int>>& group, const vector<int>& req, const vector<tuple<int, int, int>>& qry) {
    int K = int(qry.size());

    FenwickTree<long long> tree(M);
    RecursiveParallelBinarySearch<> solver(
        [&tree, &qry, M](int midValue) {
            int l, r, a;
            tie(l, r, a) = qry[midValue];

            if (l <= r)
                tree.addRange(l, r, a);
            else {
                tree.addRange(0, r, a);
                tree.addRange(l, M - 1, a);
            }
        },
        [&tree, &qry, M](int midValue) {
            int l, r, a;
            tie(l, r, a) = qry[midValue];

            if (l <= r)
                tree.addRange(l, r, -a);
            else {
                tree.addRange(0, r, -a);
                tree.addRange(l, M - 1, -a);
            }
        },
        [&tree, &group, &req](int index, int midValue) -> bool {
            long long sum = 0;
            for (int v : group[index]) {
                sum += tree.sum(v);
                if (sum >= req[index])
                    return true;
            }
            return false;
        }
    );

    return solver.solve(N, K - 1);
}

void testParallelBinarySearchRecursive() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Recursive Parallel Binary Search ------------------------" << endl;
    // https://codeforces.com/blog/entry/45578
    {
        int N = 3;
        int M = 5;
        vector<vector<int>> group{ { 0, 3 },{ 2 },{ 1, 4 } };
        vector<int> req{ 10, 5, 7 };
        vector<tuple<int, int, int>> qry{ { 3, 1, 4 },{ 0, 2, 1 },{ 2, 4, 2 } };

        vector<int> gt{ 2, -1, 0 };
        assert(solve(N, M, group, req, qry) == gt);
    }
    cout << "OK!" << endl;
}
