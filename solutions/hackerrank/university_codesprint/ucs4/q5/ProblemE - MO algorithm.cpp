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

#define MAXN    1000000
#define MAXQ    1000000

int N;
int t[MAXN + 1];
int cnt[MAXN + 1];
int ans[MAXQ + 1];

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    for (int i = 0; i < N; i++)
        cin >> t[i];

    vector<int> A(t, t + N);
    sort(A.begin(), A.end());
    A.erase(unique(A.begin(), A.end()), A.end());

    unordered_map<int, int> M;
    for (int i = 0; i < (int)A.size(); i++) {
        M[A[i]] = i;
    }

    for (int i = 0; i < N; i++)
        t[i] = M[t[i]];

    int q;
    cin >> q;

    vector<tuple<int, int, int>> Q;
    Q.reserve(q);
    for (int i = 0; i < q; i++) {
        int L, R;
        cin >> L >> R;
        Q.emplace_back(L - 1, R - 1, i);
    }

    int blockN = (int)sqrt(Q.size());
    sort(Q.begin(), Q.end(), [blockN](const auto& l, const auto& r) {
        if (get<0>(l) / blockN != get<0>(r) / blockN)
            return get<0>(l) / blockN < get<0>(r) / blockN;
        return get<1>(l) < get<1>(r);
    });

    int L = 1;
    int R = 0;

    int curAns = 0;
    for (auto& q : Q) {
        int left = get<0>(q);
        int right = get<1>(q);

        while (L < left) {
            int n = --cnt[t[L++]];
            if (n == 1)
                curAns++;
            else if (n == 0)
                curAns--;
        }
        while (L > left) {
            int n = ++cnt[t[--L]];
            if (n == 1)
                curAns++;
            else if (n == 2)
                curAns--;
        }

        while (R < right) {
            int n = ++cnt[t[++R]];
            if (n == 1)
                curAns++;
            else if (n == 2)
                curAns--;
        }
        while (R > right) {
            int n = --cnt[t[R--]];
            if (n == 1)
                curAns++;
            else if (n == 0)
                curAns--;
        }

        ans[get<2>(q)] = curAns;
    }

    for (int i = 0; i < q; i++)
        cout << ans[i] << endl;

    return 0;
}
