#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

#include "assignmentProblem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testHungarian() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Hungarian Algorithm - Assignment Problem ---------" << endl;
    {
        vector<vector<int>> A{
            { 2500, 4000, 3500 },
            { 4000, 6000, 3500 },
            { 2000, 4000, 2500 }
        };
        vector<int> assigned;
        auto ans = AssignmentProblem<int>::doHungarian(A, 3, 3, assigned);
        cout << "assigned = " << assigned << endl;
        assert(ans == 9500);
    }
    {
        vector<vector<int>> A{
            { 1500, 4000, 4500 },
            { 2000, 6000, 3500 },
            { 2000, 4000, 2500 }
        };
        vector<int> assigned;
        auto ans = AssignmentProblem<int>::doHungarian(A, 3, 3, assigned);
        cout << "assigned = " << assigned << endl;
        assert(ans == 8500);
    }
    {
        vector<vector<int>> A{
            { 2, 3, 3, 0 },
            { 3, 2, 3, 0 },
            { 3, 3, 2, 0 },
            { 9, 9, 1, 0 }
        };
        vector<int> assigned;
        auto ans = AssignmentProblem<int>::doHungarian(A, 4, 4, assigned);
        cout << "assigned = " << assigned << endl;
        assert(ans == 5);
    }
    cout << "OK" << endl;
}
