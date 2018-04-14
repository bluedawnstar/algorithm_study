#include <climits>
#include <numeric>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "hamiltonian.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testHamiltonian() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Hamiltonian Cycle & Path ---------------------------" << endl;
    {
        vector<vector<int>> dist = {
            { 0, 1, 10, 1, 10 },
            { 1, 0, 10, 10, 1 },
            { 10, 10, 0, 1, 1 },
            { 1, 10, 1, 0, 10 },
            { 10, 1, 1, 10, 0 }
        };
        auto ansCycle = Hamiltonian<int>::getShortestCycle(dist);
        cout << ansCycle.first << ", " << ansCycle.second << endl;
        assert(ansCycle.first == 5);
    }
    {
        vector<vector<int>> dist = {
            { 8, 1, 6 },
            { 3, 5, 7 },
            { 4, 9, 2 }
        };
        auto ansPath = Hamiltonian<int>::getShortestPath(dist);
        cout << ansPath.first << ", " << ansPath.second << endl;
        assert(ansPath.first == 5);
    }

    cout << "OK" << endl;
}
