#include <vector>

using namespace std;

// related question: https://www.hackerrank.com/contests/101hack49/challenges/bigger-arrays

template <typename T>
struct SqrtDecompositionLazy {
    static const T INIT_VALUE = 0;          //TODO: modify

    static T mergeOp(T a, T b) {
        return a + b;                       //TODO: modify
    }

    static T mergeBlockOp(T x, int n) {     //TODO: modify
        return x * n;
    }

    //------------------------------------------------

    int N;
    int blockSize;
    int blockCount;

    vector<T> values;
    vector<T> blockValues;
    vector<T> blockLazy;
    vector<bool> blockLazyExist;

    SqrtDecompositionLazy() {
        N = 0;
        blockSize = 0;
        blockCount = 0;
    }

    SqrtDecompositionLazy(int n) {
        init(n);
    }

    SqrtDecompositionLazy(const vector<T>& v) {
        init(v);
    }

    SqrtDecompositionLazy(const T v[], int n) {
        init(v, n);
    }

    void init(int n) {
        N = n;
        blockSize = (int)sqrt(n);
        blockCount = (N + blockSize - 1) / blockSize;

        values.resize(n);
        blockValues.resize(blockCount);
        blockLazy.resize(blockCount);
        blockLazyExist.resize(blockCount);
    }

    void init(const vector<T>& v) {
        init((int)v.size());
        copy(v.begin(), v.end(), values.begin());
        build();
    }

    void init(const T v[], int n) {
        init(n);
        copy(v, v + n, values.begin());
        build();
    }


    void build() {
        for (int i = 0; i < blockCount; i++)
            updateBlockValue(i);
    }

    void update(int l, int r, T x) {
        int blockL = l / blockSize;
        int blockR = r / blockSize;
        int blockCL = (l + blockSize - 1) / blockSize;
        int blockCU = (r + 1) / blockSize;

        if (blockL == blockR) {
            if (getBlockSize(blockL) == (r - l + 1)) {
                blockValues[blockL] = mergeBlockOp(x, r - l + 1);

                blockLazy[blockL] = x;
                blockLazyExist[blockL] = true;
            } else {
                if (blockLazyExist[blockL]) {
                    updateValue(blockL * blockSize, l, blockLazy[blockL]);
                    updateValue(r + 1, min(N, (blockL + 1) * blockSize), blockLazy[blockL]);
                    blockLazyExist[blockL] = false;
                }
                updateValue(l, r + 1, x);
                updateBlockValue(blockL);
            }
        } else {
            if (blockL != blockCL) {
                if (blockLazyExist[blockL]) {
                    updateValue(blockL * blockSize, l, blockLazy[blockL]);
                    blockLazyExist[blockL] = false;
                }
                updateValue(l, blockCL * blockSize, x);
                updateBlockValue(blockL);
            }

            T blockX = mergeBlockOp(x, blockSize);

            while (blockCL < blockCU) {
                blockValues[blockCL] = blockX;

                blockLazy[blockCL] = x;
                blockLazyExist[blockCL] = true;
                blockCL++;
            }

            if (blockCU == blockR) {
                if (blockLazyExist[blockR]) {
                    updateValue(r + 1, min(N, (blockR + 1) * blockSize), blockLazy[blockR]);
                    blockLazyExist[blockR] = false;
                }
                updateValue(blockR * blockSize, r + 1, x);
                updateBlockValue(blockR);
            }
        }
    }

