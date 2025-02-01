#include <cstdint>
#include <cstring>
#include <tuple>
#include <random>
#include <memory>
#include <functional>
#include <numeric>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "sudokuGenerator.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include "../common/iostreamhelper.h"

template <typename VV>
static void print(ostream& os, const VV& rhs) {
    for (int i = 0; i < 9; i++) {
        os << "  ";
        for (int j = 0; j < 9; j++) {
            int x = rhs[i][j];
            if (x)
                os << x;
            else
                os << '_';
        }
        os << endl;
    }
}

static ostream& operator <<(ostream& os, const vector<vector<int>>& rhs) {
    print(os, rhs);
    return os;
}

static ostream& operator <<(ostream& os, const SudokuBoard& rhs) {
    print(os, rhs);
    return os;
}

template <typename VV>
static void solve(int line, const VV& in) {
    auto res = SudokuSolver::solve(in);
    auto ans = get<0>(res).get<int>();

    cout << "--- Q ---" << endl << in;
    cout << "--- A ---" << endl << ans;
    cout << "difficulty = " << get<2>(res) << endl;
    assert(get<1>(res) == 1);
}

void testSudokuGenerator() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sudoku Generator ------------------------" << endl;
    {
        auto Q = SudokuGenerator::generate(0);
        solve(__LINE__, Q);
    }
    {
        auto Q = SudokuGenerator::generate(1);
        solve(__LINE__, Q);
    }
#ifndef _DEBUG
    {
        auto Q = SudokuGenerator::generate(50);
        solve(__LINE__, Q);
    }
#endif
    cout << "OK!" << endl;
}
