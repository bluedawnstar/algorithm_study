#pragma once

/*
    https://en.wikipedia.org/wiki/Sliding_puzzle

    rowN >= 2, colN >= 2
*/
class SlidingPuzzle {
    enum Direction {
        dirUp,
        dirDown,
        dirLeft,
        dirRight
    };

    struct InversionCounter {
        struct BIT {
            vector<int> tree;

            explicit BIT(int n) : tree(n + 1) {
            }

            long long sum(int pos) const {
                pos++;

                long long res = 0;
                while (pos > 0) {
                    res += tree[pos];
                    pos &= pos - 1;
                }

                return res;
            }

            void add(int pos, int val) {
                pos++;

                while (pos < int(tree.size())) {
                    tree[pos] += val;
                    pos += pos & -pos;
                }
            }
        };

        // O(R*C*log(R*C))
        static long long count(const vector<vector<int>>& board) {
            int rowN = int(board.size());
            int colN = int(board[0].size());

            BIT bit(rowN * colN);

            long long res = 0;
            for (int i = rowN - 1; i >= 0; i--) {
                for (int j = colN - 1; j >= 0; j--) {
                    if (board[i][j] > 0) {
                        res += bit.sum(board[i][j] - 1);
                        bit.add(board[i][j], 1);
                    }
                }
            }

            return res;
        }
    };

public:
    SlidingPuzzle(int rowN, int colN) : rowN(rowN), colN(colN), board(rowN, vector<int>(colN)), locked(rowN, vector<char>(colN)) {
        init();
    }

    void init() {
        int k = 1;
        for (int i = 0; i < rowN; i++) {
            for (int j = 0; j < colN; j++) {
                board[i][j] = k++;
                locked[i][j] = 0;
            }
        }

        board[rowN - 1][colN - 1] = 0;
        holeRow = rowN - 1;
        holeCol = colN - 1;
    }

    void initToUnsolvableState() {
        int k = 1;
        for (int i = 0; i < rowN; i++) {
            for (int j = 0; j < colN; j++) {
                board[i][j] = k++;
                locked[i][j] = 0;
            }
        }
        swap(board[rowN - 1][colN - 2], board[rowN - 2][colN - 1]);

        board[rowN - 1][colN - 1] = 0;
        holeRow = rowN - 1;
        holeCol = colN - 1;
    }


    //-- status

    int getRowSize() const {
        return rowN;
    }

    int getColSize() const {
        return colN;
    }

    int get(int row, int col) const {
        return board[row][col];
    }

    pair<int,int> find(int X) const {
        for (int i = 0; i < rowN; i++) {
            for (int j = 0; j < colN; j++) {
                if (board[i][j] == X)
                    return make_pair(i, j); // (row, col)
            }
        }
        return make_pair(rowN - 1, colN - 1);
    }

    bool isDone() const {
        int k = 1;
        for (int i = 0; i < rowN; i++) {
            for (int j = 0; j < colN; j++) {
                if (board[i][j] != k)
                    return false;
                if (++k >= rowN * colN)
                    k = 0;
            }
        }
        return true;
    }


    //-- basic movement

    bool moveHoleUp() {
        if (holeRow == 0 || locked[holeRow - 1][holeCol]) {
            return false;
        } else {
            holeRow--;
            swap(board[holeRow][holeCol], board[holeRow + 1][holeCol]);
            return true;
        }
    }

    bool moveHoleDown() {
        if (holeRow + 1 == rowN || locked[holeRow + 1][holeCol]) {
            return false;
        } else {
            holeRow++;
            swap(board[holeRow][holeCol], board[holeRow - 1][holeCol]);
            return true;
        }
    }

    bool moveHoleLeft() {
        if (holeCol == 0 || locked[holeRow][holeCol - 1]) {
            return false;
        } else {
            holeCol--;
            swap(board[holeRow][holeCol], board[holeRow][holeCol + 1]);
            return true;
        }
    }

    bool moveHoleRight() {
        if (holeCol + 1 == colN || locked[holeRow][holeCol + 1]) {
            return false;
        } else {
            holeCol++;
            swap(board[holeRow][holeCol], board[holeRow][holeCol - 1]);
            return true;
        }
    }

    void moveHoleTo(int row, int col) {
        if (holeRow == row && holeCol == col)
            return;

        vector<Direction> path = findPath(holeRow, holeCol, row, col);
        for (auto dir : path) {
            switch (dir) {
            case dirUp:
                moveHoleUp();
                break;
            case dirDown:
                moveHoleDown();
                break;
            case dirLeft:
                moveHoleLeft();
                break;
            case dirRight:
                moveHoleRight();
                break;
            }
        }
    }

