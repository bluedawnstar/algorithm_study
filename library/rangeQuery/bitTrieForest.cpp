#include <vector>
#include <algorithm>

using namespace std;

#include "bitTrieForest.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
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
static int sumSlow(vector<pair<int,int>>& v, int keyL, int keyR) {
    int res = 0;
    for (auto& it : v) {
        if (keyL <= it.first && it.first <= keyR)
            res += it.second;
    }
    return res;
}

void testBitTrieForest() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Bit Trie Forest -------------------" << endl;
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
        in.erase(unique(in.begin(), in.end(), [](const pair<int,int>& a, const pair<int,int>& b) { return a.first == b.first; }), in.end());

        N = int(in.size());

        BitTrieForest<int> trie(MAX_KEY_BIT);

        int root = trie.createTrie();
        for (auto& it : in)
            trie.insert(root, it.first, it.second);

        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            {
                auto ans1 = trie.sumGreaterThanOrEqual(root, in[L].first);
                auto gt = sumSlowGreaterThanOrEqual(in, in[L].first);
                if (ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt << endl;
                }
                assert(ans1 == gt);
            }
            {
                auto ans1 = trie.sumLessThanOrEqual(root, in[R].first);
                auto gt = sumSlowLessThanOrEqual(in, in[R].first);
                if (ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt << endl;
                }
                assert(ans1 == gt);
            }
            {
                auto ans1 = trie.sumRange(root, in[L].first, in[R].first);
                auto gt = sumSlow(in, in[L].first, in[R].first);
                if (ans1 != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt << endl;
                }
                assert(ans1 == gt);
            }
        }
    }

    cout << "OK!" << endl;
}
