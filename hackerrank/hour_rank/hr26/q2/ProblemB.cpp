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

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int,int>> p(n);         // (x, p)
    for (int i = 0; i < n; i++)
        cin >> p[i].second;

    for (int i = 0; i < n; i++)
        cin >> p[i].first;

    sort(p.begin(), p.end());

    vector<long long> sum(n + 1);
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + p[i].second;
    }

    int m;
    cin >> m;
    vector<int> y(m);
    for (int i = 0; i < m; i++)
        cin >> y[i];

    vector<tuple<int, int, int>> cloud; // (x, left or right, index)
    cloud.reserve(m + 1);
    for (int i = 0; i < m; i++) {
        int r;
        cin >> r;
        if (y[i] - r > p.back().first || y[i] + r < p.front().first)
            continue;
        cloud.emplace_back(y[i] - r, 1, i);
        cloud.emplace_back(y[i] + r + 1, -1, i);
    }
    cloud.emplace_back(INT_MAX, 1, m);
    sort(cloud.begin(), cloud.end());

    long long ans = 0;
    vector<long long> cover(m);
    int L = INT_MIN;

    unordered_set<int> S;
    for (int i = 0; i < (int)cloud.size(); i++) {
        int x = get<0>(cloud[i]);
        int d = get<1>(cloud[i]);
        int idx = get<2>(cloud[i]);

        if (x == L) {
            if (d < 0)
                S.erase(idx);
            else
                S.insert(idx);
        } else if (d > 0) {
            if (S.empty()) {
                int lo = lower_bound(p.begin(), p.end(), pair<int, int>{ L, INT_MIN }) - p.begin();
                int hi = lower_bound(p.begin(), p.end(), pair<int, int>{ x, INT_MIN }) - p.begin();
                ans += sum[hi] - sum[lo];
            } else if (S.size() == 1) {
                int lo = lower_bound(p.begin(), p.end(), pair<int, int>{ L, INT_MIN }) - p.begin();
                int hi = lower_bound(p.begin(), p.end(), pair<int, int>{ x, INT_MIN }) - p.begin();
                cover[*S.begin()] += sum[hi] - sum[lo];
            }
            S.insert(idx);
        } else if (d < 0) {
            if (S.size() == 1) {
                int lo = lower_bound(p.begin(), p.end(), pair<int, int>{ L, INT_MIN }) - p.begin();
                int hi = lower_bound(p.begin(), p.end(), pair<int, int>{ x, INT_MIN }) - p.begin();
                cover[*S.begin()] += sum[hi] - sum[lo];
            }
            S.erase(idx);
        }
        L = x;
    }

    cout << ans + *max_element(cover.begin(), cover.end()) << endl;

    return 0;
}
