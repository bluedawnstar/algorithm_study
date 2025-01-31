#pragma once

struct SudokuBoard {
    vector<array<int8_t,9>> board;  // 9x9

    SudokuBoard() : board(9) {
    }

    explicit SudokuBoard(const vector<string>& in) : board(9) {
        set(in);
    }

    explicit SudokuBoard(const vector<vector<int>>& in) : board(9) {
        set(in);
    }

    void set(const vector<string>& in) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if ('1' <= in[i][j] && in[i][j] <= '9')
                    board[i][j] = in[i][j] - '0';
                else
                    board[i][j] = 0;
            }
        }
    }

    template <typename T>
    void set(const vector<vector<T>>& in) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (1 <= in[i][j] && in[i][j] <= 9)
                    board[i][j] = static_cast<int8_t>(in[i][j]);
                else
                    board[i][j] = 0;
            }
        }
    }

    array<int8_t, 9>& operator[](int row) {
        return board[row];
    }

    template <typename T>
    vector<vector<T>> get(T/*dummpy value for type deduction*/) {
        vector<vector<T>> res(9, vector<T>(9));
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                res[i][j] = static_cast<T>(board[i][j]);
            }
        }
        return res;
    }
};

struct SudokuBlock {
    vector<array<int8_t,9>> cnt;   // [index][digit - 1] = the number of a digit

    SudokuBlock() : cnt(9) {
    }

    array<int8_t,9>& operator[](int index) {
        return cnt[index];
    }

    bool isValid() const {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (cnt[i][j] > 1)
                    return false;
            }
        }
        return true;
    }

    bool isSolved() const {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (cnt[i][j] != 1)
                    return false;
            }
        }
        return true;
    }

    bool add(int index, int digit) {
        return ++cnt[index][digit - 1] <= 1;
    }
};

struct SudokuState {
    SudokuBoard board;
    SudokuBlock rows;
    SudokuBlock cols;
    SudokuBlock squares;

    bool set(const vector<string>& in) {
        board.set(in);
        return update();
    }

    template <typename T>
    bool set(const vector<vector<T>>& in) {
        board.set(in);
        return update();
    }


    bool isValid() const {
        return rows.isValid() && cols.isValid() && squares.isValid();
    }

    bool isSolved() const {
        return rows.isSolved() && cols.isSolved() && squares.isSolved();
    }


    bool update(int row, int col, int x) {
        board[row][col] = x;
        if (x > 0) {
            return rows.add(row, x)
                && cols.add(col, x)
                && squares.add((row / 3) * 3 + (col / 3), x);
        }
        return true;
    }

private:
    bool update() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] > 0) {
                    if (!rows.add(i, board[i][j])
                     || !cols.add(j, board[i][j])
                     || !squares.add((i / 3) * 3 + (j / 3), board[i][j]))
                        return false;
                }
            }
        }
        return true;
    }
};

struct SudokuCellFlag {
    vector<array<int16_t,9>> candidateFlag; // [row][col] = bit_flag
    vector<array<int8_t,9>> fixedValue;     // [row][col] = fixed_value
    vector<int16_t> fixedFlagRow;           // [row] = bit_flag
    vector<int16_t> fixedFlagCol;           // [col] = bit_flag
    vector<int16_t> fixedFlagSquare;        // [idx] = bit_flag

    SudokuCellFlag()
            : candidateFlag(9), fixedValue(9), fixedFlagRow(9), fixedFlagCol(9), fixedFlagSquare(9) {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                candidateFlag[i][j] = (1 << 9) - 1;
    }

    bool init(SudokuBoard& board) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] > 0) {
                    if (!add(i, j, board[i][j]))
                        return false;
                }
            }
        }
        return true;
    }

    //---

    bool add(int row, int col, int x) {
        if (fixedValue[row][col] == x)
            return true;

        int R = row / 3;
        int C = col / 3;

        int bit = 1 << (x - 1);
        if (fixedValue[row][col] > 0 || (candidateFlag[row][col] & bit) == 0)
            return false;

        candidateFlag[row][col] = 0;
        fixedValue[row][col] = x;
        return updateRow(row, x)
            && updateCol(col, x)
            && updateSquare(R, C, x);
    }

    // return (row, col, x)
    tuple<int, int, int> findConfirmedCell() {
        auto ret = findCellWithOneBitFlag();
        if (get<0>(ret) >= 0)
            return ret;

        ret = findConfirmedCellInRow();
        if (get<0>(ret) >= 0)
            return ret;

        ret = findConfirmedCellInCol();
        if (get<0>(ret) >= 0)
            return ret;

        ret = findConfirmedCellInSquare();
        if (get<0>(ret) >= 0)
            return ret;

        return make_tuple(-1, -1, -1);
    }


    // return { (row, col), ... }
    vector<pair<int, int>> getTrialOrder() {
        vector<pair<int, int>> res;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (fixedValue[i][j] <= 0)
                    res.emplace_back(i, j);
            }
        }
        sort(res.begin(), res.end(), [this](const pair<int, int>& a, const pair<int, int>& b) {
            int bitA = SudokuCellFlag::bitCount(candidateFlag[a.first][a.second]);
            int bitB = SudokuCellFlag::bitCount(candidateFlag[b.first][b.second]);
            if (bitA != bitB)
                return bitA < bitB;
            if (a.first != b.first)
                return a.first < b.first;
            return a.second < b.second;
        });
        return res;
    }

