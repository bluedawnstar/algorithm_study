#pragma once

struct InversionCounter2 {
    struct BIT {
        vector<long long> tree;

        explicit BIT(int n) : tree(n + 1) {
        }

        long long sum(int pos) const {
            pos++;

            long long res = 0;
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

    static long long count(vector<int>& A) {
        int maxN = *max_element(A.begin(), A.end());
        BIT bit(maxN);

        long long res = 0;
        for (int i = int(A.size()) - 1; i >= 0; i--) {
            res += bit.sum(A[i] - 1);
            bit.add(A[i], 1);
        }

        return res;
    }
};
