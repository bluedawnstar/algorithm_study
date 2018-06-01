#pragma once

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct SparseTable2D {
    int                 rowN;
    int                 colN;
    int                 logRowN;
    int                 logColN;

    vector<vector<vector<vector<T>>>> values; // [logR][R][logC][C]
    vector<int>         H;
    T                   defaultValue;
    BinOp               mergeOp;

    explicit SparseTable2D(BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    SparseTable2D(const vector<vector<T>>& a, BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    SparseTable2D(SparseTable2D&& rhs)
        : rowN(rhs.rowN), colN(rhs.colN), logRowN(rhs.logRowN), logColN(rhs.logColN),
          value(std::move(rhs.value)), H(std::move(rhs.H)), mergeOp(std::move(rhs.mergeOp)), defaultValue(rhs.defaultValue) {
    }

    void build(const vector<vector<T>>& a) {
        rowN = (int)a.size();
        colN = (int)a[0].size();

        H.resize(max(rowN, colN) + 1);
        H[1] = 0;
        for (int i = 2; i < (int)H.size(); i++)
            H[i] = H[i >> 1] + 1;

        logRowN = H[rowN] + 1;
        logColN = H[colN] + 1;

        values.resize(logRowN, vector<vector<vector<T>>>(rowN, vector<vector<T>>(logColN, vector<T>(colN, defaultValue))));
        for (int i = 0; i < rowN; i++) {
            vector<vector<T>>& currRow = values[0][i];

            for (int j = 0; j < colN; j++)
                currRow[0][j] = a[i][j];

            for (int j = 1; j < logColN; j++) {
                vector<T>& prev = currRow[j - 1];
                vector<T>& curr = currRow[j];

                int maxColN = colN - (1 << (j - 1));
                for (int h = 0; h < maxColN; h++) {
                    curr[h] = mergeOp(prev[h], prev[h + (1 << (j - 1))]);
                }
            }
        }

        for (int i = 1; i < logRowN; i++) {
            auto& prevRow = values[i - 1];
            auto& currRow = values[i];

            int maxR = rowN - (1 << (i - 1));
            for (int r = 0; r < maxR; r++) {
                auto& prevPrevRow = prevRow[r + (1 << (i - 1))];
                auto& prevCurrRow = prevRow[r];
                auto& currCurrRow = currRow[r];

                for (int j = 0; j < logColN; j++) {
                    for (int c = 0; c < colN; c++)
                        currCurrRow[j][c] = mergeOp(prevCurrRow[j][c], prevPrevRow[j][c]);
                }
            }
        }
    }


    // O(1), inclusive
    T query(int left, int top, int right, int bottom) const {
        right++;
        bottom++;
        if (right <= left || bottom <= top)
            return defaultValue;

        int kX = H[right - left];
        int kY = H[bottom - top];

        auto r1 = mergeOp(values[kY][top][kX][left], values[kY][top][kX][right - (1 << kX)]);
        auto r2 = mergeOp(values[kY][bottom - (1 << kY)][kX][left], values[kY][bottom - (1 << kY)][kX][right - (1 << kX)]);
        return mergeOp(r1, r2);
    }

    // O(log(right - left + 1) * log(bottom - top + 1)), inclusive
    T queryNoOverlap(int left, int top, int right, int bottom) const {
        right++;
        bottom++;
        if (right <= left || bottom <= top)
            return defaultValue;

        T res = defaultValue;

        int lengthY = bottom - top;
        while (lengthY) {
#ifndef __GNUC__
            int i = (int)_tzcnt_u32(lengthY);
#else
            int i = __builtin_ctz(lengthY);
#endif
            bottom -= (1 << i);
            {
                auto& vec = values[i][bottom];

                int R = right;
                int lengthX = right - left;
                while (lengthX) {
#ifndef __GNUC__
                    int j = (int)_tzcnt_u32(lengthX);
#else
                    int j = __builtin_ctz(lengthX);
#endif
                    R -= (1 << j);

                    res = mergeOp(res, vec[j][R]);

                    lengthX &= lengthX - 1;
                }
            }

            lengthY &= lengthY - 1;
        }

        return res;
    }
};

template <typename T, typename BinOp>
inline SparseTable2D<T, BinOp> makeSparseTable2D(const vector<vector<T>>& arr, BinOp op, T dfltValue = T()) {
    return SparseTable2D<T, BinOp>(arr, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeSparseTable2D<int>(v, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, top, right, bottom);

    2) Max Sparse Table
        auto sparseTable = makeSparseTable2D<int>(v, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, top, right, bottom);

    3) GCD Sparse Table
        auto sparseTable = makeSparseTable2D<int>(v, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, top, right, bottom);

    4) Sum Sparse Table
        auto sparseTable = makeSparseTable2D<int>(v, [](int a, int b) { return a + b; });
        ...
        sparseTable.queryNoOverlap(left, top, right, bottom);
*/
