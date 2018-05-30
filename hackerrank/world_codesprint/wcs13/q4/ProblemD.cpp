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

// Complete the fewestOperationsToBalance function below.
int fewestOperationsToBalance(string s) {
    int L = 0, R = 0;

    for (auto c : s) {
        if (c == '(')
            L++;
        else {
            if (L > 0)
                L--;
            else
                R++;
        }
    }

    if (L == 0 && R == 0)
        return 0;
    else if (L > 0 && R > 0)
        return 2;
    else
        return 1;
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
