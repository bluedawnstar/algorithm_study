#include <memory.h>
#include <cassert>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cstring>
#include <climits>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <limits>
#include <functional>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     fprintf(stderr, "elapsed time (" #i ") = %f\n", double(clock() - start##i) / CLOCKS_PER_SEC)

typedef long long           ll;
typedef unsigned long long  ull;

template <typename T>
struct SimplePersistentSegmentTree {
    int N;
    vector<T> value;
    vector<int> L;
    vector<int> R;
    int root;       // first root;

    SimplePersistentSegmentTree() {
    }

    explicit SimplePersistentSegmentTree(int n) {
        build(n);
    }

    // return root node index
    int build(int n) {
        N = n;
        value.reserve(N * 4);
        L.reserve(N * 4);
        R.reserve(N * 4);
        return root = dfsBuild(0, N - 1);
    }

    // return root node index
    int add(int root, int index, T val) {
        return dfsAdd(root, 0, N - 1, index, val);
    }

private:
    int dfsBuild(int left, int right) {
        int l = -1, r = -1;
        if (left < right) {
            int mid = (left + right) >> 1;
            l = dfsBuild(left, mid);
            r = dfsBuild(mid + 1, right);
        }
        value.push_back(0);
        L.push_back(l);
        R.push_back(r);
        return int(value.size()) - 1;
    }

    int dfsAdd(int node, int left, int right, int index, T val) {
        T newValue;
        int l = -1, r = -1;
        if (left == right) {
            newValue = value[node] + val;           //TODO: merge operation
        } else {
            int mid = (left + right) >> 1;
            if (index <= mid) {
                l = dfsAdd(L[node], left, mid, index, val);
                r = R[node];
            } else {
                l = L[node];
                r = dfsAdd(R[node], mid + 1, right, index, val);
            }
            newValue = value[l] + value[r];         //TODO: merge operation
        }
        value.push_back(newValue);
        L.push_back(l);
        R.push_back(r);
        return int(value.size()) - 1;
    }
};


#define MAXN    50000

int gN;
vector<int> gE[MAXN + 1];
vector<int> gA;                 // compressed values (0 <= gA[i] < gM)

int gM;                         // sizeof gUniqA
vector<int> gUniqueA;           // unique & sorted values of gA's original values

int gTreeSize[MAXN + 1];
int gBigChild[MAXN + 1];

int gCtRoot;
int gCtParent[MAXN + 1];
bool gCtMark[MAXN + 1];
vector<int> gCtDist[MAXN + 1];  // gCtDist[u] = { distance from u to the nearest ancestor in centroid tree, next centroid, ..., the farthest ancestor }

int dfsSize(int u, int parent = -1) {
    gTreeSize[u] = 1;
    gBigChild[u] = -1;
    for (int v : gE[u]) {
        if (v == parent || gCtMark[v])
            continue;

        gTreeSize[u] += dfsSize(v, u);
        if (gBigChild[u] < 0 || gTreeSize[gBigChild[u]] < gTreeSize[v])
            gBigChild[u] = v;
    }
    return gTreeSize[u];
}

void dfsDistance(int u, int parent = -1, int d = 0) {
    gCtDist[u].push_back(d);
    for (int v : gE[u]) {
        if (v == parent || gCtMark[v])
            continue;
        dfsDistance(v, u, d + 1);
    }
}

int dfsCentroidDecomposition(int u) {
    int n = dfsSize(u);
    while (gBigChild[u] >= 0 && 2 * gTreeSize[gBigChild[u]] > n)
        u = gBigChild[u];

    dfsDistance(u, u);
    gCtMark[u] = true;
    for (int v : gE[u]) {
        if (gCtMark[v])
            continue;
        gCtParent[dfsCentroidDecomposition(v)] = u;
    }

    return u;
}

void doCentroidDecomposition() {
    gCtRoot = dfsCentroidDecomposition(0);
    gCtParent[gCtRoot] = -1;

    for (int i = 0; i < gN; i++)
        reverse(begin(gCtDist[i]), end(gCtDist[i]));    // sort ancestors of centroid tree by distance
}

//-----------------------------------------------------------------------------

vector<pair<int, int>> gAddNodes[MAXN + 1]; // gAddNodes[i] = all descendants of i to add, gAddNodes[i][j] = { distance, value }
vector<pair<int, int>> gSubNodes[MAXN + 1]; // gSubNodes[i] = all descendants of i to subtract, gSubNodes[i][j] = { distance, value }

SimplePersistentSegmentTree<int> gDistSegTree;
vector<pair<int, int>> gAddRootByDist[MAXN + 1]; // roots of persistent segment tree, gAddRootByDist[i][j] = { distance, root }
vector<pair<int, int>> gSubRootByDist[MAXN + 1]; // roots of persistent segment tree, gSubRootByDist[i][j] = { distance, root }

