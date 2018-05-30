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

vector<int> gA;
int gM, gK;

inline int modPow(int x, int n, int M) {
    if (n == 0)
        return 1;

    int p = modPow(x, n / 2, M) % M;
    p = int((long long)p * p % M);

    return ((n & 1) == 0) ? p : int((long long)p * x % M);
}

inline int modInvPrime(int a, int M) {
    return modPow(a, M - 2, M);
}

// Complete the howManyGoodSubarrays function below.
long long howManyGoodSubarrays(vector<int> A, int m, int k) {
    long long res = 0;

    if (k == 0) {
        int last0 = -1;
        for (int i = 0; i < (int)A.size(); i++) {
            A[i] %= m;
            if (A[i] == 0) {
                last0 = i;
                res += i + 1;
            } else if (last0 >= 0) {
                res += last0 + 1;
            }
        }
        return res;
    }


    long long x = 1;

    int invK = modInvPrime(k, m);

    unordered_map<int, int> M;
    for (int i = 0; i < (int)A.size(); i++) {
        A[i] %= m;
        if (A[i] == 0) {
            x = 1;
            M.clear();
            continue;
        }

        x = x * A[i] % m;

        int key = int(x * invK % m);
        auto it = M.find(key);
        if (it != M.end())
            res += it->second;

        if (x == k)
            res++;

        ++M[(int)x];
    }

    return res;
}


int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T-- > 0) {
        int N, M, K;
        cin >> N >> M >> K;
        vector<int> A(N);
        for (int i = 0; i < N; i++)
            cin >> A[i];
        cout << howManyGoodSubarrays(A, M, K) << endl;
    }

    return 0;
}
