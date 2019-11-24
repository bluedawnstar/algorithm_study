#include <cassert>
#include <functional>
#include <algorithm>

using namespace std;

#include "redBlackTreeRangeQuery.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <set>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testRBTreeRangeQuery() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Red-Black Tree with Range Query -------------------------" << endl;
    {
        RBTreeRangeQuery<int> tree([](int a, int b) { return a + b; }, 0);

        int N = 5;
        vector<int> v{ 1, 2, 3, 4, 5 };

        for (int i = 0; i < N; i++)
            tree.insert(v[i]);

        // prefix query with indexes
        for (int i = 0; i < N; i++) {
            int gt = accumulate(v.begin(), v.begin() + i + 1, 0);
            int ans = tree.queryPrefix(i);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }

        // suffix query with indexes
        for (int i = 0; i < N; i++) {
            int gt = accumulate(v.begin() + i, v.end(), 0);
            int ans = tree.querySuffix(i);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }

        // range query with indexes
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++) {
                int gt = accumulate(v.begin() + i, v.begin() + j + 1, 0);
                int ans = tree.query(i, j);
                if (gt != ans) {
                    cout << "Mismatched : " << ans << ", " << gt << endl;
                    ans = tree.query(i, j);
                }
                assert(ans == gt);
            }
        }

        // range query with value
        {
            int gt = accumulate(v.begin(), v.end(), 0);
            int ans = tree.queryWithValue(-1, 10001);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        for (int i = 0; i < N; i++) {
            if (i > 0 && v[i] == v[i - 1])
                continue;
            int gt = accumulate(v.begin() + i, v.end(), 0);
            int ans = tree.querySuffixWithValue(v[i]);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        for (int j = 0; j < N; j++) {
            if (j + 1 < N && v[j] == v[j + 1])
                continue;

            int gt = accumulate(v.begin(), v.begin() + j + 1, 0);
            int ans = tree.queryPrefixWithValue(v[j]);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        for (int i = 0; i < N; i++) {
            if (i > 0 && v[i] == v[i - 1])
                continue;

            for (int j = i; j < N; j++) {
                if (j + 1 < N && v[j] == v[j + 1])
                    continue;

                int gt = accumulate(v.begin() + i, v.begin() + j + 1, 0);
                int ans = tree.queryWithValue(v[i], v[j]);
                if (gt != ans)
                    cout << "Mismatched : " << ans << ", " << gt << endl;
                assert(ans == gt);
            }
        }
    }
    {
        RBTreeRangeQuery<int> tree([](int a, int b) { return a + b; }, 0);

        int N = 100;
        vector<int> v;
        for (int i = 0; i < N; i++)
            v.push_back(RandInt32::get() % 10000);
        sort(v.begin(), v.end());

        for (int i = 0; i < N; i++)
            tree.insert(v[i]);

        // prefix query with indexes
        for (int i = 0; i < N; i++) {
            int gt = accumulate(v.begin(), v.begin() + i + 1, 0);
            int ans = tree.queryPrefix(i);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }

        // suffix query with indexes
        for (int i = 0; i < N; i++) {
            int gt = accumulate(v.begin() + i, v.end(), 0);
            int ans = tree.querySuffix(i);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }

        // range query with indexes
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++) {
                int gt = accumulate(v.begin() + i, v.begin() + j + 1, 0);
                int ans = tree.query(i, j);
                if (gt != ans)
                    cout << "Mismatched : " << ans << ", " << gt << endl;
                assert(ans == gt);
            }
        }

        // range query with value
        {
            int gt = accumulate(v.begin(), v.end(), 0);
            int ans = tree.queryWithValue(-1, 10001);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        for (int i = 0; i < N; i++) {
            if (i > 0 && v[i] == v[i - 1])
                continue;
            int gt = accumulate(v.begin() + i, v.end(), 0);
            int ans = tree.querySuffixWithValue(v[i]);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        for (int j = 0; j < N; j++) {
            if (j + 1 < N && v[j] == v[j + 1])
                continue;

            int gt = accumulate(v.begin(), v.begin() + j + 1, 0);
            int ans = tree.queryPrefixWithValue(v[j]);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        for (int i = 0; i < N; i++) {
            if (i > 0 && v[i] == v[i - 1])
                continue;

            for (int j = i; j < N; j++) {
                if (j + 1 < N && v[j] == v[j + 1])
                    continue;

                int gt = accumulate(v.begin() + i, v.begin() + j + 1, 0);
                int ans = tree.queryWithValue(v[i], v[j]);
                if (gt != ans)
                    cout << "Mismatched : " << ans << ", " << gt << endl;
                assert(ans == gt);
            }
        }
    }
    cout << "OK!" << endl;
}
