#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "transformMatrix.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"
#include "../../common/rand.h"

void testTransformMatrix() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Transform Matrix ---------------------------" << endl;
    {
        const vector<vector<int>> A{
            { 1, 2 },
            { 0, 1 }
        };
        const vector<vector<int>> B{
            { 0, 0 },
            { 0, 0 }
        };
        const int X = 2;
        assert(TransformMatrix::canTransform(A, B, X) == true);
        assert(TransformMatrix::canTransform2(A, B, X) == true);
    }
    {
        const vector<vector<int>> A{
            { 1, 2 },
            { 0, 1 }
        };
        const vector<vector<int>> B{
            {  0, 0 },
            { -1, 0 }
        };
        const int X = 2;
        assert(TransformMatrix::canTransform(A, B, X) == false);
        assert(TransformMatrix::canTransform2(A, B, X) == false);
    }
    {
        const vector<vector<int>> A{
            { 1, 1 },
            { 2, 2 },
            { 3, 3 }
        };
        const vector<vector<int>> B{
            { 1, 0 },
            { 2, 0 },
            { 3, 0 }
        };
        const int X = 2;
        assert(TransformMatrix::canTransform(A, B, X) == false);
        assert(TransformMatrix::canTransform2(A, B, X) == false);
    }

    cout << "OK!" << endl;
}
