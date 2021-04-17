#pragma once

/*
  Transform matrix A to O with flip operations

  1. A[i][j] = { -1, 0, 1 },  0 <= i < N, 0 <= j < M
    -1 : don't care

  2. operation
    1) flip { A[row][colL], A[row][colL + 1], A[row][colL + 2], ..., A[row][colR] }
    2) flip { A[rowL][col], A[rowL + 1][col], A[rowL + 1][col], ..., A[rowR][col] }

  3. Objective
    find the smallest number of operations required to transform A to O

  https://www.codechef.com/problems/PAIRFLIP
*/
struct TransformMatrixWithFlip {
    int N, M;
    vector<vector<char>> A;

    // any solution (flip operations)
    vector<array<int, 4>> flips;    // { (col?, row/col index, L, R), ... }
                                    // ex: (0, row-index, col-L, col-R)
                                    // ex: (1, col-index, row-L, row-R)

    // transform matrix _A to O, O(N*M)
    // return possibility
    bool solve(int n, int m, const vector<vector<char>>& _A) {
        N = n;
        M = m;
        A = _A;

        rowColor = vector<int>(N);
        colColor = vector<int>(M);
        usedRow = vector<bool>(N);
        usedCol = vector<bool>(M);
        edges = vector<int>(N + M + 1);

        flips.clear();

        int total = 0, colorN = 0;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (_A[i][j] == -1)
                    A[i][j] = 0;
                total += A[i][j];
            }
        }

        // paint color
        // if A[i][j] == 1 then there is an edge between left i and right j in bipartite graph
        for (int i = 0; i < N; i++) {
            if (!rowColor[i])
                colorRow(i, ++colorN);
        }
        for (int i = 0; i < M; i++) {
            if (!colColor[i])
                colorCol(i, ++colorN);
        }
        for (int i = 1; i <= colorN; i++) {
            edges[i] >>= 1;
        }
        //assert(accumulate(edges.begin() + 1, edges.begin() + colorN + 1, 0) == total);

        if (total & 1) {                        // need extra edge
            array<int, 3> best = { 2, -1, -1 }; // (w, row, col)

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    if (_A[i][j] == -1) {       // don't care
                        int u = rowColor[i];
                        int v = colColor[j];
                        int w = 0;

                        if (u == v)
                            w = ((edges[u] & 1) == 0);
                        else
                            w = ((edges[u] & 1) == 0 && (edges[v] & 1) == 0);

                        best = min(best, { w, i, j });
                    }
                }
            }

            if (best[0] == 2) {
                flips.clear();
                return false;
            }

            A[best[1]][best[2]] = 1;
        }

        for (int i = 0; i < N; i++) {
            if (!usedRow[i])
                dfsRow(i);
        }

        pair<int, int> tmp(-1, -1);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (A[i][j] == 1) {
                    if (tmp == make_pair(-1, -1)) {
                        tmp = make_pair(i, j);
                    } else {
                        doRow(i, tmp.second, j);
                        doCol(tmp.second, tmp.first, i);
                        tmp = make_pair(-1, -1);
                    }
                }
            }
        }

        //assert(tmp == make_pair(-1, -1));
        return true;
    }

private:
    vector<bool> usedRow, usedCol;  // 
    vector<int> rowColor, colColor; // 
    vector<int> edges;              // edges[color] = the number of color  (0 < color <= N + M)

    void colorRow(int row, int color) {
        rowColor[row] = color;
        for (int c = 0; c < M; c++) {
            if (A[row][c] == 1) {
                ++edges[color];
                if (!colColor[c])
                    colorCol(c, color);
            }
        }
    }

    void colorCol(int col, int color) {
        colColor[col] = color;
        for (int r = 0; r < N; r++) {
            if (A[r][col] == 1) {
                ++edges[color];
                if (!rowColor[r])
                    colorRow(r, color);
            }
        }
    }


    void dfsRow(int row, int prevCol = -1) {
        usedRow[row] = 1;
        int u = -1;

        for (int c = 0; c < M; c++) {
            if (A[row][c] == 1 && !usedCol[c])
                dfsCol(c, row);

            if (c != prevCol && A[row][c] == 1) {
                if (u == -1) {
                    u = c;
                } else {
                    doRow(row, u, c);
                    u = -1;
                }
            }
        }

        if (prevCol != -1 && u != -1 && A[row][prevCol] == 1)
            doRow(row, u, prevCol);
    }

    void dfsCol(int col, int prevRow = -1) {
        usedCol[col] = 1;
        int u = -1;

        for (int r = 0; r < N; r++) {
            if (A[r][col] == 1 && !usedRow[r])
                dfsRow(r, col);

            if (r != prevRow && A[r][col] == 1) {
                if (u == -1) {
                    u = r;
                } else {
                    doCol(col, u, r);
                    u = -1;
                }
            }
        }

        if (prevRow != -1 && u != -1 && A[prevRow][col] == 1)
            doCol(col, u, prevRow);
    }


    void doRow(int row, int i, int j) {
        //assert((0 <= row && row < N) && (0 <= i && i < M) && (0 <= j && j < M));
        //assert(i != j);
        flips.push_back({ 0, row, i, j });
        A[row][i] ^= 1;
        A[row][j] ^= 1;
    }

    void doCol(int col, int i, int j) {
        //assert((0 <= col && col < M) && (0 <= i && i < N) && (0 <= j && j < N));
        //assert(i != j);
        flips.push_back({ 1, col, i, j });
        A[i][col] ^= 1;
        A[j][col] ^= 1;
    }
};
