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

string remove(const string& s) {
    int n = (int)s.length();

    vector<int> dp(n + 1);

    stack<int> st;
    for (int i = 0; i < n; i++) {
        if (s[i] == '(')
            st.push(i);
        else if (!st.empty()) {
            int len = i - st.top() + 1;
            st.pop();

            dp[i + 1] = len + dp[i + 1 - len];
        }
    }

    string res;
    res.reserve(n);
    for (int i = (int)s.length() - 1; i >= 0; ) {
        if (dp[i + 1] > 0)
            i -= dp[i + 1];
        else
            res.push_back(s[i--]);
    }
    reverse(res.begin(), res.end());

    if (res.empty())
        return res;

    int L = 0, R = (int)res.length() - 1;
    while (L < R) {
        if (res[L] != '(' || res[R] != ')')
            break;
        L++, R--;
    }

    if (L == 0)
        return res;
    else if (L >= R)
        return string();
    else
        return res.substr(L, R - L + 1);
}

// Complete the fewestOperationsToBalance function below.
int fewestOperationsToBalance(string s) {
    int res = 0;

    s = remove(s);
    if (s.empty())
        return 0;
    else if (s[0] == ')' && s.back() == '(')
        return 2;
    else
        return 1;

    return res;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string s;
    cin >> s;

    cout << fewestOperationsToBalance(s) << endl;

    return 0;
}
