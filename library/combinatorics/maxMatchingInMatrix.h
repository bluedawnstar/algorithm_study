#pragma once

struct MaxMatchingInMatrix {
    // row[y] = col index in row y
    // col[x] = row index in col x
    // return matching count
    static int bipartiteMatching(const vector<vector<bool>>& mat, vector<int>& row, vector<int>& col) {
        row = vector<int>(mat.size(), -1);
        col = vector<int>(mat[0].size(), -1);

        int ct = 0;
        for (int i = 0; i < int(mat.size()); i++) {
            vector<bool> seen(mat[0].size());
            if (findMatch(i, mat, row, col, seen))
                ct++;
        }
        return ct;
    }

private:
    // i = row index
    // row[y] = col index in row y
    // col[x] = row index in col x
    static bool findMatch(int i, const vector<vector<bool>>& mat, vector<int>& row, vector<int>& col, vector<bool>& seen) {
        for (int j = 0; j < int(mat[i].size()); j++) {
            if (mat[i][j] && !seen[j]) {
                seen[j] = true;
                if (col[j] < 0 || findMatch(col[j], mat, row, col, seen)) {
                    row[i] = j;
                    col[j] = i;
                    return true;
                }
            }
        }
        return false;
    }
};
