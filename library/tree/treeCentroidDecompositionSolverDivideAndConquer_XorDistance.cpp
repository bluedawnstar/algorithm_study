#include <memory.h>
#include <functional>
#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "treeCentroidDecompositionSolverDivideAndConquer_XorDistance.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testTreeCentroidDecompositionSolverDivideAndConquer_XorDistance() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Centroid Decomposition Solver by Divide and Conquer for XOR distance -------" << endl;
    {
        TreeCentroidDecompositionSolverDivideAndConquer_XorDistance solver;
        solver.init(3);
        solver.addEdge(0, 1);
        solver.addEdge(1, 2);
        solver.setValue(0, true);
        solver.setValue(2, true);

        vector<int> gt{ 2, 0, 2 };
        solver.solve();
        if (solver.answer != gt)
            cout << "Mismatched at " << __LINE__ << " : " << solver.answer << ", " << gt << endl;
        assert(solver.answer == gt);
    }
    {
        TreeCentroidDecompositionSolverDivideAndConquer_XorDistance solver;
        solver.init(6);
        solver.addEdge(0, 1);
        solver.addEdge(0, 2);
        solver.addEdge(2, 3);
        solver.addEdge(2, 4);
        solver.addEdge(3, 5);
        solver.setValue(3, true);
        solver.setValue(4, true);

        vector<int> gt{ 0, 0, 0, 2, 2, 2 };
        solver.solve();
        if (solver.answer != gt)
            cout << "Mismatched at " << __LINE__ << " : " << solver.answer << ", " << gt << endl;
        assert(solver.answer == gt);
    }
    cout << "OK!" << endl;
}