void prepareSearch() {
    doCentroidDecomposition();

    // prepare search by distance

    for (int i = 0; i < gN; i++) {
        int v = i, last = -1, at = 0;
        while (v >= 0) {
            gAddNodes[v].emplace_back(gCtDist[i][at], gA[i]);
            if (last >= 0)
                gSubNodes[last].emplace_back(gCtDist[i][at], gA[i]);

            last = v;
            v = gCtParent[v];
            at++;
        }
    }

    gDistSegTree.build(gM);

    for (int v = 0; v < gN; v++) {
        sort(begin(gAddNodes[v]), end(gAddNodes[v]));   // sort all descendant nodes by distance in a centroid tree
        sort(begin(gSubNodes[v]), end(gSubNodes[v]));   // sort all descendant nodes by distance in a centroid tree

        if (!gAddNodes[v].empty()) {
            auto& nodes = gAddNodes[v];
            int root = gDistSegTree.add(gDistSegTree.root, nodes[0].second, 1);
            for (int i = 1; i < int(nodes.size()); i++) {
                if (nodes[i - 1].first != nodes[i].first)
                    gAddRootByDist[v].emplace_back(nodes[i - 1].first, root);
                root = gDistSegTree.add(root, nodes[i].second, 1);
            }
            gAddRootByDist[v].emplace_back(nodes.back().first, root);
        }
        if (!gSubNodes[v].empty()) {
            auto& nodes = gSubNodes[v];
            int root = gDistSegTree.add(gDistSegTree.root, nodes[0].second, 1);
            for (int i = 1; i < int(nodes.size()); i++) {
                if (nodes[i - 1].first != nodes[i].first)
                    gSubRootByDist[v].emplace_back(nodes[i - 1].first, root);
                root = gDistSegTree.add(root, nodes[i].second, 1);
            }
            gSubRootByDist[v].emplace_back(nodes.back().first, root);
        }
    }
}

// d = max distance, 0 <= u < N, 0 <= d < N
// return the roots in the segment tree, for all ancestors in the centroid tree
vector<pair<int, int>> findSegmentTreeRoots(int u, int maxDist) {
    vector<pair<int, int>> segTrees;

    int v = u, last = -1, at = 0;
    while (v >= 0) {
        int h = maxDist - gCtDist[u][at];
        auto it = lower_bound(begin(gAddRootByDist[v]), end(gAddRootByDist[v]), make_pair(h + 1, 0));
        if (it != begin(gAddRootByDist[v])) {
            it--;
            segTrees.emplace_back(it->second, 1);
        }

        if (last >= 0) {
            it = lower_bound(begin(gSubRootByDist[last]), end(gSubRootByDist[last]), make_pair(h + 1, 0));
            if (it != begin(gSubRootByDist[last])) {
                it--;
                segTrees.emplace_back(it->second, -1);
            }
        }

        last = v;
        v = gCtParent[v];
        at++;
    }

    return segTrees;
}

// 1 <= kth
int findKthValue(vector<pair<int, int>>& segTreeRoots, int kth) {
    int lo = 0, hi = gM - 1;
    while (lo < hi) {
        int qt = 0;
        for (auto& r : segTreeRoots)
            qt += r.second * gDistSegTree.value[gDistSegTree.L[r.first]];

        int mid = (lo + hi) >> 1;
        if (kth <= qt) {
            for (auto& r : segTreeRoots)
                r.first = gDistSegTree.L[r.first];
            hi = mid;
        } else {
            kth -= qt;
            for (auto& r : segTreeRoots)
                r.first = gDistSegTree.R[r.first];
            lo = mid + 1;
        }
    }

    int qt = 0;
    for (auto& r : segTreeRoots)
        qt += r.second * gDistSegTree.value[r.first];

    return kth <= qt ? lo : -1;
}


//-----------------------------------------------------------------------------

// query[i] = { node, max distance, kth },  0 <= node < N, 0 <= d < N, 1 <= kth < N
vector<int> solve(vector<tuple<int, int, int>>& query) {
    // must compress values
    gUniqueA.resize(gN);
    for (int i = 0; i < gN; i++)
        gUniqueA[i] = gA[i];
    sort(gUniqueA.begin(), gUniqueA.end());
    gUniqueA.erase(unique(gUniqueA.begin(), gUniqueA.end()), gUniqueA.end());
    gM = int(gUniqueA.size());

    for (int i = 0; i < gN; i++)
        gA[i] = int(lower_bound(gUniqueA.begin(), gUniqueA.end(), gA[i]) - gUniqueA.begin());

    // prepare
    prepareSearch();

    // search
    vector<int> res(query.size());
    for (int i = 0; i < int(query.size()); i++) {
        int u, d, k;
        tie(u, d, k) = query[i];

        vector<pair<int, int>> roots = findSegmentTreeRoots(u, d);
        auto ans = findKthValue(roots, k);

        res[i] = (ans >= 0) ? gUniqueA[ans] : -1;  // uncompress
    }

    return res;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> gN;

    gA.resize(gN);
    for (int i = 0; i < gN; i++)
        cin >> gA[i];

    for (int i = 1; i < gN; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        gE[u].push_back(v);
        gE[v].push_back(u);
    }

    int qn;
    cin >> qn;

    vector<tuple<int, int, int>> Q;
    Q.reserve(qn);

    for (int i = 0; i < qn; i++) {
        int u, d, k;
        cin >> u >> d >> k;
        u--;
        Q.emplace_back(u, d, k);
    }

    auto ans = solve(Q);

    for (auto& x : ans)
        cout << x << endl;

    return 0;
}
