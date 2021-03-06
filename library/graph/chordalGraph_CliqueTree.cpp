#include <functional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

#include "chordalGraph_CliqueTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_map>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include "../set/unionFind.h"

const int MOD = 998'244'353;

static int modPow(int x, int n) {
    if (n == 0)
        return 1;

    long long t = x % MOD;
    long long res = 1;
    for (; n > 0; n >>= 1) {
        if (n & 1)
            res = res * t % MOD;
        t = t * t % MOD;
    }
    return int(res);
}

// https://www.codechef.com/problems/PALINDEQ
// https://discuss.codechef.com/t/palindeq-editorial/82148
static long long solvePalindromicEquivalence(string S) {
    int N = int(S.length());

    // step #1 : merge palindrom (L, R)
    UnionFind dsu(N);
    vector<pair<int, int>> notMatched;
    for (int i = 0; i < N; i++) {
        for (int L = i - 1, R = i + 1; L >= 0 && R < N; L--, R++) {
            if (S[L] != S[R]) {
                notMatched.emplace_back(L, R);
                break;
            }
            dsu.merge(L, R);
        }
        for (int L = i, R = i + 1; L >= 0 && R < N; L--, R++) {
            if (S[L] != S[R]) {
                notMatched.emplace_back(L, R);
                break;
            }
            dsu.merge(L, R);
        }
    }

    // step #2 : grouping
    //vector<vector<int>> groups;
    vector<int> revGroup(N);
    vector<int> groupSize;
    {
        unordered_map<int, vector<int>> M;
        for (int i = 0; i < N; i++)
            M[dsu.find(i)].push_back(i);

        for (auto& it : M) {
            for (auto j : it.second)
                revGroup[j] = int(groupSize.size());
            groupSize.push_back(int(it.second.size()));
            //groups.emplace_back(move(it.second));
        }
    }

    // step #3 : build graph
    int M = int(groupSize.size());
    vector<vector<int>> graph(M);

    for (auto& it : notMatched) {
        graph[revGroup[it.first]].push_back(revGroup[it.second]);
        graph[revGroup[it.second]].push_back(revGroup[it.first]);
    }
    for (int i = 0; i < M; i++) {
        sort(graph[i].begin(), graph[i].end());
        graph[i].resize(unique(graph[i].begin(), graph[i].end()) - graph[i].begin());
    }

    // step #4 : solve
    CliqueTree<MOD> cliqueTree(graph);

    vector<int> F(M);
    for (int i = 0; i < M; i++) {
        F[i] = modPow(2, groupSize[i]);
        if (--F[i] < 0)
            F[i] += MOD;
    }
    return cliqueTree.sumWeightProductOfIndependentSubsetsInChordalGraph(F);
}


void testChordalGraphToCliqueTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Chordal Graph to Clique Tree ----------------" << endl;
    {
        assert(solvePalindromicEquivalence("aaa") == 8);
        assert(solvePalindromicEquivalence("aba") == 5);
        assert(solvePalindromicEquivalence("abcd") == 6);
        assert(solvePalindromicEquivalence("abxxxcz") == 15);
        assert(solvePalindromicEquivalence("aabxxxcz") == 21);
    }

    cout << "OK" << endl;
}
