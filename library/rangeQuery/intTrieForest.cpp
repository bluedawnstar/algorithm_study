#include <vector>
#include <functional>

using namespace std;

#include "intTrieForest.h"
#include "intTrieForestRangeSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// (key, value)
static int sumSlowLessThanOrEqual(vector<pair<int, int>>& v, int key) {
    int res = 0;
    for (auto& it : v) {
        if (it.first <= key)
            res += it.second;
    }
    return res;
}

// (key, value)
static int sumSlowGreaterThanOrEqual(vector<pair<int, int>>& v, int key) {
    int res = 0;
    for (auto& it : v) {
        if (it.first >= key)
            res += it.second;
    }
    return res;
}

// (key, value)
static int sumSlow(vector<pair<int, int>>& v, int keyL, int keyR) {
    int res = 0;
    for (auto& it : v) {
        if (keyL <= it.first && it.first <= keyR)
            res += it.second;
    }
    return res;
}

void testIntTrieForest() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Integer Trie Forest (Binary Trie) -------------------" << endl;
    {
        int N = 10000;
        int T = 100;
        int BIT_SIZE = 20;
        int mask = (1 << BIT_SIZE) - 1;

#ifdef _DEBUG
        T = 10;
#endif

        while (T-- > 0) {
            vector<pair<int,int>> in(N);
            unordered_set<int> S;
            IntTrieForest<int,int> trie(BIT_SIZE, [](int a, int b) { return a; }, [](int a, int b) { return a; });

            for (int i = 0; i < N; i++) {
                in[i].first = in[i].second = RandInt32::get() & mask;
                S.insert(in[i].first);
            }
            sort(in.begin(), in.end());

            int root = trie.build(in);

            for (int i = 0; i <= mask; i++) {
                bool gt = (S.find(i) != S.end());
                bool ans = trie.exist(root, i);
                if (gt != ans) {
                    cout << "Mismatched at " << i << " : " << ans << ", " << gt << endl;
                    //ans = trie.exist(root, i);
                }
                assert(gt == ans);
            }
        }
    }
    cout << "OK!" << endl;
    {
        int N = 10000;
        int T = 100;
        int BIT_SIZE = 20;
        int mask = (1 << BIT_SIZE) - 1;

#ifdef _DEBUG
        T = 10;
#endif

        while (T-- > 0) {
            vector<pair<int,int>> in(N);
            unordered_set<int> S;
            IntTrieForest<int,int> trie(BIT_SIZE, [](int a, int b) { return a; }, [](int a, int b) { return a; });

            int root = trie.build();

            for (int i = 0; i < N; i++) {
                in[i].first = in[i].second = RandInt32::get() & mask;
                S.insert(in[i].first);
                trie.insert(root, in[i].first, in[i].second);
            }

            for (int i = 0; i <= mask; i++) {
                bool gt = (S.find(i) != S.end());
                bool ans = trie.exist(root, i);
                if (gt != ans) {
                    cout << "Mismatched at " << i << " : " << ans << ", " << gt << endl;
                    //ans = trie.exist(root, i);
                }
                assert(gt == ans);
            }
        }
    }
    cout << "OK!" << endl;
    {
        int N = 10000;
        int T = 100;
        int Q = 1000;
        int BIT_SIZE = 20;
        int mask = (1 << BIT_SIZE) - 1;

#ifdef _DEBUG
        N = 1000;
        T = 10;
        Q = 100;
#endif

        while (T-- > 0) {
            vector<pair<int,int>> in(N);
            IntTrieForest<int,int> trie(BIT_SIZE, [](int a, int b) { return a; }, [](int a, int b) { return a; });

            for (int i = 0; i < N; i++)
                in[i].first = in[i].second = RandInt32::get() & mask;
            sort(in.begin(), in.end());

            int root = trie.build(in);

            for (int i = 0; i < Q; i++) {
                int K = RandInt32::get() & mask;

                auto ans = trie.findMaxXor(root, K);
                if (ans.first != ans.second)
                    cout << "Invalid answer at " << K << " : " << ans.first << ", " << ans.second << endl;

                int maxXor = 0;
                for (int j = 0; j < N; j++) {
                    int t = in[j].first ^ K;
                    maxXor = max(maxXor, t);
                }

                auto gt = maxXor ^ K;
                if (gt != ans.first)
                    cout << "Mismatched at " << K << " : " << ans.first << ", " << gt << endl;
                assert(gt == ans.first);
            }
        }
    }
    cout << "OK!" << endl;

    cout << "*** test for range-sum ***" << endl;
    {
        int N = 100;
        const int MAX_KEY = 1000;
        const int MAX_KEY_BIT = 10;
        const int MAX_VAL = 10000;
        const int T = 100;

        vector<pair<int, int>> in;
        for (int i = 0; i < N; i++)
            in.emplace_back(RandInt32::get() % MAX_KEY, RandInt32::get() % MAX_VAL);
        sort(in.begin(), in.end());
        in.erase(unique(in.begin(), in.end(), [](const pair<int, int>& a, const pair<int, int>& b) { return a.first == b.first; }), in.end());

        N = int(in.size());

        auto addOp = [](int a, int b) {
            return a + b;
        };
        IntTrieForest<int, int> trie(MAX_KEY_BIT, addOp, addOp);
        IntTrieForestRangeSum<int, int> trieSum(MAX_KEY_BIT);

        int root0 = trie.build();
        int root = trieSum.createTrie();
        for (auto& it : in) {
            trie.insert(root0, it.first, it.second);
            trieSum.insert(root, it.first, it.second);
        }

        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            {
                auto ans0 = trie.query(root0, in[L].first, MAX_KEY, 0, addOp);
                auto ans1 = trieSum.sumGreaterThanOrEqual(root, in[L].first);
                auto gt = sumSlowGreaterThanOrEqual(in, in[L].first);
                if (ans0 != gt || ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans0 << ", " << ans1 << ", " << gt << endl;
                }
                assert(ans0 == gt && ans1 == gt);
            }
            {
                auto ans0 = trie.query(root0, 0, in[R].first, 0, addOp);
                auto ans1 = trieSum.sumLessThanOrEqual(root, in[R].first);
                auto gt = sumSlowLessThanOrEqual(in, in[R].first);
                if (ans0 != gt || ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans0 << ", " << ans1 << ", " << gt << endl;
                }
                assert(ans0 == gt && ans1 == gt);
            }
            {
                auto ans0 = trie.query(root0, 0, in[L].first, 0, addOp);
                auto ans1 = trieSum.sumLessThanOrEqual(root, in[L].first);
                auto gt = sumSlowLessThanOrEqual(in, in[L].first);
                if (ans0 != gt || ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans0 << ", " << ans1 << ", " << gt << endl;
                }
                assert(ans0 == gt && ans1 == gt);
            }
            {
                auto ans0 = trie.query(root0, 0, in[L].first - 1, 0, addOp);
                auto ans1 = trieSum.sumLessThanOrEqual(root, in[L].first - 1);
                auto gt = sumSlowLessThanOrEqual(in, in[L].first - 1);
                if (ans0 != gt || ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans0 << ", " << ans1 << ", " << gt << endl;
                }
                assert(ans0 == gt && ans1 == gt);
            }
            {
                auto ans0 = trie.query(root0, in[L].first - 1, in[L].first - 1, 0, addOp);
                auto ans1 = trieSum.sumRange(root, in[L].first - 1, in[L].first - 1);
                auto gt = sumSlow(in, in[L].first - 1, in[L].first - 1);
                if (ans0 != gt || ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans0 << ", " << ans1 << ", " << gt << endl;
                }
                assert(ans0 == gt && ans1 == gt);
            }
            {
                auto ans0 = trie.query(root0, in[L].first, in[R].first, 0, addOp);
                auto ans1 = trieSum.sumRange(root, in[L].first, in[R].first);
                auto gt = sumSlow(in, in[L].first, in[R].first);
                if (ans0 != gt || ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans0 << ", " << ans1 << ", " << gt << endl;
                }
                assert(ans0 == gt && ans1 == gt);
            }
        }
    }
    cout << "OK!" << endl;
}
