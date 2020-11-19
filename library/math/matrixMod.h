#pragma once

template <int mod>
struct MatrixMod {
    int N;
    vector<vector<int>> mat;

    MatrixMod() : N(0) {
    }

    explicit MatrixMod(int _N) : N(_N), mat(_N, vector<int>(_N)) {
    }

    MatrixMod(const vector<vector<int>>& rhs) : N(int(rhs.size())), mat(rhs) {
    }

    MatrixMod(vector<vector<int>>&& rhs) : N(int(rhs.size())), mat(move(rhs)) {
    }

    template <typename U>
    MatrixMod(const vector<vector<U>>& rhs) : N(int(rhs.size())), mat(N, vector<int>(N)) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<int>(rhs[i][j]);
    }

    MatrixMod(const MatrixMod<mod>& rhs) : N(rhs.N), mat(N, vector<int>(N)) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<int>(rhs.mat[i][j]);
    }


    MatrixMod<mod>& operator =(const vector<vector<int>>& rhs) {
        N = static_cast<int>(rhs.size());
        mat = rhs;
        return *this;
    }

    MatrixMod<mod>& operator =(vector<vector<int>>&& rhs) {
        N = static_cast<int>(rhs.size());
        mat = move(rhs);
        return *this;
    }

    template <typename U>
    MatrixMod<mod>& operator =(const vector<vector<U>>& rhs) {
        N = static_cast<int>(rhs.size());

        mat.assign(N, vector<int>(N));
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<int>(rhs[i][j]);

        return *this;
    }

    MatrixMod<mod>& operator =(const MatrixMod<mod>& rhs) {
        N = rhs.N;

        mat.assign(N, vector<int>(N));
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<int>(rhs.mat[i][j]);

        return *this;
    }


    bool operator ==(const MatrixMod<mod>& rhs) const {
        return mat == rhs.mat;
    }

    bool operator !=(const MatrixMod<mod>& rhs) const {
        return mat != rhs.mat;
    }


    MatrixMod<mod>& init() {
        for (int i = 0; i < N; i++)
            fill(mat[i].begin(), mat[i].end(), 0);
        return *this;
    }

    MatrixMod<mod>& init(int n) {
        N = n;
        mat = vector<vector<int>>(n, vector<int>(n));
        return *this;
    }

    MatrixMod<mod>& identity() {
        for (int i = 0; i < N; i++)
            mat[i][i] = 1;
        return *this;
    }

    const vector<int>& operator[](int row) const {
        return mat[row];
    }

    vector<int>& operator[](int row) {
        return mat[row];
    }

    template <typename U>
    MatrixMod<mod>& operator +=(U x) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                mat[i][j] += x;
                if (mat[i][j] >= mod)
                    mat[i][j] -= mod;
                else if (mat[i][j] < 0)
                    mat[i][j] += mod;
            }
        }
        return *this;
    }

    template <typename U>
    MatrixMod<mod>& operator -=(U x) {
        return operator +=(-x);
    }

    MatrixMod<mod>& operator *=(int x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<int>(1ll * mat[i][j] * x % mod);
        return *this;
    }

    MatrixMod<mod>& operator /=(int x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<int>(1ll * mat[i][j] * modInv(x) % mod);
        return *this;
    }

    MatrixMod<mod>& operator *=(const MatrixMod<mod>& rhs) {
        MatrixMod t = *this;
        multiply(*this, t, rhs);
        return *this;
    }

    MatrixMod<mod>& operator +=(const MatrixMod<mod>& rhs) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                mat[i][j] += rhs.mat[i][j];
                if (mat[i][j] >= mod)
                    mat[i][j] -= mod;
                else if (mat[i][j] < 0)
                    mat[i][j] += mod;
            }
        return *this;
    }

    MatrixMod<mod>& operator -=(const MatrixMod<mod>& rhs) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                mat[i][j] -= rhs.mat[i][j];
                if (mat[i][j] >= mod)
                    mat[i][j] -= mod;
                else if (mat[i][j] < 0)
                    mat[i][j] += mod;
            }
        return *this;
    }

    MatrixMod<mod> operator +(int x) const {
        MatrixMod<mod> res = *this;
        res += x;
        return res;
    }

    MatrixMod<mod> operator -(int x) const {
        MatrixMod<mod> res = *this;
        res -= x;
        return res;
    }

    MatrixMod<mod> operator *(int x) const {
        MatrixMod<mod> res = *this;
        res *= x;
        return res;
    }

    MatrixMod<mod> operator /(int x) const {
        MatrixMod<mod> res = *this;
        res /= x;
        return res;
    }

    MatrixMod<mod> operator *(const MatrixMod<mod>& rhs) const {
        MatrixMod<mod> res(N);
        multiply(res, *this, rhs);
        return res;
    }

    MatrixMod<mod> operator +(const MatrixMod<mod>& rhs) const {
        MatrixMod<mod> res = *this;
        res += rhs;
        return res;
    }

    MatrixMod<mod> operator -(const MatrixMod<mod>& rhs) const {
        MatrixMod<mod> res = *this;
        res -= rhs;
        return res;
    }

    MatrixMod<mod> pow(long long n) const {
        return MatrixMod<mod>::pow(*this, n);
    }


    // O(N^3)
    int det() const {
        long long res = 1;
        vector<bool> used(N);

        MatrixMod<mod> a = *this;
        for (int i = 0; i < N; i++) {
            int p;
            for (p = 0; p < N; p++) {
                if (!used[p] && a[p][i])
                    break;
            }
            if (p >= N)
                return 0;

            res = (res * a[p][i]) % mod;
            used[p] = true;

            int z = modInv(a[p][i]);
            for (int j = 0; j < N; j++)
                a[p][j] = static_cast<int>(1ll * a[p][j] * z % mod);

            for (int j = 0; j < N; j++) {
                if (j != p) {
                    z = a[j][i];
                    for (int k = 0; k < N; k++) {
                        a[j][k] -= static_cast<int>(1ll * z * a[p][k] % mod);
                        if (a[j][k] >= mod)
                            a[j][k] -= mod;
                        else if (a[j][k] < 0)
                            a[j][k] += mod;
                    }
                }
            }
        }
        return static_cast<int>(res);
    }

    int det2() const {
        long long res = 1;
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                while (a[j][i] != 0) {
                    long long t = 1ll * a[i][i] / a[j][i];
                    if (t) {
                        for (int k = i; k < N; k++)
                            a[i][k] = (a[i][k] - a[j][k] * t) % mod;
                    }
                    swap(a[i], a[j]);
                    res *= -1;
                }
            }
            res = res * a[i][i] % mod;
            if (!res)
                return 0;
        }
        return (res + mod) % mod;
    }

    MatrixMod<mod> inverse() const {
        MatrixMod<mod> res(N);
        for (int i = 0; i < N; i++)
            res[i][i] = 1;

        auto mat = this->mat;
        for (int i = 0; i < n; i++) {
            int selected = -1;
            for (int j = i; j < n; j++) {
                if (mat[j][i]) {
                    selected = j;
                    break;
                }
            }
            if (selected == -1)
                continue;
            if (i != selected) {
                swap(mat[i], mat[selected]);
                swap(res[i], res[selected]);
            }

            auto freq = modInv(mat[i][i]);
            for (int j = 0; j < N; j++) {
                mat[i][j] = static_cast<int>(1ll * mat[i][j] * freq % mod);
                res[i][j] = static_cast<int>(1ll * res[i][j] * freq % mod);
            }
            for (int j = 0; j < N; j++) {
                if (i == j)
                    continue;
                if (!mat[j][i])
                    continue;
                auto freq = mod - mat[j][i];
                for (int k = 0; k < N; k++) {
                    mat[j][k] = static_cast<int>((mat[j][k] + 1ll * mat[i][k] * freq) % mod);
                    res[j][k] = static_cast<int>((res[j][k] + 1ll * res[i][k] * freq) % mod);
                }
            }
        }

        //for (int i = 0; i < N; i++) {
        //    for (int j = 0; j < N; j++) {
        //        if (i == j)
        //            assert(m[i][j] == 1);
        //        else
        //            assert(m[i][j] == 0);
        //    }
        //}

        return res;
    }


    static void multiply(MatrixMod<mod>& out, const MatrixMod<mod>& left, const MatrixMod<mod>& right) {
        int N = left.N;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                long long x = 0;
                for (int k = 0; k < N; k++) {
                    x = (x + 1ll * left[r][k] * right[k][c]) % mod;
                }
                out[r][c] = static_cast<int>(x);
            }
        }
    }

    static const MatrixMod<mod>& getIdentity(int N) {
        static unordered_map<int, shared_ptr<MatrixMod<mod>>> M;

        auto it = M.find(N);
        if (it != M.end())
            return *it->second;

        auto mat = make_shared<MatrixMod<mod>>(N);
        mat->identity();
        M[N] = mat;

        return *mat;
    }

    //PRECONDITION: n >= 0
    static MatrixMod<mod> pow(const MatrixMod& m, long long n) {
        if (n == 1)
            return m;
        else if (n == 0)
            return getIdentity(m.N);

        auto t = pow(m, n / 2);
        if (n & 1)
            return m * t * t;
        else
            return t * t;
    }

private:
    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return static_cast<int>(res);
    }

    static int modInv(int x) {
        return modPow(x, mod - 2);
    }
};

// https://github.com/yosupo06/Algorithm/tree/54cd52339c4bb8be8719ef04dbb4486e355f03fb/src/math
//TODO: QR decomposition
