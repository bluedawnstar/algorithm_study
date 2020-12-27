#pragma once

// the number of inversion pairs (or the number of swapping in bubble sorting)
// - when it is allowed to swap two elements of an array (not necessarily adjacent) at most once
struct InversionCounterAfterOneSwap {
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

        long long sumRange(int left, int right) const {
            long long res = sum(right);
            if (left > 0)
                res -= sum(left - 1);
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

    // O(N*log(MAX_VALUE))
    static long long count(const vector<int>& A) {
        int N = int(A.size());
        int maxX = *max_element(A.begin(), A.end());

        long long totalInversionCount = 0;
        long long reducibleInversionCount = 0;
        for (int i = 0; i < N; i++) {
            BIT bit(maxX + 1);
            for (int j = i + 1; j < N; j++) {
                if (A[i] > A[j]) {
                    totalInversionCount++;
                    reducibleInversionCount = max(reducibleInversionCount, bit.sumRange(A[j] + 1, A[i] - 1) * 2 + 1);
                }
                bit.add(A[j], 1);
            }
        }

        return totalInversionCount - reducibleInversionCount;
    }
};
