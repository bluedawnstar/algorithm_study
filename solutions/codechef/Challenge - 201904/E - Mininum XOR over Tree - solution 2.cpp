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

template <typename T, typename IntT = int, typename GetOp = function<IntT(T)>, typename MergeOp = function<T(T,T)>>
struct PersistentIntTrie {
    struct Node {
        int L;
        int R;
        T   value;
    };

    int         bitSize;
    GetOp       getOp;
    MergeOp     mergeOp;

    int nodeN;
    vector<Node> nodes;

    PersistentIntTrie(int bitSize, GetOp getOp, MergeOp mergeOp)
        : bitSize(bitSize), getOp(getOp), mergeOp(mergeOp), nodeN(0) {
    }

    void clear() {
        nodeN = 0;
        nodes.clear();
    }

    //--- build a new trie

    // PRECONDITION: v must be sorted
    int build(const T v[], int n) {
        return dfsBuild(v, 0, n - 1, IntT(1) << (bitSize - 1));
    }

    // PRECONDITION: v must be sorted
    int build(const vector<T>& v) {
        return build(v.data(), int(v.size()));
    }

    //--- add

    // if there is no node, then set root as -1
    int add(int root, const T& value) {
        return dfsAdd(root, value, IntT(1) << (bitSize - 1));
    }


    bool exist(int root, IntT key) const {
        int x = root;
        for (IntT bit = IntT(1) << (bitSize - 1); bit && x >= 0; bit >>= 1) {
            if (key & bit)
                x = nodes[x].R;
            else
                x = nodes[x].L;
        }

        return x >= 0;
    }

    // check if key exists in [L, R]
    // - rootL = root of time (L - 1)
    // - rootR = root of time R
    bool exist(int rootL, int rootR, IntT key) const {
        int xL = rootL;
        int xR = rootR;
        for (IntT bit = IntT(1) << (bitSize - 1); bit && xL < xR; bit >>= 1) {
            if (key & bit) {
                xL = (xL >= 0) ? nodes[xL].R : -1;
                xR = (xR >= 0) ? nodes[xR].R : -1;
            } else {
                xL = (xL >= 0) ? nodes[xL].L : -1;
                xR = (xR >= 0) ? nodes[xR].L : -1;
            }
        }
        return xL < xR;
    }

    pair<IntT, T> findMaxXor(int root, IntT k) const {
        int x = root;

        IntT res = 0;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            if ((nodes[x].L < 0) || (nodes[x].R >= 0 && !(k & bit))) {
                x = nodes[x].R;
                res |= bit;
            } else {
                x = nodes[x].L;
            }
        }

        return make_pair(res, nodes[x].value);
    }

    // find a maximum xor value with k in [L, R]
    // - rootL = root of time (L - 1)
    // - rootR = root of time R
    pair<IntT, T> findMaxXor(int rootL, int rootR, IntT k) const {
        int xL = rootL;
        int xR = rootR;

        IntT res = 0;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            int xLL = (xL >= 0) ? nodes[xL].L : -1;
            int xLR = (xL >= 0) ? nodes[xL].R : -1;
            int xRL = (xR >= 0) ? nodes[xR].L : -1;
            int xRR = (xR >= 0) ? nodes[xR].R : -1;
            if (xLL >= xRL || (xLR < xRR && !(k & bit))) {
                xL = xLR;
                xR = xRR;
                res |= bit;
            } else {
                xL = xLL;
                xR = xRL;
            }
        }

        return make_pair(res, nodes[xR].value);
    }

private:
    int alloc() {
        if (nodeN >= int(nodes.size()))
            nodes.resize(nodeN == 0 ? 1024 : nodeN * 2);

        nodes[nodeN].L = -1;
        nodes[nodeN].R = -1;

        return nodeN++;
    }

    int dfsBuild(const T v[], int L, int R, IntT bit) {
        int x = alloc();

        if (bit == 0) {
            nodes[x].value = v[L];
            return x;
        }

        int mid;
        if ((getOp(v[L]) & bit) == (getOp(v[R]) & bit)) {
            mid = (getOp(v[L]) & bit) ? L : R + 1;
        } else {
            int i = L, j = R;
            while (i <= j) {
                int mid = (i + j) >> 1;
                if (getOp(v[mid]) & bit)
                    j = mid - 1;
                else
                    i = mid + 1;
            }
            mid = i;
        }

        if (L < mid) {
            auto nodeL = dfsBuild(v, L, mid - 1, bit >> 1);
            nodes[x].L = nodeL;
        }
        if (mid <= R) {
            auto nodeR = dfsBuild(v, mid, R, bit >> 1);
            nodes[x].R = nodeR;
        }

        return x;
    }

    int dfsAdd(int x, const T& value, IntT bit) {
        int x2 = alloc();

        if (bit == 0) {
            if (x < 0)
                nodes[x2].value = value;
            else
                nodes[x2].value = mergeOp(nodes[x].value, value);
        } else {
            if (getOp(value) & bit) {
                auto nodeR = dfsAdd(x >= 0 ? nodes[x].R : -1, value, bit >> 1);
                nodes[x2].L = (x >= 0) ? nodes[x].L : -1;
                nodes[x2].R = nodeR;
            } else {
                auto nodeL = dfsAdd(x >= 0 ? nodes[x].L : -1, value, bit >> 1);
                nodes[x2].L = nodeL;
                nodes[x2].R = (x >= 0) ? nodes[x].R : -1;
            }
        }

        return x2;
    }
};

