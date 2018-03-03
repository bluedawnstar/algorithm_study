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

#define MAXN    300000

template <typename T>
struct BipartiteGraph {
    static const T INF = T(0x3f3f3f3f);

    int srcN;
    int dstN;
    vector<vector<int>> edges;

    BipartiteGraph() : srcN(0), dstN(0) {
    }

    BipartiteGraph(int _srcN, int _dstN) : srcN(_srcN), dstN(_dstN), edges(srcN) {
    }

    void init(int _srcN, int _dstN) {
        srcN = _srcN;
        dstN = _dstN;
        edges = vector<vector<int>>(srcN);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
    }

    // maximum number of matching from srcN to dstN
    // It's faster than Kuhn's algorithm about 3x
    // Hopcroft-Karp : O(E * sqrt(V))
    int calcMaxMatchingHopcroftKarp() {
        match.assign(srcN, -1);
        matchRev.assign(dstN, -1);

        int res = 0;
        while (true) {
            bfsHopcroftKarp();

            vector<bool> visited(srcN);

            int cnt = 0;
            for (int u = 0; u < srcN; u++) {
                if (match[u] < 0 && dfsHopcroftKarp(u, visited))
                    ++cnt;
            }
            if (!cnt)
                break;

            res += cnt;
        }

        return res;
    }

    vector<int>& getLastMaxMatchingEdges() {
        return match;
    }

    //--- Min Vertex Cover ----------------------------------------------------

    // Must call this function after calcMaxMatchingXXX()
    // return (chosen A, chosen B), V(E)
    pair<vector<bool>, vector<bool>> minVertexCover() {
        pair<vector<bool>, vector<bool>> res;
        res.first.assign(srcN, false);
        res.second.assign(dstN, true);

        for (int v = 0; v < dstN; v++)
            if (matchRev[v] < 0)
                res.second[v] = false;

        while (true) {
            bool changed = false;
            for (int u = 0; u < srcN; u++) {
                if (res.first[u])
                    continue;
                for (int v : edges[u]) {
                    if (!res.second[v]) {
                        res.first[u] = true;
                        res.second[match[u]] = false;
                        changed = true;
                    }
                }
            }
            if (!changed)
                break;
        }

        return res;
    }

private:
    vector<int> match;
    vector<int> matchRev;
    vector<int> dist;

