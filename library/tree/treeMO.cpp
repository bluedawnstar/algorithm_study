#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeBasic.h"

struct TreeMO : public Tree {
    // to flatten the tree (DFS only)
    vector<pair<int, int>>  mVisTime;   // visit & exit time
    vector<int>             mTime2Node; // visit & exit time to node ID (0 <= index < 2 * N)
    int                     mCurrTime;  //

    TreeMO(int N, int logN) : Tree(N, logN), mVisTime(N), mTime2Node(N * 2) {
        mCurrTime = 0;
    }

    void clear() {
        Tree::clear();

        fill(mVisTime.begin(), mVisTime.end(), pair<int, int>());    // is it necessary?
        fill(mTime2Node.begin(), mTime2Node.begin(), 0);            // is it necessary?
        mCurrTime = 0;
    }

    //--------- DFS -----------------------------------------------------------

    void dfs(int u, int parent) {
        mVisTime[u].first = mCurrTime;
        mTime2Node[mCurrTime++] = u;

        mP[0][u] = parent;
        for (int v : mE[u]) {
            if (v == parent)
                continue;

            mLevel[v] = mLevel[u] + 1;
            dfs(v, u);
        }

        mVisTime[u].second = mCurrTime;
        mTime2Node[mCurrTime++] = u;
    }

    void dfsIter(int root) {
        struct Item {
            int u;
            int parent;
            int vi;         // child index
        };
        vector<Item> st;
        st.reserve(mN);

        st.push_back(Item{ root, -1, -1 });
        while (!st.empty()) {
            Item& it = st.back();
            if (++it.vi == 0) {
                // enter ...
                mP[0][it.u] = it.parent;
                mVisTime[it.u].first = mCurrTime;
                mTime2Node[mCurrTime++] = it.u;
            }
            if (it.vi >= (int)mE[it.u].size()) {
                // leave ...
                mVisTime[it.u].second = mCurrTime;
                mTime2Node[mCurrTime++] = it.u;
                st.pop_back();
            } else if (mE[it.u][it.vi] != it.parent) {
                // recursion
                int v = mE[it.u][it.vi];
                mLevel[v] = mLevel[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
        }
    }

    //--------- MO's algorithm ------------------------------------------------

    vector<bool> mActiveMO;
    int  mCurAnsMO;

    // lca : lca[index of Q]
    // MO  : ((MO_L, MO_R), index of Q)
    void initTreeMO(vector<pair<int, int>>& Q, vector<int>& lca, vector<pair<pair<int, int>, int>>& MO) {
        int blockN = (int)sqrt(2 * mN);

        mActiveMO = vector<bool>(mN);

        lca.clear();
        MO.clear();

        lca.reserve(Q.size());
        MO.reserve(Q.size());
        for (int i = 0; i < (int)Q.size(); i++) {
            //assert(mVisTime[Q[i].first][0] <= mVisTime[Q[i].second][0]);
            int L = Q[i].first, R = Q[i].second;

            int lc = findLCA(L, R);
            lca.push_back(lc == L ? -1 : lc);
            if (lc == L)
                MO.push_back(make_pair(make_pair(mVisTime[L].first, mVisTime[R].first), i));
            else
                MO.push_back(make_pair(make_pair(mVisTime[L].second, mVisTime[R].first), i));
        }

        sort(MO.begin(), MO.end(), [blockN](const auto& l, const auto& r) {
            if (l.first.first / blockN != r.first.first / blockN)
                return l.first.first / blockN < r.first.first / blockN;
            return l.first.second < r.first.second;
        });
    }

    void addMO(int t) {
        int u = mTime2Node[t];
        if (mActiveMO[u]) {
            removeMO(t);
        } else {
            mActiveMO[u] = true;

            //TODO: implement adding function
            //mCurAnsMO += ...;
        }
    }

    void removeMO(int t) {
        int u = mTime2Node[t];
        if (!mActiveMO[u]) {
            addMO(t);
        } else {
            mActiveMO[u] = false;

            //TODO: implement removing function
            //mCurAnsMO -= ...;
        }
    }
};


///////////////////////////////////////////////////////////////////////////////


#define MAXN    10000           // TODO: modify the maximum number of nodes
#define MAXQ    10000           // TODO: modify the maximum number of questions
#define LOGN    15              // TODO: modify LCA table size (log2(MAXN))


// template codes for MO's algorithm on a tree
void testTreeMO() {
    return; //TODO: if you want to test a split function, make this line a comment.

    // TODO: step1 - make a tree
    TreeMO tree(MAXN, LOGN);
    tree.setVertexCount(MAXN);      // TODO: set the number of vertex
    tree.dfsIter(0);                // dfs(0, -1);
    tree.makeLcaTable();

    vector<pair<int, int>> Q; // (L, R)
    // TODO: step2 - read all queries (L's visit time must be less than R's visit time)
    /*
    Q.reserve(T);
    for (int i = 0; i < T; i++) {
        int L, R;
        scanf("%d %d", &L, &R);
        L--;
        R--;

        if (tree.mVisTime[L].first > tree.mVisTime[R].first)
            swap(L, R);
        Q.push_back(make_pair(L, R));
    }
    */

    vector<int> lca;                        // LCA of (L, R)
    vector<pair<pair<int, int>, int>> MO;   // ((MO_L, MO_R), Q index)

    tree.initTreeMO(Q, lca, MO);

    int currL = MO[0].first.first;
    int currR = MO[0].first.second;
    for (int i = currL; i <= currR; i++)
        tree.addMO(i);

    vector<int> ans(Q.size());
    for (int i = 0; i < (int)Q.size(); i++) {
        int nextL = MO[i].first.first;
        int nextR = MO[i].first.second;
        int qi = MO[i].second;

        while (currL < nextL)
            tree.removeMO(currL++);
        while (currL > nextL)
            tree.addMO(--currL);
        while (currR < nextR)
            tree.addMO(++currR);
        while (currR > nextR)
            tree.removeMO(currR--);

        int addLca = 0;
        if (lca[qi] >= 0) {
            int u = lca[qi];

            //TODO: implement adding function about LCA(L, R)
            //      Any global variables mustn't be affected by this codes.
            //addLca = ...;
        }

        ans[qi] = tree.mCurAnsMO + addLca;
    }

    for (int i = 0; i < (int)Q.size(); i++) {
        printf("%d\n", ans[i]);
    }
}
