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

class UnionFind {
public:
    UnionFind() {
    }

    UnionFind(int N) : parent(N, 0), rank(N, 0) {
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }

    void init(int N) {
        parent.resize(N);
        rank.assign(N, 0);
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }


    int findSet(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = findSet(parent[x]);
    }

    int unionSet(int x, int y) {
        int xset = findSet(x);
        int yset = findSet(y);
        if (xset == yset)
            return xset;

        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
            return yset;
        } else {
            parent[yset] = xset;
            if (rank[xset] == rank[yset])
                rank[xset]++;
            return xset;
        }
    }

private:
    vector<int> parent;
    vector<int> rank;
};


#define MAXN    100000

int N, M, A, B, F, S, T;
vector<pair<string, int>> P;

vector<pair<int, int>> E;

struct Group {
    int total;
    int first;
    int second;
    int third;

    Group() {
        total = 0;
        first = 0;
        second = 0;
        third = 0;
    }
};

UnionFind UF(MAXN + 1);
Group G[MAXN + 1];
bool used[MAXN + 1];

void merge(int u, int v) {
    int ug = UF.findSet(u);
    int vg = UF.findSet(v);

    if (ug == vg)
        return;

    int t = G[ug].total + G[vg].total;
    int n1 = G[ug].first + G[vg].first;
    int n2 = G[ug].second + G[vg].second;
    int n3 = G[ug].third + G[vg].third;

    if (t > B || n1 > F || n2 > S || n3 > T)
        return;

    int uvg = UF.unionSet(u, v);

    G[uvg].total = t;
    G[uvg].first = n1;
    G[uvg].second = n2;
    G[uvg].third = n3;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> A >> B >> F >> S >> T;

    P.resize(N);
    unordered_map<string, int> mm;
    for (int i = 0; i < N; i++) {
        cin >> P[i].first >> P[i].second;
        --P[i].second;
        mm[P[i].first] = i;
        G[i].total = 1;
        switch (P[i].second) {
        case 0:
            G[i].first = 1;
            break;
        case 1:
            G[i].second = 1;
            break;
        case 2:
            G[i].third = 1;
            break;
        }
    }

    string p1, p2;
    for (int i = 0; i < M; i++) {
        cin >> p1 >> p2;
        int u = mm[p1];
        int v = mm[p2];
        E.emplace_back(u, v);
    }

    for (int i = 0; i < M; i++) {
        merge(E[i].first, E[i].second);
    }

    int maxG = 0;
    for (int i = 0; i < (int)N; i++) {
        if (UF.findSet(i) != i)
            continue;
        maxG = max(maxG, G[i].total);
    }

    if (maxG < A) {
        cout << "no groups" << endl;
        return 0;
    }

    vector<bool> gr(N);
    for (int i = 0; i < (int)N; i++) {
        if (UF.findSet(i) != i || G[i].total != maxG)
            continue;
        gr[i] = true;
    }

    vector<string> ans;
    ans.reserve(N);
    for (int i = 0; i < (int)N; i++) {
        if (gr[UF.findSet(i)])
            ans.push_back(P[i].first);
    }
    sort(ans.begin(), ans.end());

    for (auto& s : ans)
        cout << s << endl;

    return 0;
}
