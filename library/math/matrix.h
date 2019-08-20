#pragma once

//-----------------------------------------------------------------------------

template <typename T>
struct Matrix {
    int N;
    vector<vector<T>> mat;

    explicit Matrix(int _N) : N(_N), mat(_N, vector<T>(_N)) {
    }

    Matrix(const vector<vector<T>>& rhs) : N(int(rhs.size())), mat(rhs) {
    }

    Matrix(vector<vector<T>>&& rhs) : N(int(rhs.size())), mat(move(rhs)) {
    }

    template <typename U>
    Matrix(const vector<vector<U>>& rhs) : N(int(rhs.size())), mat(N, vector<T>(N)) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<T>(rhs[i][j]);
    }

    template <typename U>
    Matrix(const Matrix<U>& rhs) : N(rhs.N), mat(N, vector<T>(N)) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<T>(rhs.mat[i][j]);
    }


    Matrix& operator =(const vector<vector<T>>& rhs) {
        N = static_cast<int>(rhs.N);
        mat = rhs;
        return *this;
    }

    Matrix& operator =(vector<vector<T>>&& rhs) {
        N = static_cast<int>(rhs.size());
        mat = move(rhs);
        return *this;
    }

    template <typename U>
    Matrix& operator =(const vector<vector<U>>& rhs) {
        N = static_cast<int>(rhs.size());

        mat.assign(N, vector<T>(N));
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<T>(rhs[i][j]);

        return *this;
    }

    template <typename U>
    Matrix& operator =(const Matrix<U>& rhs) {
        N = static_cast<int>(rhs.size());

        mat.assign(N, vector<T>(N));
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] = static_cast<T>(rhs.mat[i][j]);

        return *this;
    }


    Matrix& init() {
        for (int i = 0; i < N; i++)
            fill(mat[i].begin(), mat[i].end(), 0);
        return *this;
    }

    Matrix& identity() {
        for (int i = 0; i < N; i++)
            mat[i][i] = 1;
        return *this;
    }

    const vector<T>& operator[](int row) const {
        return mat[row];
    }

    vector<T>& operator[](int row) {
        return mat[row];
    }

    Matrix& operator +=(T x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] += x;
        return *this;
    }

    Matrix& operator -=(T x) {
        return operator +=(-x);
    }

    Matrix& operator *=(T x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] *= x;
        return *this;
    }

    Matrix& operator /=(T x) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] /= x;
        return *this;
    }

    Matrix& operator *=(const Matrix& rhs) {
        Matrix t = *this;
        multiply(*this, t, rhs);
        return *this;
    }

    Matrix& operator +=(const Matrix& rhs) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] += rhs.mat[i][j];
        return *this;
    }

    Matrix& operator -=(const Matrix& rhs) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                mat[i][j] -= rhs.mat[i][j];
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

    Matrix operator +(const Matrix& rhs) const {
        Matrix res(N);
        res += rhs;
        return res;
    }

    Matrix operator -(const Matrix& rhs) const {
        Matrix res(N);
        res -= rhs;
        return res;
    }


    Matrix pow(T n) const {
        return Matrix<T>::pow(*this, n);
    }

    // O(N^3)
    double det() const {
        const double EPS = 1e-10;

        double res = 1;
        vector<bool> used(N);

        Matrix<double> a = *this;
        for (int i = 0; i < N; i++) {
            int p;
            for (p = 0; p < N; p++) {
                if (!used[p] && abs(a[p][i]) > EPS)
                    break;
            }
            if (p >= N)
                return 0;

            res *= a[p][i];
            used[p] = true;

            double z = 1.0 / a[p][i];
            for (int j = 0; j < N; j++)
                a[p][j] *= z;

            for (int j = 0; j < N; j++) {
                if (j != p) {
                    z = a[j][i];
                    for (int k = 0; k < N; k++)
                        a[j][k] -= z * a[p][k];
                }
            }
        }
        return res;
    }


    Matrix<double> inverse() const {
        const double EPS = 1e-10;

        Matrix<double> res(N);
        for (int i = 0; i < N; i++)
            res[i][i] = 1;

        Matrix<double> mat = this->mat;
        for (int i = 0; i < N; i++) {
            int selected = -1;
            for (int j = i; j < N; j++) {
                if (fabs(mat[j][i]) > EPS) {
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

            double freq = 1.0 / mat[i][i];
            for (int j = 0; j < N; j++) {
                mat[i][j] *= freq;
                res[i][j] *= freq;
            }
            for (int j = 0; j < N; j++) {
                if (i == j)
                    continue;
                if (fabs(mat[j][i]) < EPS)
                    continue;

                auto freq = mat[j][i];
                for (int k = 0; k < N; k++) {
                    mat[j][k] -= mat[i][k] * freq;
                    res[j][k] -= res[i][k] * freq;
                }
            }
        }

        //for (int i = 0; i < N; i++) {
        //    for (int j = 0; j < N; j++) {
        //        if (i == j)
        //            assert(1.0 - EPS < mat[i][j] && mat[i][j] < 1.0 + EPS);
        //        else
        //            assert(fabs(mat[i][j]) < EPS);
        //    }
        //}

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
        static unordered_map<int, shared_ptr<Matrix<T>>> M;

        auto it = M.find(N);
        if (it != M.end())
            return *it->second;
        
        auto mat = make_shared<Matrix<T>>(N);
        mat->identity();
        M[N] = mat;

        return *mat;
    }

    //PRECONDITION: n >= 0
    static Matrix pow(const Matrix& m, T n) {
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

// https://github.com/yosupo06/Algorithm/tree/54cd52339c4bb8be8719ef04dbb4486e355f03fb/src/math
//TODO: QR decomposition
