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

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    vector<pair<int, int>> K(N);
    for (int i = 0; i < N; i++)
        cin >> K[i].first >> K[i].second;

    vector<pair<int, int>> b1(N);  // y - x
    vector<pair<int, int>> b2(N);  // y + x
    for (int i = 0; i < N; i++) {
        b1[i].first = K[i].second - K[i].first;
        b1[i].second = i;
        b2[i].first = K[i].second + K[i].first;
        b2[i].second = i;
    }
    sort(b1.begin(), b1.end());
    sort(b2.begin(), b2.end());

    vector<ll> sumB1Y(N + 1), sumB2Y(N + 1);
    vector<ll> sumB1X(N + 1), sumB2X(N + 1);

    for (int i = 1; i <= N; i++) {
        int j = b1[i - 1].second;
        sumB1X[i] = sumB1X[i - 1] + K[j].first;
        sumB1Y[i] = sumB1Y[i - 1] + K[j].second;

        j = b2[i - 1].second;
        sumB2X[i] = sumB2X[i - 1] + K[j].first;
        sumB2Y[i] = sumB2Y[i - 1] + K[j].second;
    }

    while (Q-- > 0) {
        int x, y;
        cin >> x >> y;

        ll ans = 0;

        int cntB1XL = lower_bound(b1.begin(), b1.end(), make_pair(y - x, -1)) - b1.begin();
        int cntB2XL = lower_bound(b2.begin(), b2.end(), make_pair(y + x, -1)) - b2.begin();
        ans += 1ll * (cntB2XL - cntB1XL) * x + sumB1X[cntB1XL] - sumB2X[cntB2XL];

        int cntB1YL = lower_bound(b1.begin(), b1.end(), make_pair(y - x, -1)) - b1.begin();
        int cntB2YL = lower_bound(b2.begin(), b2.end(), make_pair(y + x, -1)) - b2.begin();
        ans += 1ll * (cntB1YL - (N - cntB2YL)) * y + (sumB2Y[N] - sumB2Y[cntB2YL]) - sumB1Y[cntB1YL];

        cout << ans << endl;
    }

    return 0;
}