    void moveTo(int X, int row, int col) {
        auto posX = find(X);

        if (row == posX.first && col == posX.second)
            return;

        int curR = posX.first;
        int curC = posX.second;

        vector<Direction> path = findPath(posX.first, posX.second, row, col);
        for (auto dir : path) {
            int nextR = curR;
            int nextC = curC;
            switch (dir) {
            case dirUp:
                nextR--;
                break;
            case dirDown:
                nextR++;
                break;
            case dirLeft:
                nextC--;
                break;
            case dirRight:
                nextC++;
                break;
            }

            locked[curR][curC]++;
            moveHoleTo(nextR, nextC);
            locked[curR][curC]--;

            switch (dir) {
            case dirUp:
                moveHoleDown();
                break;
            case dirDown:
                moveHoleUp();
                break;
            case dirLeft:
                moveHoleRight();
                break;
            case dirRight:
                moveHoleLeft();
                break;
            }

            curR = nextR;
            curC = nextC;
        }
    }


    //-- solution

    long long countInversions() const {
        return InversionCounter::count(board);
    }

    bool canSolve() const {
        auto inversionSum = InversionCounter::count(board);
#if 0
        // even polarity
        if (colN % 2)
            return (inversionSum % 2) == 0;
        else
            return ((inversionSum + (rowN - 1 - holeRow)) % 2) == 0;
#else
        // even polarity
        return ((inversionSum + (rowN - 1 - holeRow) * (colN - 1)) % 2) == 0;
#endif
    }

    bool solve() {
        if (rowN <= 1 || colN <= 1)
            return false;

        if (colN >= 3)
            return solveC3();
        else
            return solveC2();
    }

private:
    void clearLock() {
        for (int i = 0; i < rowN; i++) {
            for (int j = 0; j < colN; j++) {
                locked[i][j] = 0;
            }
        }
    }

    vector<Direction> findPath(int srcRow, int srcCol, int dstRow, int dstCol) {
        queue<pair<int, int>> Q;
        vector<vector<pair<int,int>>> prev(rowN, vector<pair<int,int>>(colN, make_pair(-1,-1)));
        vector<vector<bool>> visited(rowN, vector<bool>(colN));

        Q.emplace(srcRow, srcCol);
        visited[srcRow][srcCol] = true;
        while (!Q.empty()) {
            auto curr = Q.front();
            Q.pop();
            if (curr.first == dstRow && curr.second == dstCol)
                break;

            static const int dydx[4][2]{
                { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 }
            };
            for (int i = 0; i < 4; i++) {
                int r = curr.first + dydx[i][0];
                int c = curr.second + dydx[i][1];
                if (r >= 0 && r < rowN && c >= 0 && c < colN && !visited[r][c] && locked[r][c] <= 0) {
                    Q.emplace(r, c);
                    prev[r][c] = curr;
                    visited[r][c] = true;
                }
            }
        }

        int curR = dstRow;
        int curC = dstCol;

        vector<Direction> res;
        while (curR != srcRow || curC != srcCol) {
            int nextR = prev[curR][curC].first;
            int nextC = prev[curR][curC].second;

            if (nextR < curR)
                res.push_back(dirDown);
            else if (nextR > curR)
                res.push_back(dirUp);
            else if (nextC < curC)
                res.push_back(dirRight);
            else
                res.push_back(dirLeft);

            curR = nextR;
            curC = nextC;
        }
        reverse(res.begin(), res.end());
        return res;
    }

