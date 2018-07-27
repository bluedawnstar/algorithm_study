#pragma once

#include "bitVectorRank.h"

template <typename T>
struct WaveletTreeBitVector {
    static const T NaN = numeric_limits<T>::min();

    struct Node {
        T valLow, valHigh;      // value range (not index), inclusive
        BitVectorRank rank;

        Node* left;
        Node* right;

        // [first, last), [valLo, valHi]
        Node(T* first, T* last, T vLo, T vHi) : left(nullptr), right(nullptr) {
            valLow = vLo;
            valHigh = vHi;

            if (first >= last)
                return;

            // only a value
            if (valHigh == valLow) {
                int N = int(last - first);
                rank.init(N);
                rank.set();
                rank.buildRank();
                return;
            }

            T mid = valLow + (valHigh - valLow) / 2;

            int N = int(last - first);
            rank.init(N);
            auto* it = first;
            for (int i = 0; i < N; i++, ++it) {
                if (*it <= mid)
                    rank.set(i);
            }
            rank.buildRank();

            auto pivot = stable_partition(first, last, [mid](T valLo) {
                return valLo <= mid;
            });

            left = new Node(first, pivot, valLow, mid);
            right = new Node(pivot, last, mid + 1, valHigh);
        }

        // the number of numbers in range [L, R] less than or equal to k
        // inclusive (0 <= L <= R < N)
        int countLessOrEqual(int L, int R, T k) {
            if (L > R || k < valLow)
                return 0;

            if (valHigh <= k)
                return R - L + 1;

            int ltCount = rank.rank1(L - 1);
            int rtCount = rank.rank1(R);

            return left->countLessOrEqual(ltCount, rtCount - 1, k)
                + right->countLessOrEqual(L - ltCount, R - rtCount, k);
        }

        // kth smallest element in [L, R]
        // inclusive (0 <= L <= R < N)
        int kth(int L, int R, int k) {
            if (L > R)
                return 0;

            if (valLow == valHigh)
                return valLow;

            int ltCount = rank.rank1(L - 1);
            int rtCount = rank.rank1(R);
            int cntIn = rtCount - ltCount;

            if (k < cntIn)
                return left->kth(ltCount, rtCount - 1, k);
            else
                return right->kth(L - ltCount, R - rtCount, k - cntIn);
        }

        // the number of k in [L, R]
        // inclusive (0 <= L <= R < N)
        int count(int L, int R, T k) {
            if (L > R || k < valLow || k > valHigh)
                return 0;

            if (valLow == valHigh)
                return R - L + 1;

            int ltCount = rank.rank1(L - 1);
            int rtCount = rank.rank1(R);

            T mid = valLow + (valHigh - valLow) / 2;
            if (k <= mid)
                return left->count(ltCount, rtCount - 1, k);
            else
                return right->count(L - ltCount, R - rtCount, k);
        }

        // the number of k in [L, R]
        // inclusive (0 <= L <= R < N)
        int count(int L, int R, T kLow, T kHigh) {
            if (L > R || kHigh < valLow || kLow > valHigh)
                return 0;

            if (valLow == valHigh)
                return R - L + 1;

            int ltCount = rank.rank1(L - 1);
            int rtCount = rank.rank1(R);

            T mid = valLow + (valHigh - valLow) / 2;
            return left->count(ltCount, rtCount - 1, kLow, kHigh)
                + right->count(L - ltCount, R - rtCount, kLow, kHigh);
        }

        // (0 <= pos < N - 1)
        void swap(int pos, T* first, T orgX1, T orgX2) {
            if (pos + 1 >= int(rank.size()) || valLow == valHigh)
                return;

            int ltCount = rank.rank1(pos - 1);

            T mid = valLow + (valHigh - valLow) / 2;
            if (orgX1 <= mid && orgX2 <= mid)
                left->swap(ltCount, first, orgX1, orgX2);
            else if (orgX1 > mid && orgX2 > mid)
                right->swap(pos - ltCount, first + rank.count(), orgX1, orgX2);
            else {
                ::swap(*(first + pos), *(first + pos + 1));
                if (orgX1 < orgX2) {
                    rank.reset(pos);
                    rank.set(pos + 1);
                } else {
                    rank.reset(pos + 1);
                    rank.set(pos);
                }
                int idx = pos >> BitVectorRank::INDEX_SHIFT;
                if (idx != ((pos + 1) >> BitVectorRank::INDEX_SHIFT)) {
                    if (orgX1 < orgX2) {
                        --rank.rank[idx + 1];
                    } else {
                        ++rank.rank[idx + 1];
                    }
                }
            }
        }
    };

    int N;
    vector<T> data;
    Node* tree;

    WaveletTreeBitVector() : N(0), data(), tree(nullptr) {
    }

    ~WaveletTreeBitVector() {
        destroyAll(tree);
    }

    // inclusive
    void build(const T* in, int size, T valLow, T valHigh) {
        N = size;
        data.assign(in, in + size);

        tree = new Node(&data[0], &data[0] + size, valLow, valHigh);
    }

    // inclusive
    void build(const vector<T>& in, T valLow, T valHigh) {
        build(&in[0], int(in.size()), valLow, valHigh);
    }

    void build(const T* in, int size) {
        T valLow = in[0];
        T valHigh = in[0];
        for (int i = 1; i < size; i++) {
            if (valLow > in[i])
                valLow = in[i];
            if (valHigh < in[i])
                valHigh = in[i];
        }

        build(in, size, valLow, valHigh);
    }

    void build(const vector<T>& in) {
        build(&in[0], int(in.size()));
    }


    int countLessOrEqual(T k) {
        if (!tree)
            return 0;
        return tree->countLessOrEqual(0, N - 1, k);
    }


    // the number of values less than or equal to k - O(log(max_value))
    // inclusive (0 <= left <= right < N)
    int countLessOrEqual(int left, int right, T k) {
        if (!tree)
            return 0;
        return tree->countLessOrEqual(left, right, k);
    }

    // (0 <= k < N)
    int kth(int k) {
        if (!tree)
            return NaN;
        return tree->kth(0, N - 1, k);
    }

    // kth smallest element in [L, R]
    // inclusive (0 <= L <= R < N, 0 <= k < N)
    int kth(int left, int right, int k) {
        if (!tree)
            return NaN;
        return tree->kth(left, right, k);
    }

    int count(T k) {
        if (!tree)
            return 0;
        return tree->count(0, N - 1, k);
    }

    // the number of k in [L, R]
    // inclusive (0 <= L <= R < N)
    int count(int left, int right, T k) {
        if (!tree)
            return 0;
        return tree->count(left, right, k);
    }

    // the number of k in [L, R]
    // inclusive (0 <= L <= R < N)
    int count(int left, int right, T kLow, T kHigh) {
        if (!tree)
            return 0;
        return tree->count(left, right, kLow, kHigh);
    }

    // swap(A[pos], A[pos + 1])
    // (0 <= pos < N - 1)
    void swap(int pos, T orgX1, T orgX2) {
        if (!tree || pos < 0 || pos >= N - 1 || orgX1 == orgX2)
            return;
        tree->swap(pos, &data[0], orgX1, orgX2);
    }

private:
    void destroyAll(Node* node) {
        if (!node)
            return;
        if (node->left)
            destroyAll(node->left);
        if (node->right)
            destroyAll(node->right);
        delete node;
    }
};
