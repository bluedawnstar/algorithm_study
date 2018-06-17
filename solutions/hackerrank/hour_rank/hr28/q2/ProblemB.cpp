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

bool add(vector<int>& dst, const vector<int>& v1, const vector<int>& v2, int Q) {
    for (int i = 0; i < 10; i++) {
        dst[i] = v1[i] + v2[i];
        if (dst[i] > Q)
            return false;
    }
    return true;
}

static int popcount(unsigned x) {
#ifndef __GNUC__
    return (int)__popcnt(x);
#else
    return __builtin_popcount(x);
#endif
}

bool possible[1 << 20];
int M[1 << 20][20];

int maximumElegance(int q, vector<string> s, vector<int> b) {
    int N = (int)s.size();
    vector<vector<int>> C(N, vector<int>(10));
    for (int i = 0; i < N; i++) {
        for (auto c : s[i])
            ++C[i][c - '0'];
    }

    int ans = 0;

    vector<vector<int>> setSize(1 << N, vector<int>(10));
    possible[0] = true;
    for (int i = 1; i < (1 << N); i++) {
        int j = i & (i - 1);
        if (!possible[j])
            continue;

        int k = popcount((unsigned)((i & -i) - 1));
        possible[i] = add(setSize[i], setSize[j], C[k], q);
        if (!possible[i])
            continue;

        for (int t1 = i; t1; t1 &= t1 - 1) {
            int e1 = popcount((unsigned)((t1 & -t1) - 1));
            int p = i ^ (t1 & -t1);
            if (p == 0) {
                M[i][e1] = b[e1];
            } else {
                for (int t0 = p; t0; t0 &= t0 - 1) {
                    int e0 = popcount((unsigned)((t0 & -t0) - 1));
                    M[i][e1] = max(M[i][e1], M[p][e0] + (b[e0] ^ b[e1]));
                }
            }
            ans = max(ans, M[i][e1]);
        }
    }

    return ans;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    
    vector<int> b(n);
    for (int i = 0; i < n; i++)
        cin >> b[i];

    vector<string> s(n);
    for (int i = 0; i < n; i++)
        cin >> s[i];

    cout << maximumElegance(q, s, b) << endl;

    return 0;
}
