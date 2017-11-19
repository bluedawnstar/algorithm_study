#pragma once

template <typename T>
struct WaveletTree {
    struct WaveletNode {
        T valLow, valHigh;      // value range (not index), inclusive
        vector<int> freq;

        WaveletNode* left;
        WaveletNode* right;

        // [first, last), [valLo, valHi]
        WaveletNode(T* first, T* last, T vLo, T vHi) {
            valLow = vLo, valHigh = vHi;

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

            left = new WaveletNode(first, pivot, valLow, mid);
            right = new WaveletNode(pivot, last, mid + 1, valHigh);
        }


        // the number of numbers in range [lo, hi] less than or equal to k
        // inclusive
        int countLessOrEqual(int lo, int hi, T k) {
            if (lo > hi || k < valLow)
                return 0;

            if (valHigh <= k)
                return hi - lo + 1;

            int ltCount = freq[lo - 1];
            int rtCount = freq[hi];

            return left->countLessOrEqual(ltCount + 1, rtCount, k)
                + right->countLessOrEqual(lo - ltCount, hi - rtCount, k);
        }
    };

    int N;
    vector<T> data;
    WaveletNode* tree;

    WaveletTree() : N(0), data(), tree(nullptr) {
    }

    // inclusive
    void build(const T* in, int size, T valLow, T valHigh) {
        N = size;
        data.assign(in, in + size);

        tree = new WaveletNode(&data[0], &data[0] + size, valLow, valHigh);
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
        return tree->countLessOrEqual(0, N - 1, k);
    }

    // the number of values (value <= k)
    // inclusive
    int queryCountLessOrEqual(int lo, int hi, T k) {
        if (!tree)
            return 0;
        return tree->countLessOrEqual(lo, hi, k);
    }
};
