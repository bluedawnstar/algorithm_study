#pragma once

class SudokuBoard {
public:
    SudokuBoard() : board(9) {
        fill(0);
    }

    const array<int8_t, 9>& operator[](int row) const {
        return board[row];
    }

    array<int8_t, 9>& operator[](int row) {
        return board[row];
    }

    void fill(int x) {
        for (int i = 0; i < 9; i++)
            board[i].fill(static_cast<int8_t>(x));
    }

    bool isSolved() const {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] <= 0)
                    return false;
            }
        }
        return true;
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

    template <typename T>
    vector<vector<T>> get() {
        return get(T());
    }

private:
    vector<array<int8_t, 9>> board;  // 9x9
};

class SudokuDigitCounter {
public:
    SudokuDigitCounter() : cnt(9) {
        fill(0);
    }

    const array<int8_t, 9>& operator[](int index) const {
        return cnt[index];
    }

    array<int8_t, 9>& operator[](int index) {
        return cnt[index];
    }

    void fill(int x) {
        for (int i = 0; i < 9; i++)
            cnt[i].fill(static_cast<int8_t>(x));
    }

private:
    vector<array<int8_t, 9>> cnt;   // [index][digit - 1] = the number of a digit
};

class SudokuDigitFlag {
public:
    SudokuDigitFlag() : flag(9) {
        init();
    }

    const array<int16_t, 9>& operator[](int index) const {
        return flag[index];
    }

    array<int16_t, 9>& operator[](int index) {
        return flag[index];
    }

    void init() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++)
                flag[i][j] = BIT_FLAG_ALL;
        }
    }

private:
    vector<array<int16_t, 9>> flag;  // [row][col] = bit flag
    enum {
        BIT_FLAG_ALL = (1 << 9) - 1
    };
};

class SudokuState {
public:
    void init() {
        board.fill(0);
        candidateFlag.init();
        candidateRows.fill(9);
        candidateCols.fill(9);
        candidateSquares.fill(9);
    }

    bool set(const vector<string>& in) {
        init();

        bool res = true;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if ('1' <= in[i][j] && in[i][j] <= '9') {
                    if (!update(i, j, in[i][j] - '0'))
                        res = false;
                } else {
                    board[i][j] = 0;
                }
            }
        }
        return res;
    }

    template <typename VV>
    bool set(const VV& in) {
        init();

        bool res = true;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (1 <= in[i][j] && in[i][j] <= 9) {
                    if (!update(i, j, static_cast<int>(in[i][j])))
                        res = false;
                } else {
                    board[i][j] = 0;
                }
            }
        }
        return res;
    }


    const SudokuBoard& getBoard() const {
        return board;
    }

    bool isSolved() const {
        return board.isSolved();
    }


    bool update(int row, int col, int x) {
        if (board[row][col] == x)
            return true;

        int flag = getCandidateFlag(row, col);
        if (board[row][col] > 0 || (flag & (1 << (x - 1))) == 0)
            return false;

        clearCandidateFlagWithMask(row, col, flag); // clear all flags of cell(row, col)
        board[row][col] = x;            // must set board[row][col] before calling updateRow(), updateCol() and updateSquare()
        return updateRow(row, x)
            && updateCol(col, x)
            && updateSquare(row / 3, col / 3, x);
    }

    // return updatedCount (<0: failed, 0: no updated, 1>=: updated)
    int findAndUpdate() {
        int updatedCount = 0;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] > 0)
                    continue;

                int flag = getCandidateFlag(i, j);
                if (flag == 0)
                    return -1;

                int digit = 0;
                if ((flag & (flag - 1)) == 0) {
                    // only one candidate in a cell(i, j)
                    digit = SudokuState::bitIndex(flag) + 1;
                } else {
                    // find a digit that is the only candidate in a row, column, or square
                    for (int x = 0; flag; x++, flag >>= 1) {
                        if ((flag & 1) == 0)
                            continue;
                        if (candidateRows[i][x] == 1
                         || candidateCols[j][x] == 1
                         || candidateSquares[(i / 3) * 3 + (j / 3)][x] == 1) {
                            digit = x + 1;
                            break;
                        }
                    }
                }
                if (digit > 0) {
                    if (!update(i, j, digit))
                        return -1;
                    updatedCount++;
                }
            }
        }
        return updatedCount;
    }

    // return { (row, col), ... }
    vector<pair<int, int>> getTrialOrder() {
        vector<pair<int, int>> res;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] <= 0)
                    res.emplace_back(i, j);
            }
        }
        sort(res.begin(), res.end(), [this](const pair<int, int>& a, const pair<int, int>& b) {
            int bitCntA = SudokuState::bitCount(candidateFlag[a.first][a.second]);
            int bitCntB = SudokuState::bitCount(candidateFlag[b.first][b.second]);
            if (bitCntA != bitCntB)
                return bitCntA < bitCntB;
            if (a.first != b.first)
                return a.first < b.first;
            return a.second < b.second;
        });
        return res;
    }

    int getCandidateFlag(int row, int col) const {
        return static_cast<int>(candidateFlag[row][col]);
    }

