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

#define MAXN    200000

int gN;

ll gW[MAXN + 10];
ll gG[MAXN + 10];
ll gP[MAXN + 10];

struct SparseTableLCA {
    int                 mN;         // the number of vertex
    int                 mLogN;      // 
    vector<vector<int>> mP;         // mP[0][n] points to the parent
                                    // parent & acestors
    SparseTableLCA() {
        mN = 0;
        mLogN = 0;
    }

    SparseTableLCA(int N) {
        init(N);
    }

    SparseTableLCA(const vector<int>& A) {
        init(A);
    }

    SparseTableLCA(const int A[], int N) {
        init(A, N);
    }


    void init(int N) {
        mN = N;
        mLogN = (int)log2(N) + 1;
        mP.resize(mLogN);
        for (int i = 0; i < mLogN; i++)
            mP[i].resize(mN);
    }

    void init(const vector<int>& A) {
        init((int)A.size());
        copy(A.begin(), A.end(), mP[0].begin());
    }

    void init(const int A[], int N) {
        init(N);
        copy(A, A + N, mP[0].begin());
    }


    void build() {
        for (int i = 1; i < mLogN; i++) {
            for (int j = 0; j < mN; j++) {
                int pp = mP[i - 1][j];
                mP[i][j] = pp < 0 ? pp : mP[i - 1][pp];
            }
        }
    }

    int climb(int x, int dist) {
        if (dist <= 0)
            return x;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                x = mP[i][x];
            dist >>= 1;
        }

        return x;
    }

    // PRECONDITION: The depth from the root of A and B is the same.
    int findLCA(int A, int B) {
        if (A == B)
            return A;

        for (int i = mLogN - 1; i >= 0; i--) {
            if (mP[i][A] >= 0 && mP[i][A] != mP[i][B]) {
                A = mP[i][A];
                B = mP[i][B];
            }
        }

        return mP[0][A];
    }
};

struct SparseTableMin {
    int N;
    vector<vector<ll>> value;
    vector<int> H;

    template <typename U>
    SparseTableMin(const U& a, int n) {
        this->N = n;

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < (int)H.size(); i++)
            H[i] = H[i >> 1] + 1;

        value.resize(H.back() + 1, vector<ll>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < (int)value.size(); i++) {
            vector<ll>& prev = value[i - 1];
            vector<ll>& curr = value[i];
            for (int v = 0; v < n; v++) {
                if (v + (1 << (i - 1)) < n)
                    curr[v] = min(prev[v], prev[v + (1 << (i - 1))]);
                else
                    curr[v] = prev[v];
            }
        }
    }

    ll query(int left, int right) {
        right++;
        if (right <= left)
            return 0; //INT_MAX;

        int k = H[right - left];
        vector<ll>& mink = value[k];
        return min(mink[left], mink[right - (1 << k)]);
    }
};

int main(void) {
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);

    int Q;

    scanf("%d %d", &gN, &Q);
    for (int i = 1; i < gN; i++)
        scanf("%lld", &gW[i - 1]);
    for (int i = 0; i < gN; i++)
        scanf("%lld %lld", &gG[i], &gP[i]);

    SparseTableMin rmqP(gP, gN);

    vector<ll> sumG(gN);
    for (int i = 1; i < gN; i++)
        sumG[i] = sumG[i - 1] + gG[i - 1] - gW[i - 1];

    SparseTableLCA nextG(gN + 1);
    vector<ll> needG(gN + 1);
    {
        stack<int> stk;
        nextG.mP[0][gN] = gN;
        for (int i = gN - 1; i >= 0; i--) {
            while (!stk.empty() && sumG[stk.top()] >= sumG[i])
                stk.pop();
            if (stk.empty()) {
                nextG.mP[0][i] = gN;
                needG[i] = 0;
            } else {
                nextG.mP[0][i] = stk.top();
                needG[i] = needG[stk.top()] + sumG[i] - sumG[stk.top()];
            }
            stk.push(i);
        }
    }
    nextG.build();

    vector<ll> needC(gN + 1);
    for (int i = gN - 1; i >= 0; i--) {
        ll minP = rmqP.query(i, nextG.mP[0][i] - 1);
        int minX = i;   // minP가 유지되는 구간의 끝
        for (int j = nextG.mLogN - 1; j >= 0; j--) {
            if (rmqP.query(i, nextG.mP[j][minX] - 1) >= minP)
                minX = nextG.mP[j][minX];
        }

        needC[i] = needC[minX] + (needG[i] - needG[minX]) * minP;
    }


    vector<ll> A(Q);
    vector<vector<pair<int, int>>> qry(gN);

    vector<int> qryIdx;
    qryIdx.reserve(gN);

    for (int i = 0; i < Q; i++) {
        int L, R;
        scanf("%d %d", &L, &R);
        L--; R--;

        if (L < R) {
            if (qry[L].empty())
                qryIdx.push_back(L);
            qry[L].emplace_back(R, i);
        }
    }
    sort(qryIdx.begin(), qryIdx.end());

    for (int i = 0; i < gN; i++) {
        sort(qry[i].begin(), qry[i].end(), [](const pair<int, int>& l, const pair<int, int>& r) {
            return l.first < r.first;
        });
    }

    for (int i = 0; i < (int)qryIdx.size(); i++) {
        int L = qryIdx[i];
        for (int j = 0; j < (int)qry[L].size(); j++) {
            int R = qry[L][j].first;
            int fixedR = L;
            for (int k = nextG.mLogN - 1; k >= 0; k--) {
                if (nextG.mP[k][fixedR] <= R)
                    fixedR = nextG.mP[k][fixedR];
            }

            if (L == fixedR) {
                A[qry[L][j].second] = 0;
                continue;
            }

            ll minP = rmqP.query(L, fixedR - 1);
            int minX = L;
            for (int k = nextG.mLogN - 1; k >= 0; k--) {
                if (rmqP.query(L, nextG.mP[k][minX] - 1) >= minP)
                    minX = nextG.mP[k][minX];
            }

            A[qry[L][j].second] = needC[L] - needC[minX] + (needG[minX] - needG[fixedR]) * minP;
        }
    }

    for (int i = 0; i < Q; i++)
        printf("%lld\n", A[i]);

    return 0;
}