    // the number of colomns >= 3
    bool solveC3() {
        clearLock();

        for (int r = 0; r + 3 <= rowN; r++) {
            int start = r * colN;
            /*
              x x x x    2 3 x x
              x x x x => x x x x
              x x x x    x x x x
              x x x x    x x x x
            */
            for (int c = 0; c + 3 <= colN; c++) {
                moveTo(start + 2 + c, r, c);
                locked[r][c]++;
            }
            /*
              2 3 x x    2 3 x x
              x x x x => 1 x x x
              x x x x    x x x x
              x x x x    x x x x
            */
            if (r + 3 == rowN) {
                // to avoid special case
                //  2 x x x
                //  1 x x x
                //  4 x x x
                moveTo(start + colN, r + 2, colN - 1);
            }
            moveTo(start + 1, r + 1, 0);
            locked[r + 1][0]++;
            /*
              2 3 x x    2 3 x 4
              1 x x x => 1 x x x
              x x x x    x x x x
              x x x x    x x x x
            */
            moveTo(start + colN, r, colN - 1);
            locked[r][colN - 1]++;
            /*
              2 3 x 4    2 3   4
              1 x x x => 1 x x x
              x x x x    x x x x
              x x x x    x x x x
            */
            moveHoleTo(r, colN - 2);
            /*
              2 3   4    1 2 3 4
              1 x x x =>   x x x
              x x x x    x x x x
              x x x x    x x x x
            */
            for (int c = colN - 3; c >= 0; c--) {
                locked[r][c]--;
                moveHoleLeft();
                locked[r][c + 1]++;
            }
            locked[r + 1][0]--;
            moveHoleDown();
            locked[r][0]++;
        }

        if (rowN >= 2) {
            int r = rowN - 2;
            int start = r * colN + 1;
            for (int c = 0; c + 2 < colN; c++, start++) {
                /*
                  x x x  =>  4 x x  =>  4 x x
                  x x x      x x x      x   x
                */
                moveTo(start + colN, r, c);
                locked[r][c]++;
                moveHoleTo(r + 1, c + 1);
                if (board[r + 1][c] == start) {
                    locked[r][c]--;
                    /*
                      4 x x  =>    x x
                      1   x      4 1 x
                    */
                    moveHoleLeft();
                    moveHoleUp();
                    /*
                        x x  =>  x x x
                      4 1 x      4   1
                    */
                    moveHoleRight();
                    moveHoleRight();
                    moveHoleDown();
                    moveHoleLeft();
                    /*
                      x x x  =>  4 x x
                      4   1        x 1
                    */
                    moveHoleUp();
                    moveHoleLeft();
                    moveHoleDown();

                    locked[r][c]++;
                }
                /*
                    4 x x  =>  4 1 x
                    x x x        x x
                */
                moveTo(start, r, c + 1);
                locked[r][c + 1]++;
                moveHoleTo(r + 1, c);
                /*
                    4 1 x  =>  1   x
                      x x      4 x x
                */
                locked[r][c]--;
                moveHoleUp();
                locked[r + 1][c]++;
                locked[r][c + 1]--;
                moveHoleRight();
                locked[r][c]++;
            }

            /*
                1   x  =>  1 2 3
                4 x x      4 5
            */
            moveTo(start, r, colN - 2);
            locked[r][colN - 2]++;
            moveHoleTo(rowN - 1, colN - 1);

            if (board[rowN - 1][colN - 2] != rowN * colN - 1)
                return false;
        }

        return true;
    }

    bool solveC2() {
        clearLock();

        int start = 1;
        for (int r = 0; r + 2 < rowN; r++, start += 2) {
            /*
              x x      2 x      2 x
              x x  =>  x x  =>  x 
              x x      x x      x x
            */
            moveTo(start + 1, r, 0);
            locked[r][0]++;
            moveHoleTo(r + 1, 1);
            if (board[r][1] == start) {
                locked[r][0]--;
                /*
                  2 1        2
                  x    =>  x 1
                  x x      x x
                */
                moveHoleUp();
                moveHoleLeft();
                /*
                    2      x 2
                  x 1  =>  x  
                  x x      x 1
                */
                moveHoleDown();
                moveHoleDown();
                moveHoleRight();
                moveHoleUp();
                /*
                  x 2      2  
                  x    =>  x x
                  x 1      x 1
                */
                moveHoleLeft();
                moveHoleUp();
                moveHoleRight();

                locked[r][0]++;
            }
            /*
              2 x      2  
              x x  =>  1 x
              x x      x x
            */
            moveTo(start, r + 1, 0);
            locked[r + 1][0]++;
            moveHoleTo(r, 1);
            /*
              2        1 2
              1 x  =>    x
              x x      x x
            */
            locked[r][0]--;
            moveHoleLeft();
            locked[r][1]++;
            locked[r + 1][0]--;
            moveHoleDown();
            locked[r][0]++;
        }

        /*
          1 2      1 2
            x  =>  3 4
          x x      5  
        */
        moveTo(start, rowN - 2, colN - 2);
        locked[rowN - 2][colN - 2]++;
        moveHoleTo(rowN - 1, colN - 1);

        if (board[rowN - 1][colN - 2] != rowN * colN - 1)
            return false;

        return true;
    }

private:
    int rowN;
    int colN;
    vector<vector<int>> board;
    vector<vector<char>> locked;

    int holeRow;
    int holeCol;
};
