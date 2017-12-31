#pragma once

#include "matrix.h"

vector<double> gauss(Matrix<double> a, vector<double> b) {
    int N = a.N;
    for (int row = 0; row < N; row++) {
        int best = row;
        for (int i = row + 1; i < N; i++) {
            if (abs(a[best][row]) < abs(a[i][row]))
                best = i;
        }

        if (row != best) {
            swap(a[row], a[best]);
            swap(b[row], b[best]);
        }

        for (int i = row + 1; i < N; i++)
            a[row][i] /= a[row][row];

        b[row] /= a[row][row];
        for (int i = 0; i < N; i++) {
            double x = a[i][row];
            if (i != row && x != 0) {
                for (int j = row + 1; j < N; j++)
                    a[i][j] -= a[row][j] * x;

                b[i] -= b[row] * x;
            }
        }
    }

    return b;
}
