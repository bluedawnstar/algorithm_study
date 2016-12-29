#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

//--------- Common ------------------------------------------------------------

#define MAXN    10000           // TODO: modify the maximum number of nodes
#define MAXQ    10000           // TODO: modify the maximum number of questions
#define LOGN    15              // TODO: modify LCA table size

// <Node ID>
// 0 : null node
// 1 : root node
// 2 ~ N : internal or leaf nodes

int gN;
vector<int> gE[MAXN + 1];

int gLevel[MAXN + 1];           // depth (root is 0)

int gP[LOGN][MAXN + 1];         // parent & acestors

                                // to flatten the tree (DFS only)
int gVisTime[MAXN + 1][2];      // visit & exit time
int gTime2Node[MAXN * 2 + 1];   // visit & exit time to node ID (0 <= index < 2 * N)
int gCurrTime;                  //

void clear() {
    if (gN <= 0)
        return;

    for (int i = 1; i <= gN; i++)
        gE[i].clear();
    memset(gLevel, 0, sizeof(gLevel));
    memset(gP, 0, sizeof(gP));

    memset(gVisTime, 0, sizeof(gVisTime));
    memset(gTime2Node, 0, sizeof(gTime2Node));
    gCurrTime = 0;
}

//--------- DFS ---------------------------------------------------------------

void dfs(int u, int parent) {
    gP[0][u] = parent;
    gVisTime[u][0] = gCurrTime;
    gTime2Node[gCurrTime++] = u;

    for (int v : gE[u]) {
        if (v == parent)
            continue;

        gLevel[v] = gLevel[u] + 1;
        dfs(v, u);
    }

    gVisTime[u][1] = gCurrTime;
    gTime2Node[gCurrTime++] = u;
}

void dfsIter(int root) {
    struct Item {
        int u;
        int parent;
        int vi;         // child index
    };
    vector<Item> st;
    st.reserve(gN + 1);

    st.push_back(Item{ root, 0, -1 });
    while (!st.empty()) {
        Item& it = st.back();
        if (++it.vi == 0) {
            // enter ...
            gP[0][it.u] = it.parent;
            gVisTime[it.u][0] = gCurrTime;
            gTime2Node[gCurrTime++] = it.u;
        }
        if (it.vi >= (int)gE[it.u].size()) {
            // leave ...
            gVisTime[it.u][1] = gCurrTime;
            gTime2Node[gCurrTime++] = it.u;
            st.pop_back();
        } else if (gE[it.u][it.vi] != it.parent) {
            // recursion
            int v = gE[it.u][it.vi];
            gLevel[v] = gLevel[it.u] + 1;
            st.push_back(Item{ v, it.u, -1 });
        }
    }
}

//--------- LCA ---------------------------------------------------------------

void makeLcaTable() {
    for (int i = 1; i < LOGN; i++) {
        for (int j = 1; j <= gN; j++) {
            gP[i][j] = gP[i - 1][gP[i - 1][j]];
        }
    }
}

int climbTree(int node, int dist) {
    if (dist <= 0)
        return node;

    for (int i = 0; dist > 0; i++) {
        if (dist & 1)
            node = gP[i][node];
        dist >>= 1;
    }

    return node;
}

int findLCA(int A, int B) {
    if (gLevel[A] < gLevel[B])
        swap(A, B);

    A = climbTree(A, gLevel[A] - gLevel[B]);

    if (A == B)
        return A;

    int bitCnt = 0;
    for (int x = gLevel[A]; x; x >>= 1)
        bitCnt++;

    for (int i = bitCnt - 1; i >= 0; i--) {
        if (gP[i][A] > 0 && gP[i][A] != gP[i][B]) {
            A = gP[i][A];
            B = gP[i][B];
        }
    }

    return gP[0][A];
}

//--------- MO's algorithm ----------------------------------------------------

bool gActiveMO[MAXN + 1];
int  gCurAnsMO;

// lca : lca[index of Q]
// MO  : ((MO_L, MO_R), index of Q)
void initTreeMO(vector<pair<int, int>>& Q, vector<int>& lca, vector<pair<pair<int, int>, int>>& MO) {
    int blockN = (int)sqrt(2 * gN);

    lca.clear();
    MO.clear();

    lca.reserve(Q.size());
    MO.reserve(Q.size());
    for (int i = 0; i < (int)Q.size(); i++) {
        //assert(gVisTime[Q[i].first][0] <= gVisTime[Q[i].second][0]);
        int L = Q[i].first, R = Q[i].second;

        int lc = findLCA(L, R);
        lca.push_back(lc == L ? 0 : lc);
        if (lc == L)
            MO.push_back(make_pair(make_pair(gVisTime[L][0], gVisTime[R][0]), i));
        else
            MO.push_back(make_pair(make_pair(gVisTime[L][1], gVisTime[R][0]), i));
    }

    sort(MO.begin(), MO.end(), [blockN](const auto& l, const auto& r) {
        if (l.first.first / blockN != r.first.first / blockN)
            return l.first.first / blockN < r.first.first / blockN;
        return l.first.second < r.first.second;
    });
}

void addMO(int t);
void removeMO(int t);

void addMO(int t) {
    int u = gTime2Node[t];
    if (gActiveMO[u]) {
        removeMO(t);
    } else {
        gActiveMO[u] = true;

        //TODO: implement adding function
        //gCurAnsMO += ...;
    }
}

void removeMO(int t) {
    int u = gTime2Node[t];
    if (!gActiveMO[u]) {
        addMO(t);
    } else {
        gActiveMO[u] = false;

        //TODO: implement removing function
        //gCurAnsMO -= ...;
    }
}

// template codes for MO's algorithm on a tree
void testTreeMO() {
    // TODO: step1 - make a tree

    dfsIter(1); // dfs(1, 0);
    makeLcaTable();

    vector<pair<int, int>> Q; // (L, R)
    // TODO: step2 - read all queries (L's visit time must be less than R's visit time)
    /*
    Q.reserve(T);
    for (int i = 0; i < T; i++) {
        int L, R;
        scanf("%d %d", &L, &R);

        if (gVisTime[L][0] > gVisTime[R][0])
            swap(L, R);
        Q.push_back(make_pair(L, R));
    }
    */

    vector<int> lca;                        // LCA of (L, R)
    vector<pair<pair<int, int>, int>> MO;   // ((MO_L, MO_R), Q index)

    initTreeMO(Q, lca, MO);

    int currL = MO[0].first.first;
    int currR = MO[0].first.second;
    for (int i = currL; i <= currR; i++)
        addMO(i);

    vector<int> ans(Q.size());
    for (int i = 0; i < (int)Q.size(); i++) {
        int nextL = MO[i].first.first;
        int nextR = MO[i].first.second;
        int qi = MO[i].second;

        while (currL < nextL)
            removeMO(currL++);
        while (currL > nextL)
            addMO(--currL);
        while (currR < nextR)
            addMO(++currR);
        while (currR > nextR)
            removeMO(currR--);

        int addLca = 0;
        if (lca[qi] > 0) {
            int u = lca[qi];

            //TODO: implement adding function about LCA(L, R)
            //      Any global variables mustn't be affected by this codes.
            //addLca = ...;
        }

        ans[qi] = gCurAnsMO + addLca;
    }

    for (int i = 0; i < (int)Q.size(); i++) {
        printf("%d\n", ans[i]);
    }
}
