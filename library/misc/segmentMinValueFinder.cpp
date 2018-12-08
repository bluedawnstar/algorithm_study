#include <tuple>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "../rangeQuery/segmentTreeCompact.h"
#include "segmentMinValueFinder.h"

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

// https://www.hackerrank.com/contests/hourrank-31/challenges/basketball-tournament-1
static vector<int> solve(const vector<int>& v, const vector<tuple<int,int,long long>>& qry) {
    vector<int> sum(v.size() + 1);
    for (int i = 0; i < int(v.size()); i++)
        sum[i + 1] = sum[i] + v[i];
    return SegmentMinValueFinder<int,long long>::solve(v, qry, [&sum](int L, int R) { return 2ll * (R - L + 1) * (sum[R + 1] - sum[L]); });
}

void testSegmentMinValueFinder() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Segment Min Value Finder ------------------------" << endl;
    // https://www.hackerrank.com/contests/hourrank-31/challenges/basketball-tournament-1
    {
        vector<int> values{ 1, 1, 2, 3, 4 };
        vector<tuple<int, int, long long>> qry{ { 0, 4, 2 }, { 0, 4, 11 } };

        vector<int> gt{ 1, 2 };
        assert(solve(values, qry) == gt);
    }
    {
        vector<int> values{ 1, 3, 2, 4, 6 };
        vector<tuple<int, int, long long>> qry{
            { 0, 2, 3 },
            { 0, 2, 2 },
            { 1, 3, 10 },
            { 0, 0, 900 }
        };

        vector<int> gt{ 2, 1, 3, SegmentMinValueFinder<int,long long>::INF };
        assert(solve(values, qry) == gt);
    }
    cout << "OK!" << endl;
}
