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

    int T;
    cin >> T;

    while (T-- > 0) {
        string s, t;
        cin >> s >> t;

        if (s.length() > t.length()) {
            cout << -1 << endl;
            continue;
        }

        int cnt = 0;
        vector<int> c2n(26);
        vector<int> nn(26);

        for (int i = 0; i < (int)s.length(); i++) {
            if (++c2n[s[i] - 'a'] == 1)
                ++cnt;
        }

        unordered_map<ll, pair<int, int>> H2S;
        unordered_map<ll, int> H2N;

        const ll q = 100000015277ll;
        const ll d = 3355439ll;

        int M = (int)s.length();

        ll dM = 1;
        for (int i = 0; i < M; i++) {
            dM = (1ll * dM * d) % q;
        }

        int matchedN = 0;

        ll H = 0;
        for (int i = 0; i < M; i++) {
            H = (1ll * H * d + t[i]) % q;

            if (++nn[t[i] - 'a'] == c2n[t[i] - 'a'])
                matchedN++;
        }

        for (int i = M; i <= (int)t.length(); i++) {
            if (matchedN == cnt) {
                if (++H2N[H] == 1)
                    H2S[H] = make_pair(i - M, M);
            }

            if (i < (int)t.length()) {
                if (nn[t[i - M] - 'a']-- == c2n[t[i - M] - 'a'])
                    matchedN--;

                if (++nn[t[i] - 'a'] == c2n[t[i] - 'a'])
                    matchedN++;

                H = (1ll * H * d - 1ll * t[i - M] * dM + t[i]) % q;
                if (H < 0)
                    H += q;
            }
        }

        int ansN = 0;
        string ans;
        for (auto it : H2N) {
            if (it.second > ansN) {
                auto p = H2S[it.first];
                ans = t.substr(p.first, p.second);
                ansN = it.second;
            } else if (it.second == ansN) {
                auto p = H2S[it.first];
                auto temp = t.substr(p.first, p.second);
                if (temp < ans)
                    ans = temp;
            }
        }

        if (ansN == 0)
            cout << -1 << endl;
        else
            cout << ans << endl;
    }

    return 0;
}
