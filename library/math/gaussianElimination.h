#pragma once

#include "matrix.h"

// SLAE = System of n linear algebraic equations
struct SLAE {
    // O(N^3), 'a' is a N x N matrix
    static vector<double> gauss(Matrix<double> a, vector<double> b) {
        const double EPS = 1e-9;

        int N = a.N;
        for (int row = 0, col = 0; col < N; col++) {
            int best = row;
            for (int i = row + 1; i < N; i++) {
                if (abs(a[best][col]) < abs(a[i][col]))
                    best = i;
            }
            if (abs(a[best][col]) < EPS)
                continue;

            if (row != best) {
                swap(a[row], a[best]);
                swap(b[row], b[best]);
            }

            auto x = a[row][col];
            for (int i = col; i < N; i++)
                a[row][i] /= x;
            b[row] /= x;

            for (int i = 0; i < N; i++) {
                double x = a[i][col];
                if (i != row && x != 0) {
                    for (int j = col; j < N; j++)
                        a[i][j] -= a[row][j] * x;
                    b[i] -= b[row] * x;
                }
            }

            row++;
        }

        return b;
    }

    static const int INF = 0x3f3f3f3f;

    // O(N^3), 'a' is a N x M matrix.
    static int gaussGeneral(vector<vector<double>> a, vector<double> b, vector<double>& ans) {
        const double EPS = 1e-9;

        int N = (int)a.size();
        int M = (int)a[0].size();

        vector<int> where(M, -1);
        for (int col = 0, row = 0; col < M && row < N; col++) {
            int best = row;
            for (int i = row; i < N; ++i) {
                if (abs(a[best][col]) < abs(a[i][col]))
                    best = i;
            }
            if (abs(a[best][col]) < EPS)
                continue;

            if (row != best) {
                swap(a[row], a[best]);
                swap(b[row], b[best]);
            }

            where[col] = row;

            auto x = a[row][col];
            for (int i = col; i < M; i++)
                a[row][i] /= x;
            b[row] /= x;

            for (int i = 0; i < N; ++i) {
                double x = a[i][col];
                if (i != row && x != 0) {
                    for (int j = col; j < M; ++j)
                        a[i][j] -= a[row][j] * x;
                    b[i] -= b[row] * x;
                }
            }

            row++;
        }

        ans.assign(M, 0);
        for (int i = 0; i < M; i++) {
            if (where[i] >= 0)
                ans[i] = b[where[i]];
        }

        for (int i = 0; i < N; i++) {
            double sum = 0;
            for (int j = 0; j < M; j++)
                sum += ans[j] * a[i][j];
            if (abs(sum - b[i]) > EPS)
                return 0;
        }

        for (int i = 0; i < M; i++)
            if (where[i] == -1)
                return INF;

        return 1;
    }
};