    T query(int l, int r) {
        T res = INIT_VALUE;

        int blockL = l / blockSize;
        int blockR = r / blockSize;
        int blockCL = (l + blockSize - 1) / blockSize;
        int blockCU = (r + 1) / blockSize;

        if (blockL == blockR) {
            if (blockLazyExist[blockL])
                res = mergeBlockOp(blockLazy[blockL], r - l + 1);
            else
                res = mergeValue(l, r + 1);
        } else {
            if (blockL != blockCL) {
                if (blockLazyExist[blockL])
                    res = mergeBlockOp(blockLazy[blockL], blockCL * blockSize - l);
                else
                    res = mergeValue(l, blockCL * blockSize);
            }

            while (blockCL < blockCU) {
                res = mergeOp(res, blockValues[blockCL]);
                blockCL++;
            }

            if (blockCU == blockR) {
                if (blockLazyExist[blockR])
                    res = mergeOp(res, mergeBlockOp(blockLazy[blockR], r % blockSize + 1));
                else
                    res = mergeOp(res, mergeValue(blockR * blockSize, r + 1));
            }
        }

        return res;
    }

private:
    void updateBlockValue(int block) {
        blockValues[block] = INIT_VALUE;

        int l = block * blockSize;
        int r = min(N, l + blockSize);
        while (l < r)
            blockValues[block] = mergeOp(blockValues[block], values[l++]);
    }

    T mergeValue(int l, int r) {
        T res = INIT_VALUE;

        while (l < r)
            res = mergeOp(res, values[l++]);

        return res;
    }

    void updateValue(int l, int r, T x) {
        while (l < r)
            values[l++] = x;
    }

    int getBlockSize(int block) {
        return ((N / blockSize) == block) ? (N % blockSize) : blockSize;
    }
};


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include <algorithm>
#include "../tree/treeSegmentTree.h"
#include "../tree/treeSegmentTreeLazy.h"

void testSqrtDecomposition() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Sqrt Decomposition ------------------------" << endl;
    {
        vector<int> in{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        auto segTree = makeSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTreeLazy = makeSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
        SqrtDecompositionLazy<int> sqrtDecom(in);

        for (int i = 0; i < 10000; i++) {
            if (rand() % 2) {
                int L = rand() % (int)in.size();
                int R = rand() % (int)in.size();
                if (L > R)
                    swap(L, R);
                assert(segTree.query(L, R) == segTreeLazy.query(L, R));
                assert(segTree.query(L, R) == sqrtDecom.query(L, R));
            } else {
                int L = rand() % (int)in.size();
                int R = rand() % (int)in.size();
                int x = rand() % 100;
                if (L > R)
                    swap(L, R);

                int a = segTree.updateRange(L, R, x);
                int b = segTreeLazy.updateRange(L, R, x);
                sqrtDecom.update(L, R, x);
                assert(a == b);
            }
        }
    }
    cout << "OK!" << endl;

    cout << "*** Speed Test ***" << endl;
    {
        int T = 100000;
        int N = 1000000;
        vector<int> in(N);

        auto segTree = makeSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTreeLazy = makeSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
        SqrtDecompositionLazy<int> sqrtDecom(in);

        vector<tuple<int, int, int, int>> Q;
        for (int i = 0; i < T; i++) {
            if (rand() % 2) {
                int L = rand() % (int)in.size();
                int R = rand() % (int)in.size();
                if (L > R)
                    swap(L, R);
                Q.emplace_back(1, L, R, 0);
            } else {
                int L = rand() % (int)in.size();
                int R = rand() % (int)in.size();
                int x = rand() % 100;
                if (L > R)
                    swap(L, R);
                Q.emplace_back(0, L, R, x);
            }
        }

        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);

                int a = segTree.query(L, R);
                int b = segTreeLazy.query(L, R);
                int c = sqrtDecom.query(L, R);
                if (a != b)
                    cout << "It'll Never be shown!" << endl;
                if (a != c)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree.updateRange(L, R, x);
                segTreeLazy.updateRange(L, R, x);
                sqrtDecom.update(L, R, x);
            }
        }
        cout << "OK!" << endl;

        PROFILE_START(0);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (segTree.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree.updateRange(L, R, x);
            }
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (segTreeLazy.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTreeLazy.updateRange(L, R, x);
            }
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (sqrtDecom.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                sqrtDecom.update(L, R, x);
            }
        }
        PROFILE_STOP(2);
    }
}
