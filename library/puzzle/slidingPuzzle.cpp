#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "slidingPuzzle.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <numeric>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static void shuffle(SlidingPuzzle& puzzle) {
    int rowN = puzzle.getRowSize();
    int colN = puzzle.getColSize();
    for (int i = 0; i < rowN * colN * 10; i++) {
        switch (RandInt32::get() & 3) {
        case 0:
            puzzle.moveHoleUp();
            break;
        case 1:
            puzzle.moveHoleDown();
            break;
        case 2:
            puzzle.moveHoleLeft();
            break;
        default:
            puzzle.moveHoleRight();
        }
    }
}

static void shuffleToUnsolvableState(SlidingPuzzle& puzzle) {
    puzzle.initToUnsolvableState();
    shuffle(puzzle);
}

ostream& operator <<(ostream& os, const SlidingPuzzle& puzz) {
    int rowN = puzz.getRowSize();
    int colN = puzz.getColSize();

    for (int i = 0; i < rowN; i++) {
        os << puzz.get(i, 0);
        for (int j = 1; j < colN; j++) {
            os << ' ' << puzz.get(i, j);
        }
        os << endl;
    }

    return os;
}

void testSlidingPuzzle() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sliding Puzzle ------------------------" << endl;
    {
        const int T = 100;
        for (int tc = 0; tc < T; tc++) {
            int rowN = RandInt32::get() % 10 + 2;
            int colN = RandInt32::get() % 10 + 2;

            bool solvable = ((RandInt32::get() & 1) == 1);

            SlidingPuzzle puzzle(rowN, colN);
            if (solvable)
                shuffle(puzzle);
            else
                shuffleToUnsolvableState(puzzle);

            bool possible = puzzle.canSolve();
            bool ok = puzzle.solve() && puzzle.isDone();
            if (possible != solvable) {
                cout << "Mismatched at " << __LINE__ << ", " << tc << " : " << rowN << " x " << colN << ", possible=" << possible << ", solvable=" << solvable << endl;
                cout << "---" << endl
                     << puzzle << endl;
            }
            if (ok != solvable) {
                cout << "Failed at " << __LINE__ << ", " << tc << " : " << rowN << " x " << colN << endl;
                cout << "---" << endl
                     << puzzle << endl;
            }
            assert(possible == solvable);
            assert(ok == solvable && possible == solvable);
        }
    }
    cout << "OK!" << endl;
}
