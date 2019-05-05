#pragma once

template <typename T>
struct UniquePathCounter {
    template <typename T>
    struct Matrix {
        int N;
        vector<vector<T>> val;

        Matrix(int _N) : N(_N), val(_N, vector<T>(_N)) {
        }

        Matrix(const vector<vector<T>>& rhs) : N(int(rhs.size())), val(rhs) {
        }

        Matrix(vector<vector<T>>&& rhs) : N(int(rhs.size())), val(move(rhs)) {
        }

        template <typename U>
        Matrix(const vector<vector<U>>& rhs) : N(int(rhs.size())), val(N, vector<T>(N)) {
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    val[i][j] = (T)rhs[i][j];
        }

        template <typename U>
        Matrix(const Matrix<U>& rhs) : N(rhs.N), val(N, vector<T>(N)) {
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    val[i][j] = (T)rhs.val[i][j];
        }


        Matrix& operator =(const vector<vector<T>>& rhs) {
            N = int(rhs.N);
            val = rhs;
            return *this;
        }

        Matrix& operator =(vector<vector<T>>&& rhs) {
            N = int(rhs.size());
            val = move(rhs);
            return *this;
        }

        template <typename U>
        Matrix& operator =(const vector<vector<U>>& rhs) {
            N = int(rhs.size());

            val.assign(N, vector<T>(N));
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    val[i][j] = (T)rhs[i][j];

            return *this;
        }

        template <typename U>
        Matrix& operator =(const Matrix<U>& rhs) {
            N = int(rhs.size());

            val.assign(N, vector<T>(N));
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    val[i][j] = (T)rhs.val[i][j];

            return *this;
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

        Matrix& operator *=(const Matrix& rhs) {
            Matrix t = *this;
            multiply(*this, t, rhs);
            return *this;
        }

        Matrix& operator +=(const Matrix& rhs) {
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    val[i][j] += rhs.val[i][j];
            return *this;
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

        static Matrix getIdentity(int N) {
            Matrix res(N);
            res.identity();
            return res;
        }

        //PRECONDITION: n >= 0
        static Matrix pow(const Matrix& m, int n) {
            if (n == 1)
                return m;
            else if (n == 0)
                return getIdentity(m.N);

            auto t = m;
            auto res = getIdentity(m.N);
            for (; n > 0; n >>= 1) {
                if (n & 1)
                    res *= t;
                t = move(t * t);
            }

            return res;
        }
    };

    static vector<T> countNaive(const vector<int> edges[], int N, int u, int step) {
        vector<T> from(N), to(N);
        to[u] = T(1);

        for (int j = 0; j < step; j++) {
            swap(from, to);
            fill(to.begin(), to.end(), T(0));
            for (int u = 0; u < N; u++) {
                for (auto v : edges[u])
                    to[v] += from[u];
            }
        }

        return to;
    }

    static vector<T> countNaive(const vector<vector<int>>& edges, int u, int step) {
        return countNaive(edges.data(), int(edges.size()), u, step);
    }


    static vector<vector<T>> countAllNaive(const vector<int> edges[], int N, int step) {
        vector<vector<T>> res(N);

        for (int i = 0; i < N; i++) {
            vector<T> from(N), to(N);
            to[i] = 1;

            for (int j = 0; j < step; j++) {
                swap(from, to);
                fill(to.begin(), to.end(), T(0));
                for (int u = 0; u < N; u++) {
                    for (auto v : edges[u])
                        to[v] += from[u];
                }
            }
            res[i].swap(to);
        }

        return res;
    }

    static vector<vector<T>> countAllNaive(const vector<vector<int>>& edges, int step) {
        return countAllNaive(edges.data(), int(edges.size()), step);
    }


    // from edges
    static Matrix<T> countAllFastFromEdges(const vector<int> edges[], int N, int step) {
        vector<vector<T>> adj(N, vector<T>(N));

        for (int u = 0; u < N; u++) {
            for (auto v : edges[u])
                adj[u][v] = 1;
        }
        return Matrix<T>::pow(Matrix<T>(adj), step);
    }

    static Matrix<T> countAllFastFromEdges(const vector<vector<int>>& edges, int step) {
        return countAllFastFromEdges(edges.data(), int(edges.size()), step);
    }

    static Matrix<T> countAllFastFromAdjMatrix(const vector<vector<T>>& adj, int step) {
        return Matrix<T>::pow(Matrix<T>(adj), step);
    }
};
