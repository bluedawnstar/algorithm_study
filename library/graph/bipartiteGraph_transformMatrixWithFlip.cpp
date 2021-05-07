#include <cassert>
#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#include "bipartiteGraph_transformMatrixWithFlip.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testBipartiteGraphTransformMatrixWithFlip() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Transform Matrix with Flip ---------------------------" << endl;
    {
        int N = 3, M = 3;
        vector<vector<char>> A{
            {  1, 1, 0 },
            {  0, 1, 1 },
            { -1, 0, 1 }
        };

        TransformMatrixWithFlip solver;
        bool ok = solver.solve(N, M, A);
        assert(ok && solver.flips.size() == 3);
    }
    {
        int N = 2, M = 2;
        vector<vector<char>> A{
            {  1, -1 },
            { -1, -1 }
        };

        TransformMatrixWithFlip solver;
        bool ok = solver.solve(N, M, A);
        assert(ok && solver.flips.size() == 1);
    }
    {
        int N = 1, M = 3;
        vector<vector<char>> A{
            { 1, 1, 1 }
        };

        TransformMatrixWithFlip solver;
        bool ok = solver.solve(N, M, A);
        assert(ok == false);
    }

    cout << "OK!" << endl;
}
