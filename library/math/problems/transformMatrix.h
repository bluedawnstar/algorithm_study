#pragma once

/*
  https://www.codechef.com/problems/CONSADD

  Determine whether it is possible to change A to B in a finite number of below operations.
  - Choose an integer v.
  - Choose X consecutive elements of A, either in the same row or in the same column.
  - Add v to each of the chosen elements of A.
*/
struct TransformMatrix {
    // Can transform A to B?
    // O(R*C*X)
    template <typename T>
    static bool canTransform(const vector<vector<T>>& A, const vector<vector<T>>& B, T X) {
        int R = int(A.size());
        int C = int(A[0].size());

        vector<vector<T>> D(R, vector<T>(C));
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++)
                D[i][j] = B[i][j] - A[i][j];
        }

        for (int i = R - 1; i >= X - 1; i--) {
            for (int j = 0; j < C; j++) {
                T v = D[i][j];
                for (int k = 0; k < X; k++)
                    D[i - k][j] -= v;
            }
        }
        for (int i = X - 1; i >= 0; i--) {
            for (int j = C - 1; j >= X - 1; j--) {
                T v = D[i][j];
                for (int k = 0; k < X; k++)
                    D[i][j - k] -= v;
            }
        }

        /* check!

              1  2 ... X
           | -x -x -x  .  .  . |
           | -x -x -x  .  .  . |
           | -x -x -x  .  .  . |
           |  .  .  .  x  .  . |
           |  .  .  .  .  .  . |
           |  .  .  .  .  .  . |
        */
        T v = D[X - 1][X - 1];
        for (int i = 0; i + 1 < X; i++) {
            for (int j = 0; j + 1 < X; j++) {
                if (D[i][j] != -v)
                    return false;
            }
        }

        return true;
    }

    // https://discuss.codechef.com/t/consadd-editorial/86612
    // O(R*C)
    template <typename T>
    static bool canTransform2(const vector<vector<T>>& A, const vector<vector<T>>& B, T X) {
        int R = int(A.size());
        int C = int(A[0].size());

        vector<vector<T>> D(X, vector<T>(X));
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                D[i % X][j % X] += B[i][j] - A[i][j];
            }
        }

        for (int i = 1; i < X; i++) {
            T v = D[i][0] - D[0][0];
            for (int j = 1; j < X; j++) {
                if (v != D[i][j] - D[0][j])
                    return false;
            }
        }

        return true;
    }
};
