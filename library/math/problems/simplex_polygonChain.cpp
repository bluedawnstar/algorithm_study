#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "simplex_polygonChain.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"
#include "../../common/rand.h"

void testSimplexPolygonChain() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simplex Problems - Polygon Chain ---------------------------" << endl;
    {
        vector<vector<pair<int, int>>> polygonPoints{
            {
                { 1, 3 },
                { 2, 2 },
                { 3, 3 },
                { 2, 4 },
            },
            {
                { 0, 0 },
                { 2, 0 },
                { 2, 2 },
                { 0, 2 },
            }
        };
        
        vector<PolygonChainSolver::Polygon> polygons(polygonPoints.size());
        for (int i = 0; i < int(polygonPoints.size()); i++)
            polygons[i].points = polygonPoints[i];

        auto ans1 = PolygonChainSolver::solve(polygons);
        auto ans2 = PolygonChainSolver::solve2(polygons);
        if (ans1 != 3 || ans2 != 3) {
            cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
        }
        assert(ans1 == 3 && ans2 == 3);
    }
    cout << "OK!" << endl;
}
