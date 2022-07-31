#pragma once

/*
    A peak is a local maximum or a local minimum.
    - local maximum:  A[i-1] < A[i] > A[i + 1]  ,  0 < i < N - 1
    - local minimum:  A[i-1] > A[i] < A[i + 1]  ,  0 < i < N - 1
 */
struct LongestNonPeakSubsequence {
    struct SegmentTreeMax {
        int       RealN;
        int       N;            // the size of array
        vector<int> tree;       //

        explicit SegmentTreeMax(int size) {
            init(size);
        }

        void init(int size) {
            RealN = size;
            N = size + (size & 1);
            tree = vector<int>(N * 2);
        }

        int query(int index) const {
            return tree[index + N];
        }

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

    // 1 <= A[i] <= |A|
    // O(N*logN)
    static int calcMaxLength(const vector<int>& A) {
        int N = int(A.size());
        SegmentTreeMax incST(N + 2);
        SegmentTreeMax decST(N + 2);

        for (auto x : A) {
            int inc1 = incST.query(0, x - 1);
            int inc2 = incST.query(x);
            int dec1 = decST.query(x + 1, N + 1);
            int dec2 = decST.query(x);

            /*
              <inc1>   <inc2>   <dec2>
                 x
                /       _..._x  \_..._x
               /       /
            */
            int inc = max(max(inc1, inc2), dec2);
            /*
              <dec1>   <dec2>   <inc2>
               \
                \      \_..._x   _..._x
                 x              /
            */
            int dec = max(max(dec1, dec2), inc2);
            incST.update(x, inc + 1);
            decST.update(x, dec + 1);
        }

        return max(incST.query(1, N), decST.query(1, N));
    }
};
