#pragma once

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
