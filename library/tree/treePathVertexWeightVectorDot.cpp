#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "treePathVertexWeightVectorDot.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static vector<int> makeTree(int N) {
    vector<int> res(N);
    for (int i = 1; i < N; i++)
        res[i] = RandInt32::get() % i;
    return res;
}

static vector<int> makePath(TreePathVertexWeightVectorDot& tree, int u) {
    vector<int> res;
    while (u >= 0) {
        res.push_back(u);
        u = tree.P[0][u];
    }
    return res;
}

static vector<long long> makeGT(TreePathVertexWeightVectorDot& tree, const vector<pair<int,int>>& qry) {
    vector<long long> res;
    for (auto& it : qry) {
        auto path1 = makePath(tree, it.first);
        auto path2 = makePath(tree, it.second);

        long long dot = 0;
        for (int i = 0; i < int(path1.size()); i++)
            dot += 1ll * tree.W[path1[i]] * tree.W[path2[i]];

        res.push_back(dot);
    }
    return res;
}

void testTreePathVertexWeightVectorDot() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Tree Path - Vertex Weight Vector Dot -----------------------" << endl;
    {
        const int T = 10;
        const int N = 1'000;
        const int MAXW = 1'000;

        for (int tt = 0; tt < T; tt++) {
            vector<int> W(N);
            for (int i = 0; i < N; i++)
                W[i] = RandInt32::get() % MAXW + 1;
            auto treeP = makeTree(N);

            TreePathVertexWeightVectorDot tree(W);
            for (int i = 1; i < N; i++)
                tree.add(treeP[i], i);
            tree.build();

            vector<vector<int>> depthGroup(*max_element(tree.level.begin(), tree.level.end()) + 1);
            for (int u = 0; u < N; u++)
                depthGroup[tree.level[u]].push_back(u);

            vector<pair<int, int>> qry;
            for (int i = 0; i < N; i++) {
                int j = i % depthGroup.size();
                int u = depthGroup[j][RandInt32::get() % depthGroup[j].size()];
                int v = depthGroup[j][RandInt32::get() % depthGroup[j].size()];
                qry.emplace_back(u, v);
            }

            auto gt = makeGT(tree, qry);
            auto ans = tree.solve(qry);

            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
