#pragma once

// LSB first
struct BoolExprBitSet {
    static int popCount(unsigned long long x) {
#if defined(_M_X64)
        return int(__popcnt64(x));
#elif defined(__GNUC__)
        return __builtin_popcountll(x);
#else
        return int(__popcnt(unsigned(x))) + int(__popcnt(unsigned(x >> 32)));
#endif
    }

    static const int BIT_SIZE = sizeof(unsigned long long) * 8;
    static const unsigned long long BIT_ALL = (unsigned long long)-1ll;
    static const unsigned long long BIT_ONE = 1ull;

    static const int INDEX_MASK = 0x3F;
    static const int INDEX_SHIFT = 6;

    int N;
    vector<unsigned long long> values;

    BoolExprBitSet() {
    }

    explicit BoolExprBitSet(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        int r = N % BIT_SIZE;
        values = vector<unsigned long long>((N + BIT_SIZE - 1) / BIT_SIZE);
    }

    template <typename T>
    void buildFrom(const vector<T>& onePositions, int n) {
        init(n);
        for (int i : onePositions)
            set(i);
    }

    int size() const {
        return N;
    }

    int count() const {
        int res = 0;
        for (int i = 0; i < int(values.size()); i++)
            res += popCount(values[i]);
        return res;
    }

    bool test(int pos) const {
        int idx = pos >> INDEX_SHIFT;
        int off = BIT_SIZE - 1 - (pos & INDEX_MASK);    // bit reverse
        return (values[idx] & (BIT_ONE << off)) != 0;
    }

    void set(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = BIT_SIZE - 1 - (pos & INDEX_MASK);
        values[idx] |= BIT_ONE << off;
    }

    void reset(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = BIT_SIZE - 1 - (pos & INDEX_MASK);
        values[idx] &= ~(BIT_ONE << off);
    }

    void reset() {
        int n = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            values[i] = 0;
    }

    BoolExprBitSet& operator ^=(const BoolExprBitSet& rhs) {
        int n = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            values[i] ^= rhs.values[i];
        return *this;
    }

    bool operator ==(const BoolExprBitSet& rhs) const {
        int n = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        for (int i = 0; i < n; i++) {
            if (values[i] != rhs.values[i])
                return false;
        }
        return true;
    }

    bool operator !=(const BoolExprBitSet& rhs) const {
        return !operator ==(rhs);
    }

    bool operator <(const BoolExprBitSet& rhs) const {
        return compare(rhs) < 0;
    }

    bool operator <=(const BoolExprBitSet& rhs) const {
        return compare(rhs) <= 0;
    }

    bool operator >(const BoolExprBitSet& rhs) const {
        return compare(rhs) > 0;
    }

    bool operator >=(const BoolExprBitSet& rhs) const {
        return compare(rhs) >= 0;
    }

    int compare(const BoolExprBitSet& rhs) const {
        int n = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        for (int i = 0; i < n; i++) {
            if (values[i] < rhs.values[i])
                return -1;
            else if (values[i] > rhs.values[i])
                return 1;
        }
        return 0;
    }

    size_t getHash() const {
        size_t res = 0;

        int n = (N + BIT_SIZE - 1) >> INDEX_SHIFT;
        for (int i = 0; i < n; i++) {
            if (i & 1)
                res ^= values[i] << 1;
            else
                res ^= values[i] >> 1;
        }
        return res;
    }
};

