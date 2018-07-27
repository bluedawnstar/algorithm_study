#include <cassert>
#include <cmath>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "lineSegmentAllIntersections.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "line.h"

static vector<tuple<int, int, Vec2D<double>>> findAllIntersectionsSlow(vector<LineSegmentAllIntersections::LineSegment>& segments) {
    vector<tuple<int, int, Vec2D<double>>> res;

    for (int i = 0; i < int(segments.size()); i++)
        segments[i].id = i;

    Vec2D<double> pt;
    for (int i = 0; i < int(segments.size()); i++) {
        for (int j = i + 1; j < int(segments.size()); j++) {
            if (segmentIntersection(segments[i].a, segments[i].b, segments[j].a, segments[j].b, pt))
                res.emplace_back(segments[i].id, segments[j].id, pt);
        }
    }

    return res;
}

static void sort(vector<tuple<int, int, Vec2D<double>>>& v) {
    for (int i = 0; i < int(v.size()); i++) {
        v[i] = make_tuple(min(get<0>(v[i]), get<1>(v[i])), max(get<0>(v[i]), get<1>(v[i])), get<2>(v[i]));
    }
    sort(v.begin(), v.end());
}

static bool compare(const vector<tuple<int, int, Vec2D<double>>>& L, const vector<tuple<int, int, Vec2D<double>>>& R) {
    if (L.size() != R.size())
        return false;

    for (int i = 0; i < int(L.size()); i++) {
        if (get<0>(L[i]) != get<0>(R[i]) || get<1>(L[i]) != get<1>(R[i]))
            return false;

        if (fabs(get<2>(R[i]).x - get<2>(L[i]).x) >= EPSILON || fabs(get<2>(R[i]).y - get<2>(L[i]).y) >= EPSILON)
            return false;
    }

    return true;
}

static ostream& operator <<(ostream& os, const LineSegmentAllIntersections::LineSegment& t) {
    os << "((" << t.a.x << ", " << t.a.y << "), (" << t.b.x << ", " << t.b.y << "), " << t.id << ")";
    return os;
}

static ostream& operator <<(ostream& os, const tuple<int, int, Vec2D<double>>& t) {
    os << "(" << get<0>(t) << ", " << get<1>(t) << ", (" << get<2>(t).x << "," << get<2>(t).y << ")" << ")";
    return os;
}

void testLineSegmentAllIntersections() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Line Segment All Intersections ------------------" << endl;

    for (int T = 0; T < 10; T++) {
        //int N = 1000;
        //int X = 1000000000;
        int N = 1000;
        int X = 100000;
        vector<LineSegmentAllIntersections::LineSegment> segments;
        unordered_set<double> S;
        for (int i = 0; i < N; i++) {
            double x1 = RandInt32::get() % X, y1 = RandInt32::get() % X;
            double x2 = RandInt32::get() % X, y2 = RandInt32::get() % X;
            if (x1 == x2 || y1 == y2)
                continue;
            if (S.find(y1 * X + x1) != S.end() || S.find(y2 * X + x2) != S.end())
                continue;
            S.insert(y1 * X + x1);
            S.insert(y2 * X + x2);
            segments.emplace_back(Vec2D<double>{ x1, y1 }, Vec2D<double>{ x2, y2 });
        }

        auto gt = findAllIntersectionsSlow(segments);
        sort(gt);

        auto ans = LineSegmentAllIntersections::findAllIntersections(segments, EPSILON);
        sort(ans);

        if (!compare(ans, gt)) {
            cout << "[" << T << "] in: " << segments << endl;
            cout << "gt = " << gt << endl;
            cout << "ans = " << ans << endl;
            cerr << "Wrong answer!" << endl;
        }
        assert(compare(ans, gt));
    }

    cout << "OK!" << endl;
}
