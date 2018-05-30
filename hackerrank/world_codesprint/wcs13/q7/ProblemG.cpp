#include <memory.h>
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
#include <tuple>
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

template <typename T, typename MergeOp = function<T(T, T)>>
struct DynamicTreeSqrtDecompositionPathQuery {
    int                 N;              // the number of vertex
    int                 sqrtN;          // sqrt(N)

    vector<set<int>>    edges;          // edges (vertex number)

    vector<int>         level;          // level (root is 0)

    vector<int>         parent;         // 
    vector<int>         jump;           // to a ancestor

    vector<T>           values;         // 
    vector<T>           sqrtValues;     // 
    T                   defaultValue;   // 
    MergeOp             mergeOp;        // 

    DynamicTreeSqrtDecompositionPathQuery(MergeOp op, T dflt = T())
        : N(0), sqrtN(0), mergeOp(op), defaultValue(dflt) {
    }

    explicit DynamicTreeSqrtDecompositionPathQuery(int n, MergeOp op, T dflt = T(), int sqrtN = 0)
        : mergeOp(op), defaultValue(dflt) {
        init(n, sqrtN);
    }

    void init(int _n, int _sqrtN = 0) {
        N = _n;
        sqrtN = _sqrtN;
        if (sqrtN <= 0)
            sqrtN = (int)sqrt(N);

        edges = vector<set<int>>(N);
        level.assign(N, 0);
        parent.assign(N, -1);
        jump.assign(N, -1);

        values.assign(N, defaultValue);
        sqrtValues.assign(N, defaultValue);
    }

    //--- build

    // before calling build()
    void addEdge(int u, int v) {
        edges[u].insert(v);
        edges[v].insert(u);
    }

    // before calling build()
    void setValue(int u, const T& value) {
        values[u] = value;
    }

    void build(int root) {
        dfsBuild(root, -1);
    }

    //--- lca

    int lca(int u, int v) const {
        while (jump[u] != jump[v]) {
            if (level[u] > level[v])
                swap(u, v);
            v = jump[v];
        }
        return lcaNaive(u, v);
    }

    // dist >= 0
    int climb(int u, int dist) const {
        int p = jump[u];
        while (p >= 0 && (level[u] - level[p]) <= dist) {
            dist -= (level[u] - level[p]);
            u = p;
            p = jump[u];
        }
        return climbNaive(u, dist);
    }

    // kth >= 1
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    int climbKth(int u, int kth, const function<int(T)>& valueToCountF) const {
        int p = jump[u];
        int d;
        while (p >= 0 && (d = valueToCountF(sqrtValues[u])) < kth) {
            kth -= d;
            u = p;
            p = jump[u];
        }
        return climbKthNaive(u, kth, valueToCountF);
    }

    //--- update

    void update(int u, const T& val) {
        if (values[u] != val) {
            values[u] = val;
            dfsUpdate(u, parent[u]);
        }
    }

    // PRECONDITION: u must be not root
    // PRECONDITION: newParent >= 0
    void changeParent(int u, int newParent) {
        if (newParent == parent[u])
            return;

        if (parent[u] >= 0) {
            edges[u].erase(parent[u]);
            edges[parent[u]].erase(u);
        }

        addEdge(u, newParent);
        parent[u] = newParent;
        dfsBuild(u, newParent);
    }

    //--- query

    T queryToRoot(int u) const {
        T res = sqrtValues[u];
        int p = jump[u];
        while (p >= 0) {
            res = mergeOp(res, sqrtValues[p]);
            p = jump[p];
        }
        return res;
    }

    T queryToAncestor(int u, int ancestor) const {
        T res = defaultValue;
        while (jump[u] != jump[ancestor]) {
            res = mergeOp(res, sqrtValues[u]);
            u = jump[u];
        }
        return mergeOp(res, queryToAncestorNaive(u, ancestor));
    }

    // dist >= 0
    T queryTowardRoot(int u, int dist) const {
        T res = defaultValue;
        int p = jump[u];
        while (p >= 0 && (level[u] - level[p]) <= dist) {
            dist -= (level[u] - level[p]);
            res = mergeOp(res, sqrtValues[u]);
            u = p;
            p = jump[u];
        }
        return mergeOp(res, queryTowardRootNaive(u, dist));
    }

    T query(int u, int v) const {
        if (u == v)
            return values[u];

        if (level[u] > level[v])
            swap(u, v);

        int lc = lca(u, v);
        if (lc == u)
            return queryToAncestor(v, u);

        return mergeOp(queryToAncestor(u, lc), queryTowardRoot(v, level[v] - level[lc] - 1));
    }