struct BooleanXorExpression {
    /*
    variableCount : the number of all variables
    X/Y : expressions
    X[0][0] ^ X[0][1] ^ ... = Y[0]
    X[1][0] ^ X[1][1] ^ ... = Y[1]
    ...
    X[i][0] ^ X[i][1] ^ ... = Y[i]
    ...

    X[i][j] : variable ID (0 <= X[i][j] < variableCount)
    Y[i]    : 0 or 1
    */
    // O(N^2*M*logN), N = expression count, M = variable count
    static long long countSolutionMod(const vector<vector<int>>& X, const vector<int>& Y, int variableCount, int mod) {
        int N = int(Y.size());

        vector<int> idx(N);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&X](int a, int b) { return X[a] < X[b]; });

        //step1: remove duplicate expressions
        int j = 0;
        for (int i = 1; i < N; i++) {
            if (X[idx[j]] == X[idx[i]]) {
                if (Y[idx[j]] != Y[idx[i]])
                    return 0ll;
            } else if (++j < i) {
                swap(idx[j], idx[i]);
            }
        }
        idx.resize(j + 1);

        //step2: make a matrix, O(N*M)
        vector<pair<vector<int>,int>> mat(idx.size(), pair<vector<int>,int>(vector<int>(variableCount), 0));
        {
            for (int i = 0; i < int(idx.size()); i++) {
                for (auto j : X[idx[i]])
                    mat[i].first[j] = 1;
                mat[i].second = Y[idx[i]];
            }
        }

        //step3: solve with gaussian elimination, O(N^2*M*logN)
        int cnt = 0;
        {
            int H = int(mat.size());
            int W = variableCount;
            for (int row = 0; row < H; row++) {
                int selected = row;
                for (int i = row + 1; i < H; i++) {
                    if (mat[selected].first < mat[i].first)
                        selected = i;
                }
                if (selected != row)
                    swap(mat[selected], mat[row]);

                // add expression
                if (row > 0 && mat[row - 1].first == mat[row].first) {
                    if (mat[row - 1].second != mat[row].second)
                        return 0ll;
                } else if (!isEmptyRow(mat[row].first)) {
                    cnt++;
                } else if (mat[row].second) {
                    return 0ll; // remove "<empty> = 1"
                }

                // process expressions with same order
                for (int i = row + 1; i < H; i++) {
                    if (!mat[i].first[row])
                        continue;

                    if (mat[row].first == mat[i].first && mat[row].second != mat[i].second)
                        return 0ll;

                    for (int j = row; j < W; j++)
                        mat[i].first[j] ^= mat[row].first[j];
                    mat[i].second ^= mat[row].second;
                }
            }
        }

        if (cnt == variableCount)
            return 1ll;
        else if (cnt < variableCount)
            return modPow(2, variableCount - cnt, mod);

        return 1ll;
    }

    // O(N^2*M / BIT_SIZE), N = expression count, M = variable count
    static long long countSolutionMod(const vector<BoolExprBitSet>& X, const vector<int>& Y, int variableCount, int mod) {
        int N = int(Y.size());

        vector<int> idx(N);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&X](int a, int b) { return X[a] < X[b]; });

        //step1: remove duplicate expressions
        int j = 0;
        for (int i = 1; i < N; i++) {
            if (X[idx[j]] == X[idx[i]]) {
                if (Y[idx[j]] != Y[idx[i]])
                    return 0ll;
            } else if (++j < i) {
                swap(idx[j], idx[i]);
            }
        }
        idx.resize(j + 1);

        //step2: make a matrix
        vector<pair<BoolExprBitSet, int>> mat;
        mat.reserve(idx.size());
        for (auto i : idx)
            mat.emplace_back(X[i], Y[i]);

        //step3: solve with gaussian elimination
        int cnt = 0;
        {
            int H = int(mat.size());
            int W = variableCount;
            for (int row = 0; row < H; row++) {
                int selected = row;
                for (int i = row + 1; i < H; i++) {
                    if (mat[selected].first < mat[i].first)
                        selected = i;
                }
                if (selected != row)
                    swap(mat[selected], mat[row]);

                // add expression
                if (row > 0 && mat[row - 1].first == mat[row].first) {
                    if (mat[row - 1].second != mat[row].second)
                        return 0ll;
                } else if (mat[row].first.count() > 0) {
                    cnt++;
                } else if (mat[row].second) {
                    return 0ll; // remove "<empty> = 1"
                }

                // process expressions with same order
                for (int i = row + 1; i < H; i++) {
                    if (!mat[i].first.test(row))
                        continue;

                    if (mat[row].first == mat[i].first && mat[row].second != mat[i].second)
                        return 0ll;

                    mat[i].first ^= mat[row].first;
                    mat[i].second ^= mat[row].second;
                }
            }
        }

        if (cnt == variableCount)
            return 1ll;
        else if (cnt < variableCount)
            return modPow(2, variableCount - cnt, mod);

        return 1ll;
    }

private:
    static long long modPow(int x, int n, int mod) {
        if (n == 0)
            return 1ll;

        long long t = x % mod;
        long long res = 1ll;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return res;
    }

    static bool isEmptyRow(const vector<int>& v) {
        for (auto x : v) {
            if (x)
                return false;
        }
        return true;
    }
};
