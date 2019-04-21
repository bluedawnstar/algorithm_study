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

#define MAXL    1000

string gS;

bool gDPFull[MAXL + 1][MAXL + 1];
int gDPLeft[MAXL + 1][MAXL + 1];
int gDPRight[MAXL + 1][MAXL + 1];

void build() {
    int N = int(gS.length());

    for (int i = 0; i < N; i++) {
        gDPFull[i][i] = true;
        gDPLeft[i][i] = 1;
        gDPRight[i][i] = 1;
    }

    for (int i = 0; i < N - 1; i++) {
        if (gS[i] == gS[i + 1])
            gDPFull[i][i + 1] = true;
        gDPLeft[i][i + 1] = gDPLeft[i][i] + gDPFull[i][i + 1];
        gDPRight[i][i + 1] = gDPRight[i][i] + gDPFull[i][i + 1];
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (gS[i] == gS[j])
                gDPFull[i][j] = gDPFull[i + 1][j - 1];
            gDPLeft[i][j] = gDPLeft[i][j - 1] + gDPFull[i][j];
            gDPRight[i][j] = gDPRight[i + 1][j] + gDPFull[i][j];
        }
    }
}

int gDP[MAXL + 1][MAXL + 1];

ll solve() {
    int N = int(gS.length());
    if (N == 1)
        return 0;
    else if (N == 2)
        return gS[0] == gS[1];

    build();

    memset(gDP, 0, sizeof(gDP));

    ll res = 0;

    for (int i = 0; i < N - 1; i++) {
        if (gS[i] == gS[i + 1])
            gDP[i][i + 1] = 1;
            res += gDP[i][i + 1];           // x
    }

    for (int i = N - 1; i >= 0; i--) {
        if (i + 2 < N) {
            if (gS[i] == gS[i + 2]) {
                gDP[i][i + 2] = 3;
                res += gDP[i][i + 2];       // x|yx, xy|x, x.x
            }
        }
        for (int j = i + 3; j < N; j++) {
            if (gS[i] == gS[j]) {
                if (gDPFull[i + 1][j - 1])
                    gDP[i][j] = 2;                  // x|y***yx, xy***y|x

                gDP[i][j] += gDP[i + 1][j - 1]      // xy***yx
                          + gDPLeft[i + 1][j - 2]   // xy**y.x
                          + gDPRight[i + 2][j - 1]  // x.y**yx
                          + 1;                      // x.....x

                res += gDP[i][j];
            }
        }
    }

    return res;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> gS;

    cout << solve() << endl;

    return 0;
}