    void bfsHopcroftKarp() {
        dist.assign(srcN, -1);

        queue<int> Q;
        for (int u = 0; u < srcN; u++) {
            if (match[u] < 0) {
                Q.push(u);
                dist[u] = 0;
            }
        }

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : edges[u]) {
                int u2 = matchRev[v];
                if (u2 >= 0 && dist[u2] < 0) {
                    dist[u2] = dist[u] + 1;
                    Q.push(u2);
                }
            }
        }
    }

    bool dfsHopcroftKarp(int u, vector<bool>& visited) {
        visited[u] = true;

        for (int v : edges[u]) {
            int u2 = matchRev[v];
            if (u2 < 0 || !visited[u2] && dist[u2] == dist[u] + 1 && dfsHopcroftKarp(u2, visited)) {
                matchRev[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    }
};

struct SegmentTreeLine1D {
    int N;
    // (the number of lines occupying min covered subsegments, total length of min covered subsegments)
    vector<pair<int, int>> tree;
    vector<int> lazy;               // delta to add

    SegmentTreeLine1D(int maxN) : N(maxN), tree(maxN * 4), lazy(maxN * 4) {

    }

    void build() {
        buildSub(1, 0, N - 1);
    }

    void build(int N) {
        this->N = N;
        buildSub(1, 0, N - 1);
    }

    // inclusive
    // delta is normally -1 or 1
    void add(int lineX1, int lineX2, int delta) {
        addSub(1, 0, N - 1, lineX1, lineX2, delta);
    }

    // return N - covered area
    int queryNotCovered() {
        return tree[1].first == 0 ? tree[1].second : 0;
    }

    // total length of 'count' covered segments in (left, right)
    // inclusive
    int query(int left, int right, int count) {
        return querySub(1, 0, N - 1, left, right, count);
    }

    void forEachCovered(int left, int right, const function<void(int)>& f) {
        forEachCoveredSub(1, 0, N - 1, left, right, f);
    }

private:
    // inclusive
    void buildSub(int node, int left, int right) {
        if (left == right) {
            tree[node].second = 1;
        } else {
            int mid = left + (right - left) / 2;

            buildSub(2 * node, left, mid);
            buildSub(2 * node + 1, mid + 1, right);

            merge(node);
        }
    }

    void merge(int node) {
        if (tree[2 * node].first == tree[2 * node + 1].first) {
            tree[node].first = tree[2 * node].first;
            tree[node].second = tree[2 * node].second + tree[2 * node + 1].second;
        } else {
            tree[node] = min(tree[2 * node], tree[2 * node + 1]);
        }
    }

    // inclusive
    void addSub(int node, int left, int right, int lineX1, int lineX2, int delta) {
        if (left == lineX1 && right == lineX2) {
            addTo(node, delta);
        } else {
            push(node);

            int mid = left + (right - left) / 2;

            if (lineX2 <= mid)
                addSub(2 * node, left, mid, lineX1, lineX2, delta);
            else if (lineX1 > mid)
                addSub(2 * node + 1, mid + 1, right, lineX1, lineX2, delta);
            else {
                addSub(2 * node, left, mid, lineX1, mid, delta);
                addSub(2 * node + 1, mid + 1, right, mid + 1, lineX2, delta);
            }

            merge(node);
        }
    }

    void addTo(int node, int delta) {
        lazy[node] += delta;
        tree[node].first += delta;
    }

    void push(int node) {
        if (lazy[node] != 0) {
            addTo(2 * node, lazy[node]);
            addTo(2 * node + 1, lazy[node]);
            lazy[node] = 0;
        }
    }

    // inclusive
    int querySub(int node, int left, int right, int lineX1, int lineX2, int count) {
        if (left == lineX1 && right == lineX2) {
            return tree[node].first == count ? tree[node].second : 0;
        } else {
            push(node);

            int mid = left + (right - left) / 2;

            if (lineX2 <= mid)
                return querySub(2 * node, left, mid, lineX1, lineX2, count);
            else if (lineX1 > mid)
                return querySub(2 * node + 1, mid + 1, right, lineX1, lineX2, count);
            else
                return querySub(2 * node, left, mid, lineX1, mid, count)
                + querySub(2 * node + 1, mid + 1, right, mid + 1, lineX2, count);
        }
    }

    void forEachCoveredSub(int node, int left, int right, int lineX1, int lineX2, const function<void(int)>& f) {
        if (left > right || lineX1 > right || lineX2 < left)
            return;

        if (left == lineX1 && right == lineX2) {
            if (tree[node].first > 0) {
                for (int i = left; i <= right; i++)
                    f(i);
                return;
            } else if (tree[node].first == 0 && tree[node].second == (right - left + 1))
                return;
        }

        if (left < right) {
            push(node);

            int mid = left + (right - left) / 2;
            if (lineX2 <= mid)
                forEachCoveredSub(2 * node, left, mid, lineX1, lineX2, f);
            else if (lineX1 > mid)
                forEachCoveredSub(2 * node + 1, mid + 1, right, lineX1, lineX2, f);
            else {
                forEachCoveredSub(2 * node, left, mid, lineX1, mid, f);
                forEachCoveredSub(2 * node + 1, mid + 1, right, mid + 1, lineX2, f);
            }
        }
    }
};

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<pair<pair<int, int>, pair<int, int>>> R(N);
    for (int i = 0; i < N; i++)
        cin >> R[i].first.first >> R[i].first.second >> R[i].second.first >> R[i].second.second;

    vector<int> X2I(MAXN + 1, -1);
    vector<int> Y2I(MAXN + 1, -1);
    vector<int> I2X;
    vector<int> I2Y;

    I2X.reserve(N + 1);
    I2Y.reserve(N + 1);

    PROFILE_START(0);
    {
        SegmentTreeLine1D treeX(MAXN + 1), treeY(MAXN + 1);
        treeX.build();
        treeY.build();
        for (int i = 0; i < N; i++) {
            treeX.add(R[i].first.first, R[i].second.first, 1);
            treeY.add(R[i].first.second, R[i].second.second, 1);
        }

        treeX.forEachCovered(0, MAXN, [&I2X, &X2I](int x) {
            int xi = (int)I2X.size();
            I2X.push_back(x);
            X2I[x] = xi;
        });
        treeY.forEachCovered(0, MAXN, [&I2Y, &Y2I](int y) {
            int yi = (int)I2Y.size();
            I2Y.push_back(y);
            Y2I[y] = yi;
        });
    }
    PROFILE_STOP(0);

    vector<tuple<int, int, int, int>> evt;
    for (int i = 0; i < N; i++) {
        evt.emplace_back(R[i].first.first, 1, R[i].first.second, R[i].second.second);
        evt.emplace_back(R[i].second.first + 1, -1, R[i].first.second, R[i].second.second);
    }
    sort(evt.begin(), evt.end());

    PROFILE_START(1);
    BipartiteGraph<int> graph((int)I2X.size(), (int)I2Y.size());
    {
        SegmentTreeLine1D tree(MAXN + 1);
        tree.build();

        int lastX = 0, X = 0;

        auto f = [&](int y) {
            for (int x = lastX; x < X; x++)
                graph.addEdge(X2I[x], Y2I[y]);
        };

        for (int i = 0; i < (int)evt.size(); ) {
            X = get<0>(evt[i]);

            if (tree.queryNotCovered() < MAXN + 1)
                tree.forEachCovered(0, MAXN, f);
            lastX = X;

            for (; i < (int)evt.size() && get<0>(evt[i]) == X; i++)
                tree.add(get<2>(evt[i]), get<3>(evt[i]), get<1>(evt[i]));
        }
    }
    PROFILE_STOP(1);
    
    auto ans = graph.calcMaxMatchingHopcroftKarp();
    auto ans2 = graph.minVertexCover();

    vector<int> ansX, ansY;
    for (int i = 0; i < (int)ans2.first.size(); i++) {
        if (!ans2.first[i])
            continue;
        ansX.push_back(I2X[i]);
    }
    for (int i = 0; i < (int)ans2.second.size(); i++) {
        if (!ans2.second[i])
            continue;
        ansY.push_back(I2Y[i]);
    }

    cout << ans << endl;

    cout << ansX.size() << endl;
    if (!ansX.empty()) {
        cout << ansX[0];
        for (int i = 1; i < (int)ansX.size(); i++)
            cout << " " << ansX[i];
    }
    cout << endl;

    cout << ansY.size() << endl;
    if (!ansY.empty()) {
        cout << ansY[0];
        for (int i = 1; i < (int)ansY.size(); i++)
            cout << " " << ansY[i];
    }
    cout << endl;

    return 0;
}
