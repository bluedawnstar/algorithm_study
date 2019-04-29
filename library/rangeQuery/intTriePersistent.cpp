#include <vector>
#include <functional>

using namespace std;

#include "intTriePersistent.h"

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

void testIntTriePersistent() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Persistent Integer Trie (Binary Trie) -------------------" << endl;
    {
        int N = 10000;
        int T = 100;
        int BIT_SIZE = 20;
        int mask = (1 << BIT_SIZE) - 1;

#ifdef _DEBUG
        T = 10;
#endif

        while (T-- > 0) {
            vector<int> in(N);
            unordered_set<int> S;
            PersistentIntTrie<int> trie(BIT_SIZE, [](int x) { return x; }, [](int a, int b) { return a; });

            for (int i = 0; i < N; i++) {
                in[i] = RandInt32::get() & mask;
                S.insert(in[i]);
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
            vector<int> in(N);
            unordered_set<int> S;
            PersistentIntTrie<int> trie(BIT_SIZE, [](int x) { return x; }, [](int a, int b) { return a; });

            int root = -1;

            for (int i = 0; i < N; i++) {
                in[i] = RandInt32::get() & mask;
                S.insert(in[i]);
                root = trie.add(root, in[i]);
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
        int BIT_SIZE = 20;
        int mask = (1 << BIT_SIZE) - 1;

#ifdef _DEBUG
        T = 10;
#endif

        while (T-- > 0) {
            vector<int> in(N);
            PersistentIntTrie<int> trie(BIT_SIZE, [](int x) { return x; }, [](int a, int b) { return a; });

            vector<int> roots(N + 1);
            roots[0] = -1;

            for (int i = 0; i < N; i++) {
                in[i] = RandInt32::get() & mask;
                roots[i + 1] = trie.add(roots[i], in[i]);
            }

            for (int i = 0; i < N; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);

                int K = in[RandInt32::get() % N];

                bool gt = false;
                for (int i = L; i <= R; i++) {
                    if (in[i] == K) {
                        gt = true;
                        break;
                    }
                }
                bool ans = trie.exist(roots[L], roots[R + 1], K);
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
            vector<int> in(N);
            PersistentIntTrie<int> trie(BIT_SIZE, [](int x) { return x; }, [](int a, int b) { return a; });

            for (int i = 0; i < N; i++)
                in[i] = RandInt32::get() & mask;
            sort(in.begin(), in.end());

            int root = trie.build(in);

            for (int i = 0; i < Q; i++) {
                int K = RandInt32::get() & mask;

                auto ans = trie.findMaxXor(root, K);
                if (ans.first != ans.second)
                    cout << "Invalid answer at " << K << " : " << ans.first << ", " << ans.second << endl;

                int maxXor = 0;
                for (int j = 0; j < N; j++) {
                    int t = in[j] ^ K;
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
            vector<int> in(N);
            PersistentIntTrie<int> trie(BIT_SIZE, [](int x) { return x; }, [](int a, int b) { return a; });

            vector<int> roots(N + 1);
            roots[0] = -1;

            for (int i = 0; i < N; i++) {
                in[i] = RandInt32::get() & mask;
                roots[i + 1] = trie.add(roots[i], in[i]);
            }

            for (int i = 0; i < Q; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);

                int K = RandInt32::get() & mask;

                auto ans = trie.findMaxXor(roots[L], roots[R + 1], K);
                if (ans.first != ans.second)
                    cout << "Invalid answer at " << K << " : " << ans.first << ", " << ans.second << endl;

                int maxXor = 0;
                for (int j = L; j <= R; j++) {
                    int t = in[j] ^ K;
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
}
