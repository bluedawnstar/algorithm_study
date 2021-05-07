#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "bipartiteGraph_maxMatchingInMatrix.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testBipartiteGraphMaxMatchingInMatrix() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Matching In Matrix -----------------------------------" << endl;
    {
        int N = 5, M = 5;
        vector<vector<bool>> mat{
            { true, true, true, true, true },
            { true, true, true, true, true },
            { true, true, true, true, true },
            { true, true, true, true, true },
            { true, true, true, true, true },
        };
        
        vector<int> row(N);
        vector<int> col(M);
        int ans = MaxMatchingInMatrix::bipartiteMatching(mat, row, col);
        cout << ans << endl;
        cout << "row = " << row << endl;
        cout << "col = " << col << endl;
        assert(ans == 5);
    }
    {
        int N = 5, M = 5;
        vector<vector<bool>> mat{
            {  true, false, true, true, true },
            {  true, false, true, true, true },
            { false,  true, true, true, true },
            { false,  true, true, true, true },
            { false,  true, true, true, true },
        };

        vector<int> row(N);
        vector<int> col(M);
        int ans = MaxMatchingInMatrix::bipartiteMatching(mat, row, col);
        cout << ans << endl;
        cout << "row = " << row << endl;
        cout << "col = " << col << endl;
        assert(ans == 5);
    }
    {
        int N = 5, M = 5;
        vector<vector<bool>> mat{
            {  true, false, false, false,  true },
            { false, false, false,  true, false },
            { false, false, false, false,  true },
            { false, false, false, false,  true },
            { false,  true, false, false,  true },
        };

        vector<int> row(N);
        vector<int> col(M);
        int ans = MaxMatchingInMatrix::bipartiteMatching(mat, row, col);
        cout << ans << endl;
        cout << "row = " << row << endl;
        cout << "col = " << col << endl;
        assert(ans == 4);
    }

    cout << "OK!" << endl;
}
