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

typedef long long           ll;
typedef unsigned long long  ull;

template <typename T>
struct DPConvexHullTrickMin {
    struct Line {
        T m, b;     // f(x) = m * x + b

        template <typename U>
        inline T get(U x) const {
            return m * x + b;
        }
    };

    deque<Line> lines;

    // when Xs of queries (not insert) are ascending, x[j] <= x[j + 1]
    // PRECONDITION: m[k] >= m[k + 1]
    void insert(T m, T b) {
        Line l{ m, b };

        while (lines.size() > 1 && ccw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        lines.push_back(l);
    }

    // when Xs of queries (not insert) are descending, x[j] >= x[j + 1]
    // PRECONDITION: m[k] <= m[k + 1]
    void insertReverse(T m, T b) {
        Line l{ m, b };

        while (lines.size() > 1 && cw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        lines.push_back(l);
    }

    template <typename U>
    T query(U x) {
        if (lines.empty())
            return 0;

        while (lines.size() > 1 && lines[0].get(x) >= lines[1].get(x))
            lines.pop_front();

        return lines[0].get(x);
    }

private:
    static T area(const Line& a, const Line& b, const Line& c) {
        T ax = (b.m - a.m);
        T bx = (c.b - a.b);
        T ay = (c.m - a.m);
        T by = (b.b - a.b);
        return ax * bx - ay * by;
    }

    static bool ccw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) >= 0;
    }

    static bool cw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) <= 0;
    }
};

vector<ll> A;
vector<ll> S;
vector<ll> SS;

ll dfs(int L, int R) {
    if (L >= R)
        return LLONG_MIN;
    else if (L + 1 == R)
        return 2ll * A[L] * A[R];

    int mid = L + (R - L) / 2;

    vector<int> idxL(mid - L + 1);
    iota(idxL.begin(), idxL.end(), L);
    sort(idxL.begin(), idxL.end(), [](int l, int r) {
        return S[l] > S[r];
    });

    vector<int> idxR(R - mid);
    iota(idxR.begin(), idxR.end(), mid + 2);
    sort(idxR.begin(), idxR.end(), [](int l, int r) {
        return S[l] < S[r];
    });

    DPConvexHullTrickMin<ll> cht;
    for (int i : idxL)
        cht.insert(2ll * S[i], -(S[i] * S[i] + SS[i]));
    
    ll ans = 0;
    for (int i : idxR) {
        ans = max(ans, (S[i] * S[i] - SS[i]) - cht.query(S[i]));
    }

    ans = max(ans, dfs(L, mid));
    ans = max(ans, dfs(mid + 1, R));

    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    A.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }

    S.resize(N + 1);
    SS.resize(N + 1);
    for (int i = 0; i < N; i++) {
        S[i + 1] = S[i] + A[i];
        SS[i + 1] = SS[i] + A[i] * A[i];
    }

    cout << dfs(0, N - 1) / 2 << endl;

    return 0;
}
