#pragma once

template <typename T>
struct WaveletTree {
    static const T NaN = numeric_limits<T>::min();

    struct Node {
        T valLow, valHigh;      // value range (not index), inclusive
        vector<int> freq;

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
                freq.reserve(last - first + 1);
                freq.push_back(0);

                for (auto it = first; it != last; it++)
                    freq.push_back(freq.back() + 1);

                return;
            }

            T mid = valLow + (valHigh - valLow) / 2;

            freq.reserve(last - first + 1);
            freq.push_back(0);

            auto lessEqualMid = [mid](T valLo) {
                return valLo <= mid;
            };
            for (auto it = first; it != last; it++)
                freq.push_back(freq.back() + lessEqualMid(*it));

            auto pivot = stable_partition(first, last, lessEqualMid);

            left = new Node(first, pivot, valLow, mid);
            right = new Node(pivot, last, mid + 1, valHigh);
        }

        // the number of numbers in range [L, R] less than or equal to k
        // inclusive (1 <= L <= R <= N)
        int countLessOrEqual(int L, int R, T k) {
            if (L > R || k < valLow)
                return 0;

            if (valHigh <= k)
                return R - L + 1;

            int ltCount = freq[L - 1];
            int rtCount = freq[R];

            return left->countLessOrEqual(ltCount + 1, rtCount, k)
                + right->countLessOrEqual(L - ltCount, R - rtCount, k);
        }

        // kth smallest element in [L, R]
        // inclusive (1 <= L <= R <= N)
        int kth(int L, int R, int k) {
            if (L > R)
                return 0;

            if (valLow == valHigh)
                return valLow;

            int cntIn = freq[R] - freq[L - 1];
            int ltCount = freq[L - 1];
            int rtCount = freq[R];

            if (k <= cntIn)
                return left->kth(ltCount + 1, rtCount, k);
            else
                return right->kth(L - ltCount, R - rtCount, k - cntIn);
        }

        // the number of k in [L, R]
        // inclusive (1 <= L <= R <= N)
        int count(int L, int R, T k) {
            if (L > R || k < valLow || k > valHigh)
                return 0;

            if (valLow == valHigh)
                return R - L + 1;

            int ltCount = freq[L - 1];
            int rtCount = freq[R];

            T mid = valLow + (valHigh - valLow) / 2;
            if (k <= mid)
                return left->count(ltCount + 1, rtCount, k);
            else
                return right->count(L - ltCount, R - rtCount, k);
        }

        // the number of k in [L, R]
        // inclusive (1 <= L <= R <= N)
        int count(int L, int R, T kLow, T kHigh) {
            if (L > R || kHigh < valLow || kLow > valHigh)
                return 0;

            if (valLow == valHigh)
                return R - L + 1;

            int ltCount = freq[L - 1];
            int rtCount = freq[R];

            T mid = valLow + (valHigh - valLow) / 2;
            return left->count(ltCount + 1, rtCount, kLow, kHigh)
                 + right->count(L - ltCount, R - rtCount, kLow, kHigh);
        }

        // (1 <= pos < N)
        void swap(int pos, T* first, T orgX1, T orgX2) {
            if (pos + 1 >= (int)freq.size() || valLow == valHigh)
                return;

            int ltCount = freq[pos - 1];

            T mid = valLow + (valHigh - valLow) / 2;
            if (orgX1 <= mid && orgX2 <= mid)
                left->swap(ltCount + 1, first, orgX1, orgX2);
            else if (orgX1 > mid && orgX2 > mid)
                right->swap(pos - ltCount, first + freq.back(), orgX1, orgX2);
            else {
                ::swap(*(first + pos - 1), *(first + pos));
                if (orgX1 < orgX2)
                    --freq[pos];
                else
                    ++freq[pos];
            }
        }
    };

    int N;
    vector<T> data;
    Node* tree;

    WaveletTree() : N(0), data(), tree(nullptr) {
    }

    ~WaveletTree() {
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
        build(&in[0], (int)in.size(), valLow, valHigh);
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
        build(&in[0], (int)in.size());
    }

    int queryCountLessOrEqual(T k) {
        if (!tree)
            return 0;
        return tree->countLessOrEqual(1, N, k);
    }


    // the number of values less than or equal to k : O(log(max_value))
    // inclusive (0 <= left <= right < N)
    int queryCountLessOrEqual(int left, int right, T k) {
        if (!tree)
            return 0;
        return tree->countLessOrEqual(left + 1, right + 1, k);
    }

    // (0 <= k < N)
    int kth(int k) {
        if (!tree)
            return NaN;
        return tree->kth(1, N, k + 1);
    }

    // kth smallest element in [L, R]
    // inclusive (0 <= L <= R < N, 0 <= k < N)
    int kth(int left, int right, int k) {
        if (!tree)
            return NaN;
        return tree->kth(left + 1, right + 1, k + 1);
    }

    int count(T k) {
        if (!tree)
            return 0;
        return tree->count(1, N, k);
    }

    // the number of k in [L, R]
    // inclusive (0 <= L <= R < N)
    int count(int left, int right, T k) {
        if (!tree)
            return 0;
        return tree->count(left + 1, right + 1, k);
    }

    // the number of k in [L, R]
    // inclusive (0 <= L <= R < N)
    int count(int left, int right, T kLow, T kHigh) {
        if (!tree)
            return 0;
        return tree->count(left + 1, right + 1, kLow, kHigh);
    }

    // swap(A[pos], A[pos + 1])
    // (0 <= pos < N - 1)
    void swap(int pos, T orgX1, T orgX2) {
        if (!tree || pos < 0 || pos >= N - 1 || orgX1 == orgX2)
            return;
        tree->swap(pos + 1, &data[0], orgX1, orgX2);
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
