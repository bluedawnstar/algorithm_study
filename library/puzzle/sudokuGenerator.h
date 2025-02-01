#pragma once

#include "sudokuSolver.h"

class SudokuGenerator {
public:
    static SudokuBoard generate(int minDifficulty = 0) {
        SudokuState state;
        state.init();

        vector<tuple<int, int, array<int8_t,9>>> cells(81);
        for (int i = 0, k = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++, k++) {
                get<0>(cells[k]) = i;
                get<1>(cells[k]) = j;
                iota(get<2>(cells[k]).begin(), get<2>(cells[k]).end(), 1);
                shuffle(get<2>(cells[k]).begin(), get<2>(cells[k]).end(), SudokuGenerator::getRng());
            }
        }
        shuffle(cells.begin(), cells.end(), SudokuGenerator::getRng());

        SudokuBoard result;

        function<bool(SudokuState&, int)> dfs;
        dfs = [&dfs, minDifficulty, &cells, &result](SudokuState& prevState, int depth) -> bool {
            int row = get<0>(cells[depth]);
            int col = get<1>(cells[depth]);
            for (int x : get<2>(cells[depth])) {
                SudokuState newState = prevState;
                if (!newState.update(row, col, x))
                    continue;

                auto ans = SudokuSolver::solve(newState.getBoard());
                if (get<1>(ans) == 1) {
                    if (get<2>(ans) >= minDifficulty) {
                        result = newState.getBoard();
                        return true;
                    }
                } else if (get<1>(ans) == 2) {
                    if (dfs(newState, depth + 1))
                        return true;
                }
            }
            return false;
        };
        dfs(state, 0);
        return result;
    }

private:
    static mt19937& getRng() {
        static random_device rd;
        static mt19937 rng(rd());
        return rng;
    }
};
