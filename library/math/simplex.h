#pragma once

/*
  <LP(Linear Program) solver>
    Maximize  c^T x
    - subject to Ax <= b, x >= 0

    - time complexity: O(2^n). O(M*N^2) in experiment. dependent on the modeling.

    from https://github.com/kth-competitive-programming/kactl/blob/master/content/numerical/Simplex.h
    from https://github.com/koosaga/DeobureoMinkyuParty/blob/master/teamnote.pdf

  <How to solve>
    result = Simplex(A, b, c).solve(x);

    - input  : A, b, c
    - output : result, x
*/
struct Simplex {
    using T = double;
    const T EPSILON = 1e-8;
    const T INF = numeric_limits<T>::infinity();

    int m, n;                   // A[m][n], b[m], c[n]
    vector<int> N, B;
    vector<vector<T>> D;

    Simplex() : m(0), n(0) {
    }

    Simplex(const vector<vector<T>>& A, const vector<T>& b, const vector<T>& c) {
        init(A, b, c);
    }

    void init(const vector<vector<T>>& A, const vector<T>& b, const vector<T>& c) {
        m = int(b.size());
        n = int(c.size());
        N = vector<int>(n + 1);
        B = vector<int>(m);
        D = vector<vector<T>>(m + 2, vector<T>(n + 2));
        /*
            D = | A[  0][0]  A[  0][1] ... A[  0][n-1]  -1  b[  0] |
                | A[  1][0]  A[  1][1] ... A[  1][n-1]  -1  b[  1] |
                |   ...        ...           ...                   |
                | A[m-1][0]  A[m-1][1] ... A[m-1][n-1]  -1  b[m-1] |
                |     -c[0]      -c[1] ...     -c[n-1]   0       0 |
                |         0          0 ...           0   1       0 |
            B = | n+0  n+1  n+2  ...  n+m-1 |
            N = [ 0  1  2  ...  n-1 -1 |
        */
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++)
                D[i][j] = A[i][j];
        }
        for (int i = 0; i < m; i++) {
            B[i] = n + i;
            D[i][n] = -1;
            D[i][n + 1] = b[i];
        }

        for (int j = 0; j < n; j++) {
            N[j] = j;
            D[m][j] = -c[j];
        }
        N[n] = -1;
        D[m + 1][n] = 1;
    }

    /*
        return
          1) -INF  : if there is no solution
          2) INF   : if there are arbitrarily good solutions
          3) other : the maximum value of c^T x
    */
    T solve(vector<T>& x) {
        int r = 0;
        for (int i = 1; i < m; i++) {
            if (D[i][n + 1] < D[r][n + 1])
                r = i;
        }

        if (D[r][n + 1] < -EPSILON) {
            pivot(r, n);
            if (!simplex(2) || D[m + 1][n + 1] < -EPSILON)
                return -INF;

            for (int i = 0; i < m; i++) {
                if (B[i] == -1) {
                    int s = 0;
                    for (int j = 1; j < n + 1; j++) {
                        if (s == -1 || make_pair(D[i][j], N[j]) < make_pair(D[i][s], N[s]))
                            s = j;
                    }
                    pivot(i, s);
                }
            }
        }

        bool ok = simplex(1);
        x = vector<T>(n);
        for (int i = 0; i < m; i++) {
            if (B[i] < n)
                x[B[i]] = D[i][n + 1];
        }

        return ok ? D[m][n + 1] : INF;
    }

private:
    void pivot(int r, int s) {
        T* a = D[r].data();
        T inv = 1 / a[s];
        for (int i = 0; i < m + 2; i++) {
            if (i != r && abs(D[i][s]) > EPSILON) {
                T *b = D[i].data();
                T inv2 = b[s] * inv;
                for (int j = 0; j < n + 2; j++)
                    b[j] -= a[j] * inv2;
                b[s] = a[s] * inv2;
            }
        }
        for (int j = 0; j < n + 2; j++) {
            if (j != s)
                D[r][j] *= inv;
        }
        for (int i = 0; i < m + 2; i++) {
            if (i != r)
                D[i][s] *= -inv;
        }
        D[r][s] = inv;
        swap(B[r], N[s]);
    }

    bool simplex(int phase) {
        int x = m + phase - 1;
        for (;;) {
            int s = -1;
            for (int j = 0; j < n + 1; j++) {
                if (N[j] != -phase) {
                    if (s == -1 || make_pair(D[x][j], N[j]) < make_pair(D[x][s], N[s]))
                        s = j;
                }
            }
            if (D[x][s] >= -EPSILON)
                return true;
            int r = -1;
            for (int i = 0; i < m; i++) {
                if (D[i][s] <= EPSILON)
                    continue;
                if (r == -1 || make_pair(D[i][n + 1] / D[i][s], B[i]) < make_pair(D[r][n + 1] / D[r][s], B[r]))
                    r = i;
            }
            if (r == -1)
                return false;
            pivot(r, s);
        }
    }
};
