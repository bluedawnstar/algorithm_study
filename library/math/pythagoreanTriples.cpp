#include <tuple>
#include <numeric>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>

using namespace std;

#include "pythagoreanTriples.h"
#include "pythagoreanTriples_finder.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

// https://www.hackerearth.com/problem/algorithm/right-angled-triangle-2-840be98b/

void testPythagoreanTriples() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Pythagorean Triples with Interget-Sides -----------------------" << endl;
    {
        IntegerSidedPythagoreanTriples triples(1'000'000);
        
        vector<pair<long long, long long>> X{
            { 2, 0 },
            { 3, 1 },
            { 5, 2 },
            { 12, 4 },
            { 13, 2 },
            { 15, 5 },
            { 9972, 23 }
        };
        for (auto& it : X) {
            auto X = it.first;
            auto gt = it.second;
            auto ans = triples.count(X);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    {
        vector<pair<long long, long long>> X{
            { 2, 0 },
            { 3, 1 },
            { 5, 2 },
            { 12, 4 },
            { 13, 2 },
            { 15, 5 },
            { 9972, 23 }
        };
        for (auto& it : X) {
            auto X = it.first;
            auto gt = it.second;
            auto ans = int(IntegerSidedPythagoreanTripleFinder::findTriplesWithOneSide(X).size());
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