private:
    bool clearCandidateFlag(int row, int col, int x) {
        int bit = 1 << --x;
        if (candidateFlag[row][col] & bit) {
            candidateFlag[row][col] &= ~bit;
            candidateRows[row][x]--;
            candidateCols[col][x]--;
            candidateSquares[(row / 3) * 3 + (col / 3)][x]--;
        }
        return candidateFlag[row][col] != 0 || board[row][col] > 0; // return if the cell is valid
    }

    void clearCandidateFlagWithMask(int row, int col, int mask) {
        for (int x = 1; mask; x++, mask >>= 1) {
            if (mask & 1)
                clearCandidateFlag(row, col, x);
        }
    }

    bool updateRow(int row, int x) {
        for (int i = 0; i < 9; i++) {
            if (!clearCandidateFlag(row, i, x))
                return false;
        }
        return true;
    }

    bool updateCol(int col, int x) {
        for (int i = 0; i < 9; i++) {
            if (!clearCandidateFlag(i, col, x))
                return false;
        }
        return true;
    }

    bool updateSquare(int R, int C, int x) {
        R *= 3;
        C *= 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (!clearCandidateFlag(R + i, C + j, x))
                    return false;
            }
        }
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

private:
    SudokuBoard        board;
    SudokuDigitFlag    candidateFlag;   // [row][col] = bit_flag
    SudokuDigitCounter candidateRows;   // [row][digit - 1] = the number of a digit
    SudokuDigitCounter candidateCols;   // [col][digit - 1] = the number of a digit
    SudokuDigitCounter candidateSquares;// [3x3_bloock_index][digit - 1] = the number of a digit

    friend class SudokuSolver;
};

class SudokuSolver {
public:
    SudokuSolver() : inputValid(true) {
    }

    SudokuSolver(const vector<string>& in) {
        set(in);
    }

    template <typename VV>
    SudokuSolver(const VV& in) {
        set(in);
    }

    bool set(const vector<string>& in) {
        return inputValid = input.set(in);
    }

    template <typename VV>
    bool set(const VV& in) {
        return inputValid = input.set(in);
    }

    //---

    // return (a_solution, solution_count, backtracking_depth)
    //   solotion_count: 0 - no solution, 1 - unique solution, 2 - two or more solutions
    tuple<SudokuBoard,int,int> solve() const {
        auto state = input;
        tuple<SudokuBoard, int, int> result(state.board, 0, 0);

        if (!inputValid)
            return result;

        // step #1 : find cells to have an unique value
        enum SimpleSolverResultT {
            ssrNoSolution,
            ssrSolved,
            ssrNotSolved
        };
        function<int(SudokuState&)> simpleSolver = [&result](SudokuState& state) -> SimpleSolverResultT {
            bool solved;
            while ((solved = state.isSolved()) == false) {
                int updatedCnt = state.findAndUpdate();
                if (updatedCnt < 0)
                    return ssrNoSolution;
                else if (updatedCnt == 0)
                    break;
            }

            if (solved) {
                if (++get<1>(result) == 1)
                    get<0>(result) = state.board;
                return ssrSolved;
            }

            return ssrNotSolved;
        };
        {
            auto newState = state;
            if (simpleSolver(newState) != ssrNotSolved)
                return result;
        }

        // step #2 : find solution(s) with backtracking
        vector<pair<int, int>> order = state.getTrialOrder();
        get<2>(result) = static_cast<int>(order.size());

        function<bool(SudokuState&, int)> dfs;
        dfs = [&dfs, &order, &simpleSolver, &result](SudokuState& prevState, int depth) -> bool {
            int row = order[depth].first;
            int col = order[depth].second;
            while (prevState.board[row][col] > 0) {
                if (++depth >= static_cast<int>(order.size()))
                    return false;
                row = order[depth].first;
                col = order[depth].second;
            }

            for (int x = 1, flag = prevState.getCandidateFlag(row, col); flag; x++, flag >>= 1) {
                if ((flag & 1) == 0)
                    continue;

                SudokuState newState = prevState;
                // place `x` on (row, col)
                if (!newState.update(row, col, x))
                    continue;

                switch (simpleSolver(newState)) {
                case ssrSolved:
                    // backtracking stops if thre are two or more solutions
                    if (get<1>(result) >= 2)
                        return true;
                    break;
                case ssrNotSolved:
                    // try more with backtracking
                    if (dfs(newState, depth + 1))
                        return true;
                    break;
                }
            }
            return false;
        };
        dfs(state, 0);
        return result;
    }

    //---

    // return (a_solution, solution_count, backtracking_depth)
    //   solotion_count: 0 - no solution, 1 - unique solution, 2 - two or more solutions
    static tuple<SudokuBoard,int,int> solve(const vector<string>& in) {
        SudokuSolver solver(in);
        return solver.solve();
    }

    // return (a_solution, solution_count, backtracking_depth)
    //   solotion_count: 0 - no solution, 1 - unique solution, 2 - two or more solutions
    template <typename VV>
    static tuple<SudokuBoard,int,int> solve(const VV& in) {
        SudokuSolver solver(in);
        return solver.solve();
    }

private:
    bool inputValid;
    SudokuState input;
};