    // Use this functions when MergeOp is 'add' (supporting subtraction)
    T queryToAncestorAccumulative(int u, int ancestor) const {
        T res = defaultValue;
        while (jump[u] != jump[ancestor]) {
            res = mergeOp(res, sqrtValues[u]);
            u = jump[u];
        }
        return mergeOp(res, sqrtValues[u] - sqrtValues[ancestor] + values[ancestor]);
    }

    T queryAccumulative(int u, int v) const {
        if (u == v)
            return values[u];

        if (level[u] > level[v])
            swap(u, v);

        int lc = lca(u, v);
        if (lc == u)
            return queryToAncestorAccumulative(v, u);

        return mergeOp(queryToAncestorAccumulative(u, lc), queryToAncestorAccumulative(v, lc)) - values[lc];
    }

protected:
    void dfsBuild(int u, int par) {
        parent[u] = par;
        if (par >= 0)
            level[u] = level[par] + 1;

        if (level[u] % sqrtN == 0) {
            jump[u] = parent[u];
            sqrtValues[u] = values[u];
        } else {
            jump[u] = jump[par];
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);
        }

        for (auto v : edges[u]) {
            if (v != par)
                dfsBuild(v, u);
        }
    }

    void dfsUpdate(int u, int par) {
        if (level[u] % sqrtN == 0)
            sqrtValues[u] = values[u];
        else
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);

        for (auto v : edges[u]) {
            if (v != par)
                dfsUpdate(v, u);
        }
    }

    int lcaNaive(int u, int v) const {
        if (u == v)
            return u;

        if (level[u] > level[v])
            swap(u, v);

        while (level[u] != level[v])
            v = parent[v];

        while (u != v) {
            u = parent[u];
            v = parent[v];
        }
        return u;
    }

    int climbNaive(int u, int dist) const {
        while (u >= 0 && dist-- > 0)
            u = parent[u];
        return u;
    }

    int climbKthNaive(int u, int kth, const function<int(T)>& valueToCountF) const {
        while (u >= 0 && kth > 0) {
            if (valueToCountF(values[u])) {
                if (--kth == 0)
                    break;
            }
            u = parent[u];
        }
        return u;
    }

    T queryToAncestorNaive(int u, int ancestor) const {
        T res = values[u];
        if (u != ancestor) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u != ancestor);
        }

        return res;
    }

    T queryTowardRootNaive(int u, int dist) const {
        T res = values[u];
        if (dist > 0) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u >= 0 && --dist > 0);
        }
        return res;
    }
};

template <typename T, typename MergeOp>
inline DynamicTreeSqrtDecompositionPathQuery<T, MergeOp> makeDynamicTreeSqrtDecompositionPathQuery(MergeOp op, T dfltValue) {
    return DynamicTreeSqrtDecompositionPathQuery<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline DynamicTreeSqrtDecompositionPathQuery<T, MergeOp> makeDynamicTreeSqrtDecompositionPathQuery(int size, MergeOp op, T dfltValue, int sqrtN = 0) {
    return DynamicTreeSqrtDecompositionPathQuery<T, MergeOp>(size, op, dfltValue, sqrtN);
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    auto tree = makeDynamicTreeSqrtDecompositionPathQuery(N, [](int a, int b) { return a + b; }, 0);

    for (int i = 0; i < N; i++) {
        int c;
        cin >> c;
        tree.setValue(i, c);
    }

    for (int i = 1; i < N; i++) {
        int u;
        cin >> u;
        u--;

        tree.addEdge(u, i);
    }

    tree.build(0);

    while (Q-- > 0) {
        char t;
        cin >> t;
        if (t == 'T') {
            int x;
            cin >> x; x--;
            tree.update(x, !tree.values[x]);
        } else if (t == 'C') {
            int u, v;
            cin >> u >> v; u--; v--;
            tree.changeParent(u, v);
        } else {
            int u, v, k;
            cin >> u >> v >> k; u--; v--;

            int lc = tree.lca(u, v);
            int nu = tree.queryToAncestorAccumulative(u, lc);
            int nv = tree.queryToAncestorAccumulative(v, lc);
            int n = nu + nv - (tree.values[lc] ? 1 : 0);
            if (n < k)
                cout << -1 << endl;
            else if (k <= nu) {
                cout << tree.climbKth(u, k, [](int val) { return val; }) + 1 << endl;
            } else {
                k = n - k + 1;
                cout << tree.climbKth(v, k, [](int val) { return val; }) + 1 << endl;
            }
        }
    }

    return 0;
}