#define MAXN    200000
#define MAXW    (1 << 20)
#define MAXK    (1 << 20)

int gN;
int gW[MAXN + 1];
vector<int> gE[MAXN + 1];

int gCurrTime = 0;
pair<int, int> gVisit[MAXN + 1];

int tV[MAXN + 1];

PersistentIntTrie<int> gTrie(20,
    [](int x) { return gW[x]; },
    [](int a, int b) {
    return min(a, b);
});
int gTrieRoots[MAXN + 2];

void dfsBuild1(int u, int p) {
    tV[gCurrTime] = u;
    gVisit[u].first = gCurrTime++;
    for (auto v : gE[u]) {
        if (v == p)
            continue;
        dfsBuild1(v, u);
    }
    gVisit[u].second = gCurrTime;
}

void dfsBuild2(int u, int p) {
    vector<pair<int,int>> sub;
    for (auto v : gE[u]) {
        if (v == p)
            continue;
        dfsBuild2(v, u);

        sub.emplace_back(gVisit[v].second - gVisit[v].first, v);
    }
    sort(sub.begin(), sub.end(), greater<pair<int,int>>());

    int root = -1;
    if (!sub.empty()) {
        root = gTrieRoots[sub[0].second];
        for (int i = 1; i < int(sub.size()); i++) {
            int L = gVisit[sub[i].second].first;
            int R = gVisit[sub[i].second].second;
            for (; L < R; L++)
                root = gTrie.add(root, tV[L]);
        }
    }
    gTrieRoots[u] = gTrie.add(root, u);
}

void build() {
    dfsBuild1(0, -1);

    gTrie.clear();
    dfsBuild2(0, -1);
}

#if 1
namespace FastIO {
    char gInBuff[1 << 25]; // sizeof in varied in problem
    const char* gInPtr;

    void init() {
        gInPtr = gInBuff;
        auto size = fread(gInBuff, 1, sizeof(gInBuff) - 4, stdin);
        gInBuff[size] = 0;
    }

    int readInt() {
        unsigned u = 0, s = 0;

        while (*gInPtr && *gInPtr <= 32)
            ++gInPtr;

        if (*gInPtr == '-') {
            s = ~s;
            ++gInPtr;
        } else if (*gInPtr == '+') {
            ++gInPtr;
        }

        while (*gInPtr >= '0' && *gInPtr <= '9')
            u = (u << 3) + (u << 1) + (*gInPtr++ - '0');

        return static_cast<int>((u ^ s) + !!s);
    }
}

int main(void) {
    FastIO::init();

    int T = FastIO::readInt();

    while (T-- > 0) {
        int Q;

        gN = FastIO::readInt();
        Q = FastIO::readInt();

        for (int i = 0; i < gN; i++) {
            gE[i].clear();
            gW[i] = FastIO::readInt();
        }

        for (int i = 1; i < gN; i++) {
            int u, v;
            u = FastIO::readInt();
            v = FastIO::readInt();
            u--;
            v--;
            gE[u].push_back(v);
            gE[v].push_back(u);
        }

        build();

        int lastX = 0, lastV = 0;
        while (Q-- > 0) {
            int a, b;
            a = FastIO::readInt();
            b = FastIO::readInt();

            int v = (a ^ lastV) - 1;
            int k = b ^ lastX;

            auto r = gTrie.findMaxXor(gTrieRoots[v], k);
            lastV = r.second + 1;
            lastX = r.first ^ k;

            printf("%d %d\n", lastV, lastX);
        }
    }

    return 0;
}
#else
int main(void) {
    int T;
    cin >> T;

    while (T-- > 0) {
        int Q;

        cin >> gN >> Q;

        for (int i = 0; i < gN; i++) {
            gE[i].clear();
            cin >> gW[i];
        }

        for (int i = 1; i < gN; i++) {
            int u, v;
            cin >> u >> v;
            u--;
            v--;
            gE[u].push_back(v);
            gE[v].push_back(u);
        }

        build();

        int lastX = 0, lastV = 0;
        while (Q-- > 0) {
            int a, b;
            cin >> a >> b;

            int v = (a ^ lastV) - 1;
            int k = b ^ lastX;

            int L = gVisit[v].first;
            int R = gVisit[v].second;

            auto r = gTrie.findMaxXor(gTrieRoots[v], k);
            lastV = r.second + 1;
            lastX = r.first ^ k;

            cout << lastV << ' ' << lastX << endl;
        }
    }

    return 0;
}
#endif
