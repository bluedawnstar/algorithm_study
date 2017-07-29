#pragma once

template <typename T>
struct Matrix2x2 {
    T a00, a01;
    T a10, a11;

    Matrix2x2& init() {
        a00 = a01 = a10 = a11;
        return *this;
    }

    Matrix2x2& init(T _a00, T _a01, T _a10, T _a11) {
        a00 = _a00; a01 = _a01;
        a10 = _a10; a11 = _a11;
        return *this;
    }

    Matrix2x2& identity() {
        a00 = a11 = 1;
        a01 = a10 = 0;
        return *this;
    }

    Matrix2x2& operator +=(T x) {
        a00 += x; a01 += x;
        a10 += x; a11 += x;
        return *this;
    }

    Matrix2x2& operator -=(T x) {
        return operator +=(-x);
    }

    Matrix2x2& operator *=(T x) {
        a00 *= x; a01 *= x;
        a10 *= x; a11 *= x;
        return *this;
    }

    Matrix2x2& operator /=(T x) {
        a00 /= x; a01 /= x;
        a10 /= x; a11 /= x;
        return *this;
    }

    Matrix2x2& operator *=(const Matrix2x2& rhs) {
        T t00 = a00 * rhs.a00 + a01 * rhs.a10;
        T t01 = a00 * rhs.a01 + a01 * rhs.a11;
        T t10 = a10 * rhs.a00 + a11 * rhs.a10;
        T t11 = a10 * rhs.a01 + a11 * rhs.a11;
        a00 = t00; a01 = t01;
        a10 = t10; a11 = t11;
        return *this;
    }

    Matrix2x2 operator +(T x) const {
        Matrix2x2 res;
        res.a00 = a00 + x; res.a01 = a01 + x;
        res.a10 = a10 + x; res.a11 = a11 + x;
        return res;
    }

    Matrix2x2 operator -(T x) const {
        return operator +(-x);
    }

    Matrix2x2 operator *(T x) const {
        Matrix2x2 res;
        res.a00 = a00 * x; res.a01 = a01 * x;
        res.a10 = a10 * x; res.a11 = a11 * x;
        return res;
    }

    Matrix2x2 operator /(T x) const {
        Matrix2x2 res;
        res.a00 = a00 / x; res.a01 = a01 / x;
        res.a10 = a10 / x; res.a11 = a11 / x;
        return res;
    }

    Matrix2x2 operator *(const Matrix2x2& rhs) const {
        Matrix2x2 res;
        res.a00 = a00 * rhs.a00 + a01 * rhs.a10;
        res.a01 = a00 * rhs.a01 + a01 * rhs.a11;
        res.a10 = a10 * rhs.a00 + a11 * rhs.a10;
        res.a11 = a10 * rhs.a01 + a11 * rhs.a11;
        return res;
    }

    static const Matrix2x2& getIdentity() {
        static Matrix2x2 id{ T(1), T(0), T(0), T(1) };
        return id;
    }

    //PRECONDITION: n >= 0
    static Matrix2x2 pow(const Matrix2x2& m, int n) {
        if (n == 1)
            return m;
        else if (n == 0)
            return getIdentity();

        if (n & 1)
            return m * pow(m, n - 1);

        auto t = pow(m, n / 2);
        return t * t;
    }
};

template <typename T, int mod>
struct Matrix2x2Mod {
    T a00, a01;
    T a10, a11;

    Matrix2x2Mod& init() {
        a00 = a01 = a10 = a11;
        return *this;
    }

    Matrix2x2Mod& init(T _a00, T _a01, T _a10, T _a11) {
        a00 = _a00; a01 = _a01;
        a10 = _a10; a11 = _a11;
        return *this;
    }

    Matrix2x2Mod& identity() {
        a00 = a11 = 1;
        a01 = a10 = 0;
        return *this;
    }

    Matrix2x2Mod& operator +=(T x) {
        a00 = (a00 + x) % mod; a01 = (a01 + x) % mod;
        a10 = (a10 + x) % mod; a11 = (a11 + x) % mod;
        return *this;
    }

    Matrix2x2Mod& operator -=(T x) {
        return operator +=(-x);
    }

    Matrix2x2Mod& operator *=(T x) {
        a00 = (a00 * x) % mod; a01 = (a01 * x) % mod;
        a10 = (a10 * x) % mod; a11 = (a11 * x) % mod;
        return *this;
    }

    Matrix2x2Mod& operator *=(const Matrix2x2Mod& rhs) {
        T t00 = (a00 * rhs.a00 + a01 * rhs.a10) % mod;
        T t01 = (a00 * rhs.a01 + a01 * rhs.a11) % mod;
        T t10 = (a10 * rhs.a00 + a11 * rhs.a10) % mod;
        T t11 = (a10 * rhs.a01 + a11 * rhs.a11) % mod;
        a00 = t00; a01 = t01;
        a10 = t10; a11 = t11;
        return *this;
    }

