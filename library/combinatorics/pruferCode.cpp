#include <functional>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

#include "pruferCode.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

bool isEqual(vector<vector<int>> edgesL, vector<vector<int>> edgesR) {
    if (edgesL.size() != edgesR.size())
        return false;

    for (auto u = 0; u < int(edgesL.size()); u++) {
        sort(edgesL[u].begin(), edgesL[u].end());
        sort(edgesR[u].begin(), edgesR[u].end());
        if (edgesL[u] != edgesR[u])
            return false;
    }

    return true;
}

void testPruferCode() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Prufer Code ---------------------------------" << endl;
    {
        vector<vector<int>> edges{
            { 3 },
            { 3 },
            { 3 },
            { 0, 1, 2, 4 },
            { 3, 5 },
            { 4 }
        };
        vector<int> gt{ 3, 3, 3, 4 };
        auto ans = PruferCode::convertTreeToPruferCode(edges);
        //cout << ans << endl;
        assert(ans == gt);

        auto tree = PruferCode::convertPruferCodeToTree(ans);
        if (!isEqual(edges, tree)) {
            cout << "Mismatched : " << endl;
            cout << edges << endl;
            cout << tree << endl;
        }
        assert(isEqual(edges, tree));
    }
    {
        vector<vector<int>> edges{
            { 2, 4 },
            { 3 },
            { 0, 3 },
            { 1, 2, 5 },
            { 0 },
            { 3 }
        };
        vector<int> gt{ 3, 0, 2, 3 };
        auto ans = PruferCode::convertTreeToPruferCode(edges);
        //cout << ans << endl;
        assert(ans == gt);

        auto tree = PruferCode::convertPruferCodeToTree(ans);
        if (!isEqual(edges, tree)) {
            cout << "Mismatched : " << endl;
            cout << edges << endl;
            cout << tree << endl;
        }
        assert(isEqual(edges, tree));
    }
    {
        vector<vector<int>> edges{
            { 1, 2 },
            { 0 },
            { 0, 4 },
            { 4 },
            { 2, 3 }
        };
        vector<int> gt{ 0, 2, 4 };
        auto ans = PruferCode::convertTreeToPruferCode(edges);
        //cout << ans << endl;
        assert(ans == gt);

        auto tree = PruferCode::convertPruferCodeToTree(ans);
        if (!isEqual(edges, tree)) {
            cout << "Mismatched : " << endl;
            cout << edges << endl;
            cout << tree << endl;
        }
        assert(isEqual(edges, tree));
    }

    cout << "OK!" << endl;
}