private:
    bool updateRow(int row, int x) {
        int bit = 1 << (x - 1);
        if (fixedFlagRow[row] & bit)
            return false;

        for (int i = 0; i < 9; i++) {
            candidateFlag[row][i] &= ~bit;
            if (candidateFlag[row][i] == 0 && fixedValue[row][i] <= 0)
                return false;
        }
        fixedFlagRow[row] |= bit;
        return true;
    }

    bool updateCol(int col, int x) {
        int bit = 1 << (x - 1);
        if (fixedFlagCol[col] & bit)
            return false;

        for (int i = 0; i < 9; i++) {
            candidateFlag[i][col] &= ~bit;
            if (candidateFlag[i][col] == 0 && fixedValue[i][col] <= 0)
                return false;
        }
        fixedFlagCol[col] |= bit;
        return true;
    }

    bool updateSquare(int R, int C, int x) {
        int idx = R * 3 + C;

        R *= 3;
        C *= 3;
        int bit = 1 << (x - 1);
        if (fixedFlagSquare[idx] & bit)
            return false;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                candidateFlag[R + i][C + j] &= ~bit;
                if (candidateFlag[R + i][C + j] == 0 && fixedValue[R + i][C + j] <= 0)
                    return false;
            }
        }
        fixedFlagSquare[idx] |= bit;
        return true;
    }


    static int bitIndex(int x) {
        for (int i = 0; x; i++, x >>= 1) {
            if (x & 1)
                return i;
        }
        return -1;
    }

    static int bitCount(int x) {
        int res = 0;
        for (; x; x &= x - 1) {
            res++;
        }
        return res;
    }

    // return (row, col, x)
    tuple<int, int, int> findCellWithOneBitFlag() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (fixedValue[i][j] > 0)
                    continue;
                auto x = candidateFlag[i][j];
                if (x && (x & (x - 1)) == 0)
                    return make_tuple(i, j, SudokuCellFlag::bitIndex(x) + 1);
            }
        }
        return make_tuple(-1, -1, -1);
    }

    // return (row, col, x)
    tuple<int, int, int> findConfirmedCellInRow() {
        for (int i = 0; i < 9; i++) {
            for (int bit = (1 << 8), x = 9; bit; bit >>= 1, x--) {
                if (fixedFlagRow[i] & bit)
                    continue;

                int first = -1;
                for (int j = 0; j < 9; j++) {
                    if (fixedValue[i][j] <= 0 && (candidateFlag[i][j] & bit)) {
                        if (first < 0)
                            first = j;
                        else {
                            first = -1;
                            break;
                        }
                    }
                }
                if (first >= 0)
                    return make_tuple(i, first, x);
            }
        }
        return make_tuple(-1, -1, -1);
    }
    
    // return (row, col, x)
    tuple<int,int,int> findConfirmedCellInCol() {
        for (int j = 0; j < 9; j++) {
            for (int bit = (1 << 8), x = 9; bit; bit >>= 1, x--) {
                if (fixedFlagCol[j] & bit)
                    continue;

                int first = -1;
                for (int i = 0; i < 9; i++) {
                    if (fixedValue[i][j] > 0 && (candidateFlag[i][j] & bit)) {
                        if (first < 0)
                            first = i;
                        else {
                            first = -1;
                            break;
                        }
                    }
                }
                if (first >= 0)
                    return make_tuple(first, j, x);
            }
        }
        return make_tuple(-1, -1, -1);
    }

    // return (row, col, x)
    tuple<int,int,int> findConfirmedCellInSquare() {
        for (int index = 0; index < 9; index++) {
            int R = (index / 3) * 3;
            int C = (index % 3) * 3;
            for (int bit = (1 << 8), x = 9; bit; bit >>= 1, x--) {
                if (fixedFlagSquare[index] & bit)
                    continue;

                pair<int, int> first(-1, -1);
                for (int i = 0; i < 9; i++) {
                    int dR = i / 3;
                    int dC = i % 3;

                    if (fixedValue[R + dR][C + dC] > 0 && (candidateFlag[R + dR][C + dC] & bit)) {
                        if (first.first < 0)
                            first = make_pair(R + dR, C + dC);
                        else {
                            first = make_pair(-1, -1);
                            break;
                        }
                    }
                }
                if (first.first >= 0)
                    return make_tuple(first.first, first.second, x);
            }
        }
        return make_tuple(-1, -1, -1);
    }
};

