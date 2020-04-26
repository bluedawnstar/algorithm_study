#pragma once

// https://www.hackerearth.com/problem/algorithm/happy-segments-e290faa6/description/
// It checks if for every number 'i' in a subarray, the occurrence of 'i' is equal to X[i]
//  
//  N = length of A
//  M = value range
//  A = value array (0 <= A[i] < M, 0 <= i < N)
//  X = required count of number i in a range (1 <= X[i] <= N, 0 <= i < M)
struct XTimesValueChecker {
    struct BIT {
        vector<int> tree;

        BIT() {
        }

        explicit BIT(int n) : tree(n + 1) {
        }

        void init(int n) {
            tree = vector<int>(n + 1);
        }

        // sum from 0 to pos
        int sum(int pos) const {
            pos++;

            int res = 0;
            while (pos > 0) {
                res += tree[pos];
                pos &= pos - 1;
            }

            return res;
        }

        void add(int pos, int val) {
            pos++;

            while (pos < int(tree.size())) {
                tree[pos] += val;
                pos += pos & -pos;
            }
        }
    };

    // N = length of A
    // M = value range
    // A = value array (0 <= A[i] < M, 0 <= i < N)
    // X = required count of number i in a range (1 <= X[i] <= N, 0 <= i < M)

    static vector<bool> solve(const vector<int>& A, const vector<int>& X, const vector<pair<int, int>>& query) {
        int N = int(A.size());
        int M = int(X.size());

        vector<vector<pair<int, int>>> add(N);
        vector<vector<pair<int, int>>> del(N);

        vector<vector<int>> revA(M);
        for (int i = 0; i < N; i++) {
            if (revA[A[i]].empty())
                revA[A[i]].push_back(-1);
            revA[A[i]].push_back(i);
        }
        for (int i = 0; i < M; i++) {
            if (revA[i].empty())
                continue;

            /*
               ... j      j+1 ...... j+X[i]  j+X[i]+1 ...
             --------------------------------------------------
               ... a ..... a ......... a ...... a ............
                     ^     ^         ^          ^             ^
                     |     |         |          |             |
                    l1 ** r1         |          |             |
                          l2 ****** r2         l3 ********** r3

                     OOOOOOxxxxxxxxxxxxOOOOOOOOOxxxxxxxxxxxxxxxx
                           :           :        :               :
                    l1:   +1          -1       +1              -1
                    r1:   -1          +1       -1              +1
            */

            int size = int(revA[i].size());
            for (int j = 0; j + 1 < size; j++) {
                int l1 = revA[i][j] + 1;
                int r1 = revA[i][j + 1];
                int l2 = revA[i][j + 1];
                int r2 = ((j + X[i] < size) ? revA[i][j + X[i]] : N) - 1;
                if (X[i] != 1) {
                    add[l1].emplace_back(l2, r2);
                    del[r1].emplace_back(l2, r2);
                }
                if (j + X[i] + 1 < size) {
                    int l3 = revA[i][j + X[i] + 1];
                    int r3 = N - 1;
                    add[l1].emplace_back(l3, r3);
                    del[r1].emplace_back(l3, r3);
                }
            }
        }

        vector<bool> ans(query.size());
        vector<vector<pair<int, int>>> Q(N);
        for (int i = 0; i < int(query.size()); i++)
            Q[query[i].first].emplace_back(query[i].second, i);

        BIT bit(N);
        for (int i = 0; i < N; i++) {
            for (auto& it : add[i]) {
                bit.add(it.first, 1);
                bit.add(it.second + 1, -1);
            }

            for (auto& it : Q[i])
                ans[it.second] = (bit.sum(it.first) == 0);

            for (auto& it : del[i]) {
                bit.add(it.first, -1);
                bit.add(it.second + 1, 1);
            }
        }

        return ans;
    }
};
