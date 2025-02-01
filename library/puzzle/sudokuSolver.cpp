#include <cstdint>
#include <cstring>
#include <tuple>
#include <memory>
#include <functional>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "sudokuSolver.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include "../common/iostreamhelper.h"

static ostream& operator <<(ostream& os, const vector<vector<int>>& rhs) {
    os << "{ ";
    if (!rhs.empty())
        os << rhs[0] << endl;
    for (int i = 1; i < (int)rhs.size(); i++)
        os << ", " << rhs[i] << endl;
    os << " }";

    return os;
}

static void solve(int line, const vector<string>& in, const vector<vector<int>>& gt) {
    auto res = SudokuSolver::solve(in);
    auto ans = get<0>(res).get<int>();
    if (ans != gt) {
        cout << "Mismatched at " << line << " : the number of solutions = " << get<1>(res) << endl
             << "ans = " << endl << ans << endl
             << "gt = " << endl << gt << endl;
    }
    cout << "difficulty = " << get<2>(res) << endl;
    assert(get<1>(res) == 1);
    assert(ans == gt);
}

static void solve0(int line, const vector<string>& in) {
    auto res = SudokuSolver::solve(in);
    if (get<1>(res) != 0) {
        cout << "Mismatched at " << line << " : the number of solutions = " << get<1>(res) << endl;
    }
    assert(get<1>(res) == 0);
}

static void solve2(int line, const vector<string>& in) {
    auto res = SudokuSolver::solve(in);
    if (get<1>(res) != 2) {
        cout << "Mismatched at " << line << " : the number of solutions = " << get<1>(res) << endl;
    }
    assert(get<1>(res) == 2);
}

void testSudokuSolver() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sudoku Solver ------------------------" << endl;
    cout << "*** solve with basic method" << endl;
    {
        vector<string> in{
            "__2___9__",
            "32______7",
            "51__438__",
            "____3___1",
            "7__614__3",
            "2___7____",
            "__315__26",
            "1______98",
            "__6___4__"
        };
        solve0(__LINE__, in);   // no solution
    }
    {
        vector<string> in{
            "__2___9__",
            "_________",
            "_________",
            "_________",
            "_________",
            "_________",
            "_________",
            "_________",
            "_________",
        };
        solve2(__LINE__, in);   // two or more solutions
    }
    // medium
    {
        vector<string> in{
            "__2___9__",
            "36______7",
            "51__438__",
            "____3___1",
            "7__614__3",
            "2___7____",
            "__315__26",
            "1______98",
            "__6___4__"
        };
        vector<vector<int>> gt{
            {8,7,2,5,6,1,9,3,4},
            {3,6,4,2,9,8,1,5,7},
            {5,1,9,7,4,3,8,6,2},
            {6,4,8,9,3,2,5,7,1},
            {7,9,5,6,1,4,2,8,3},
            {2,3,1,8,7,5,6,4,9},
            {4,8,3,1,5,9,7,2,6},
            {1,5,7,4,2,6,3,9,8},
            {9,2,6,3,8,7,4,1,5}
        };
        solve(__LINE__, in, gt);
    }
    // medium
    {
        vector<string> in{
            "542____6_",
            "__76_____",
            "36___8__7",
            "__6_29___",
            "__3_5_1__",
            "___76_9__",
            "7__1___49",
            "_____67__",
            "_5____312"
        };
        vector<vector<int>> gt{
            {5,4,2,9,7,3,8,6,1},
            {8,9,7,6,1,2,4,5,3},
            {3,6,1,5,4,8,2,9,7},
            {4,1,6,3,2,9,5,7,8},
            {9,7,3,8,5,4,1,2,6},
            {2,8,5,7,6,1,9,3,4},
            {7,2,8,1,3,5,6,4,9},
            {1,3,4,2,9,6,7,8,5},
            {6,5,9,4,8,7,3,1,2},
        };
        solve(__LINE__, in, gt);
    }
    // hard
    {
        vector<string> in{
            "_______1_",
            "4________",
            "_2_______",
            "____5_4_7",
            "__8___3__",
            "__1_9____",
            "3__4__2__",
            "_5_1_____",
            "___8_6___"
        };
        vector<vector<int>> gt{
            {6,9,3,7,8,4,5,1,2},
            {4,8,7,5,1,2,9,3,6},
            {1,2,5,9,6,3,8,7,4},
            {9,3,2,6,5,1,4,8,7},
            {5,6,8,2,4,7,3,9,1},
            {7,4,1,3,9,8,6,2,5},
            {3,1,9,4,7,5,2,6,8},
            {8,5,6,1,2,9,7,4,3},
            {2,7,4,8,3,6,1,5,9},
        };
        solve(__LINE__, in, gt);
    }
    cout << "*** solve with basic and backtracking methods" << endl;
    // hardest
    {
        // from https://www.sudokuwiki.org/Arto_Inkala_Sudoku
        vector<string> in{
            "8________",
            "__36_____",
            "_7__9_2__",
            "_5___7___",
            "____457__",
            "___1___3_",
            "__1____68",
            "__85___1_",
            "_9____4__"
        };
        vector<vector<int>> gt{
            {8,1,2,7,5,3,6,4,9},
            {9,4,3,6,8,2,1,7,5},
            {6,7,5,4,9,1,2,8,3},
            {1,5,4,2,3,7,8,9,6},
            {3,6,9,8,4,5,7,2,1},
            {2,8,7,1,6,9,5,3,4},
            {5,2,1,9,7,4,3,6,8},
            {4,3,8,5,2,6,9,1,7},
            {7,9,6,3,1,8,4,5,2},
        };
        solve(__LINE__, in, gt);
    }
    // two or more solutions
    {
        // from https://namu.wiki/w/%EC%8A%A4%EB%8F%84%EC%BF%A0
        vector<string> in{
            "_________",
            "_12_34567",
            "_345_6182",
            "__1_582_6",
            "__86____1",
            "_2___7_5_",
            "__37_5_28",
            "_8__6_7__",
            "2_7_83615"
        };
        vector<vector<int>> gt{
            {5,7,6,8,2,1,3,4,9},
            {8,1,2,9,3,4,5,6,7},
            {9,3,4,5,7,6,1,8,2},
            {7,4,1,3,5,8,2,9,6},
            {3,5,8,6,9,2,4,7,1},
            {6,2,9,1,4,7,8,5,3},
            {4,6,3,7,1,5,9,2,8},
            {1,8,5,2,6,9,7,3,4},
            {2,9,7,4,8,3,6,1,5}
        };
        solve(__LINE__, in, gt);

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (in[i][j] < '1' || in[i][j] > '9')
                    continue;

                auto x = in[i][j];
                in[i][j] = '_';
                solve2(__LINE__, in);   // two or more solutions
                in[i][j] = x;
            }
        }
    }

    cout << "OK!" << endl;
}