class SudokuSolver {
public:
    SudokuSolver() {
    }

    explicit SudokuSolver(const vector<string>& in) {
        set(in);
    }

    explicit SudokuSolver(const vector<vector<int>>& in) {
        set(in);
    }

    bool set(const vector<string>& in) {
        return input.set(in);
    }

    template <typename T>
    bool set(const vector<vector<T>>& in) {
        return input.set(in);
    }

    //---

    // return (a_solution, solution_count)
    //   solotion_count: 0 - no solution, 1 - unique solution, 2 - two or more solutions
    pair<SudokuBoard, int> solve() const {
        auto state = input;
        if (!state.isValid())
            return make_pair(state.board, 0);

        // given input numbers
        SudokuCellFlag cellFlag;
        if (!cellFlag.init(state.board))
            return make_pair(state.board, 0);

        pair<SudokuBoard, int> result(state.board, 0);

        // step #1 : find cells to have an unique value
        enum SimpleSolverResultT {
            ssrNoSolution,
            ssrSolved,
            ssrNotSolved
        };
        function<int(SudokuState&, SudokuCellFlag&)>
                simpleSolver = [&result](SudokuState& state, SudokuCellFlag& cellFlag) -> SimpleSolverResultT {
            bool solved;
            int row, col, x;
            while ((solved = state.isSolved()) == false) {
                tie(row, col, x) = cellFlag.findConfirmedCell();
                if (row < 0)
                    break;

                if (!state.update(row, col, x) || !cellFlag.add(row, col, x))
                    return ssrNoSolution;
            }

            if (solved) {
                if (++result.second == 1)
                    result.first = state.board;
                return ssrSolved;
            }

            return ssrNotSolved;
        };
        {
            auto newState = state;
            auto newCellFlag = cellFlag;
            if (simpleSolver(newState, newCellFlag) != ssrNotSolved)
                return result;
        }

        // step #2 : find solution(s) with backtracking
        vector<pair<int,int>> order = cellFlag.getTrialOrder();
        function<bool(SudokuState&, SudokuCellFlag&, int)> dfs;
        dfs = [&dfs, &order, &result, &simpleSolver](SudokuState& prevState, SudokuCellFlag& prevCellFlag, int depth) -> bool {
            int row, col;
            row = order[depth].first;
            col = order[depth].second;
            while (prevCellFlag.fixedValue[row][col] > 0) {
                if (++depth >= static_cast<int>(order.size()))
                    return false;
                row = order[depth].first;
                col = order[depth].second;
            }

            for (int x = 1, flag = static_cast<int>(prevCellFlag.candidateFlag[row][col]); flag; x++, flag >>= 1) {
                if ((flag & 1) == 0)
                    continue;

                SudokuState newState = prevState;
                SudokuCellFlag newCellFlag = prevCellFlag;
                // place `x` on (row, col)
                if (!newState.update(row, col, x) || !newCellFlag.add(row, col, x))
                    continue;

                switch (simpleSolver(newState, newCellFlag)) {
                case ssrSolved:
                    // backtracking stops if thre are two or more solutions
                    if (result.second >= 2)
                        return true;
                    break;
                case ssrNotSolved:
                    // try more with backtracking
                    if (dfs(newState, newCellFlag, depth + 1))
                        return true;
                    break;
                }
            }
            return false;
        };
        dfs(state, cellFlag, 0);
        return result;
    }

    //---

    static pair<SudokuBoard, int> solve(const vector<string>& in) {
        SudokuSolver solver(in);
        return solver.solve();
    }

    static pair<SudokuBoard, int> solve(const vector<vector<int>>& in) {
        SudokuSolver solver(in);
        return solver.solve();
    }

private:
    SudokuState input;
};