    Matrix2x2Mod operator +(T x) const {
        Matrix2x2Mod res;
        res.a00 = (a00 + x) % mod; res.a01 = (a01 + x) % mod;
        res.a10 = (a10 + x) % mod; res.a11 = (a11 + x) % mod;
        return res;
    }

    Matrix2x2Mod operator -(T x) const {
        return operator +(-x);
    }

    Matrix2x2Mod operator *(T x) const {
        Matrix2x2Mod res;
        res.a00 = (a00 * x) % mod; res.a01 = (a01 * x) % mod;
        res.a10 = (a10 * x) % mod; res.a11 = (a11 * x) % mod;
        return res;
    }

    Matrix2x2Mod operator *(const Matrix2x2Mod& rhs) const {
        Matrix2x2Mod res;
        res.a00 = (a00 * rhs.a00 + a01 * rhs.a10) % mod;
        res.a01 = (a00 * rhs.a01 + a01 * rhs.a11) % mod;
        res.a10 = (a10 * rhs.a00 + a11 * rhs.a10) % mod;
        res.a11 = (a10 * rhs.a01 + a11 * rhs.a11) % mod;
        return res;
    }

    static const Matrix2x2Mod& getIdentity() {
        static Matrix2x2Mod id{ T(1), T(0), T(0), T(1) };
        return id;
    }

    //PRECONDITION: n >= 0
    static Matrix2x2Mod pow(const Matrix2x2Mod& m, int n) {
        if (n == 1)
            return m;
        else if (n == 0)
            return getIdentity();

        if (n & 1)
            return m * pow(m, n - 1);

        auto t = pow(m, n / 2);
        return t * t;
    }
};


//-----------------------------------------------------------------------------

template <typename T>
struct Matrix {
    int N;
    vector<vector<T>> val;

    Matrix(int _N) : N(_N), val(_N, vector<T>(_N)) {
    }

    Matrix& init() {
        for (int i = 0; i < N; i++)
            fill(val[i].begin(), val[i].end(), 0);
        return *this;
    }

    Matrix& identity() {
        for (int i = 0; i < N; i++)
            val[i][i] = 1;
        return *this;
    }

    const vector<T>& operator[](int row) const {
        return val[row];
    }

    vector<T>& operator[](int row) {
        return val[row];
    }

    Matrix& operator +=(T x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                val[i][j] += x;
        return *this;
    }

    Matrix& operator -=(T x) {
        return operator +=(-x);
    }

    Matrix& operator *=(T x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                val[i][j] *= x;
        return *this;
    }

    Matrix& operator /=(T x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                val[i][j] /= x;
        return *this;
    }

    Matrix& operator *=(const Matrix& rhs) {
        Matrix t = *this;
        multiply(*this, t, rhs);
        return *this;
    }

    Matrix operator +(T x) const {
        Matrix res = *this;
        res += x;
        return res;
    }

    Matrix operator -(T x) const {
        Matrix res = *this;
        res -= x;
        return res;
    }

    Matrix operator *(T x) const {
        Matrix res = *this;
        res *= x;
        return res;
    }

    Matrix operator /(T x) const {
        Matrix res = *this;
        res /= x;
        return res;
    }

    Matrix operator *(const Matrix& rhs) const {
        Matrix res(N);
        multiply(res, *this, rhs);
        return res;
    }

    static void multiply(Matrix& out, const Matrix& left, const Matrix& right) {
        int N = left.N;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                out[r][c] = 0;
                for (int k = 0; k < N; k++) {
                    out[r][c] += left[r][k] * right[k][c];
                }
            }
        }
    }

    static const Matrix& getIdentity(int N) {
        /*static unordered_map<int, shared_ptr<Matrix<T>>> M;

        auto& it = M.find(N);
        if (it != M.end())
            return *it->second;
        
        auto mat = make_shared<Matrix<T>>(N);
        mat->identity();
        M[N] = mat;

        return *mat;
        */
        static vector<shared_ptr<Matrix<T>>> M;

        if (N >= (int)M.size()) {
            M.resize(N + 1);
            M[N] = make_shared<Matrix<T>>(N);
            M[N]->identity();
        }
        return *M[N];
    }

    //PRECONDITION: n >= 0
    static Matrix pow(const Matrix& m, int n) {
        if (n == 1)
            return m;
        else if (n == 0)
            return getIdentity(m.N);

        if (n & 1)
            return m * pow(m, n - 1);

        auto t = pow(m, n / 2);
        return t * t;
    }
};
