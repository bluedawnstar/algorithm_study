#pragma once

/*
  Longest Zigzag Subsequence
  https://www.hackerearth.com/challenges/competitive/october-circuits-20/algorithm/the-alones-array-459a5370/

  1. input
     A = { A[0], A[1], A[2], ..., A[N - 1] },  0 <= A[i] <= maxX

  2. result
     longest zigzag subsequence = { A[i0], A[i1], A[i2], ... A[im] }
       - i0 < i1 < i2 < ... < im
       - A[i0] >= A[i1] <= A[i2] >= A[i3] ... A[im]
*/
struct LongestZigzagSubsequence {
    struct SegmentTreeMax {
        int       RealN;
        int       N;            // the size of array
        vector<int> tree;       //

        SegmentTreeMax() : RealN(0), N(0), tree() {
        }

        explicit SegmentTreeMax(int size) {
            init(size);
        }

        void init(int size) {
            RealN = size;
            N = size + (size & 1);
            tree.assign(N * 2, 0);
        }

        int query(int index) const {
            return tree[index + N];
        }

        // inclusive
        int query(int left, int right) const {
            int resL = 0;
            int resR = 0;

            for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1) {
                if (L & 1)
                    resL = max(resL, tree[L++]);
                if (R & 1)
                    resR = max(tree[--R], resR);
            }

            return max(resL, resR);
        }

        void update(int index, int newValue) {
            tree[index + N] = newValue;

            for (int i = (index + N) >> 1; i > 0; i >>= 1)
                tree[i] = max(tree[i << 1], tree[(i << 1) | 1]);
        }
    };

    // O(NlogN)
    static int calcLength(const int A[], int N) {
        int maxX = *max_element(A, A + N);

        SegmentTreeMax segTree1(maxX + 1);
        SegmentTreeMax segTree2(maxX + 1);

        int res = 0;

        for (int i = 0; i < N; i++) {
            int mx1 = segTree1.query(0, A[i]) + 1;
            int mx2 = segTree2.query(A[i], maxX) + 1;

            if ((mx1 & 1) == 0)
                mx1--;
            if (mx2 & 1)
                mx2--;

            res = max(res, max(mx1, mx2));

            segTree1.update(A[i], mx2);
            segTree2.update(A[i], mx1);
        }

        return res;
    }

    // O(NlogN)
    static int calcLength(const vector<int>& A) {
        return calcLength(A.data(), int(A.size()));
    }
};
