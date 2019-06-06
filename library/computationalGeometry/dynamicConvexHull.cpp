#include <cmath>
#include <cassert>
#include <functional>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

#include "vec2D.h"
#include "convexHull.h"
#include "dynamicConvexHull.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

template <typename T>
static ostream& operator <<(ostream& os, const Vec2D<T>& x) {
    os << "(" << x.first << "," << x.second << ")";
    return os;
}

void testDynamicConvexHull() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Dynamic Convex Hull ------------------------" << endl;
    {
        vector<pair<int, int>> hull{ { 0, 0  }, { 3, -1 }, { 4, 5 }, { -1, 4 } };

        DynamicConvexHull<int> dch;
        dch.build(hull);
        dch.add(100, 100);

        auto res = dch.getAllSorted();
        vector<pair<int,int>> gt{ { -1, 4 }, { 0, 0 }, { 3, -1 }, { 100, 100 } };
        if (res != gt)
            cout << "Mismatched : " << gt << ", " << res << endl;
        assert(res == gt);
    }
    {
        vector<pair<int, int>> hull{ { 0, 0  }, { 3, -1 }, { 4, 5 }, { -1, 4 }, { 100, 100 } };

        DynamicConvexHull<int> dch;
        for (auto it : hull)
            dch.add(it.first, it.second);

        auto res = dch.getAllSorted();
        vector<pair<int,int>> gt{ { -1, 4 }, { 0, 0 }, { 3, -1 }, { 100, 100 } };
        if (res != gt)
            cout << "Mismatched : " << gt << ", " << res << endl;
        assert(res == gt);
    }
    {
        int N = 100000;
        int T = 100;
#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif
        while (T-- > 0) {
            vector<pair<int, int>> points(N);
            for (int i = 0; i < N; i++) {
                points[i].first = RandInt32::get() % 100000000;
                points[i].second = RandInt32::get() % 100000000;
            }

            vector<pair<int, int>> gtLower, gtUpper;
            {
                auto p = points;
                GrahamAndrew::convexHull(p, gtLower, gtUpper);
            }

            DynamicConvexHull<int> dch;
            for (int i = 0; i < N; i++)
                dch.add(points[i].first, points[i].second);

            auto ansLower = dch.getLower();
            auto ansUpper = dch.getUpper();

            auto gtD = GrahamAndrew::calcMaxDiameter(gtLower, gtUpper);
            auto ansD = dch.calcMaxDiameter();

            if (ansLower != gtLower)
                cout << "Mismatched at " << __LINE__ << " on " << __FILE__ << " :" << ansLower << ", " << gtLower << endl;
            if (ansUpper != gtUpper)
                cout << "Mismatched at " << __LINE__ << " on " << __FILE__ << " :" << ansUpper << ", " << gtUpper << endl;
            if (ansD != gtD)
                cout << "Mismatched at " << __LINE__ << " on " << __FILE__ << " :" << ansD << ", " << gtD << endl;

            assert(ansLower == gtLower && ansUpper == gtUpper);
            assert(ansD == gtD);
        }
    }
    cout << "*** speed test..." << endl;
    {
        int N = 1000000;
        int T = 100;
#ifdef _DEBUG
        N = 100000;
        T = 100;
#endif
        vector<pair<int, int>> points(N);
        for (int i = 0; i < N; i++) {
            points[i].first = RandInt32::get() % 100000000;
            points[i].second = RandInt32::get() % 100000000;
        }

        PROFILE_START(0);
        int sum1 = 0;
        {
            vector<pair<int,int>> p(points.begin(), points.end());
            vector<pair<int, int>> gtLower, gtUpper;
            GrahamAndrew::convexHull(p, gtLower, gtUpper);
            sum1 += int(gtLower.size()) + int(gtUpper.size());
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        int sum2 = 0;
        {
            DynamicConvexHull<int> dch;

            vector<pair<int,int>> p(points.begin(), points.end());
            for (int i = 0; i < N; i++)
                dch.add(p[i].first, p[i].second);

            auto low = dch.getLower();
            auto upp = dch.getUpper();
            sum2 += int(low.size()) + int(upp.size());
        }
        PROFILE_STOP(1);

        if (sum1 != sum2)
            cout << "Mismatched at " << __LINE__ << " on " << __FILE__ << " :" << sum1 << ", " << sum2 << endl;

        assert(sum1 == sum2);
    }
    cout << "OK!" << endl;
}
