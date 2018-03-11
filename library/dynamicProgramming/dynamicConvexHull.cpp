#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "dynamicConvexHullUpper.h"
#include "dynamicConvexHullLower.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include "../misc/randomAccessSparseTable.h"
#include "persistentLowerConvexHull.h"
#include "persistentUpperConvexHull.h"

static void testLower(const vector<int>& X, const vector<int>& Y) {
    int N = (int)X.size();

    DynamicLowerConvexHull<int> ch(N + 1);
    PersistentLowerConvexHull<int> gt(N + 1);

    //ch.add(X[0], Y[0]);
    ch.addLinear(X[0], Y[0]);
    gt.addLinear(X[0], Y[0]);

    for (int i = 1; i < N; i++) {
        //ch.add(X[i], Y[i]);
        ch.addLinear(X[i], Y[i]);
        auto last = gt.addLinear(X[i], Y[i]);

        auto t1 = ch.queryWithSlope(X[i], Y[i]);
        vector<pair<int, int>> p1;
        p1.push_back(ch.points[t1[0]]);
        if (t1.size() > 1)
            p1.push_back(ch.points[t1[1]]);

        auto t2 = gt.queryWithSlope(last, X[i], Y[i]);
        if (t2.size() >= 2)
            swap(t2[0], t2[1]);
        vector<pair<int, int>> p2;
        p2.push_back(gt.points[t2[0]]);
        if (t2.size() > 1)
            p2.push_back(gt.points[t2[1]]);

        assert(p1 == p2);
    }
}

static void testUpper(const vector<int>& X, const vector<int>& Y) {
    int N = (int)X.size();

    DynamicUpperConvexHull<int> ch(N + 1);
    PersistentUpperConvexHull<int> gt(N + 1);

    //ch.add(X[0], Y[0]);
    ch.addLinear(X[0], Y[0]);
    gt.addLinear(X[0], Y[0]);

    for (int i = 1; i < N; i++) {
        //ch.add(X[i], Y[i]);
        ch.addLinear(X[i], Y[i]);
        auto last = gt.addLinear(X[i], Y[i]);

        auto t1 = ch.queryWithSlope(X[i], Y[i]);
        vector<pair<int, int>> p1;
        p1.push_back(ch.points[t1[0]]);
        if (t1.size() > 1)
            p1.push_back(ch.points[t1[1]]);

        auto t2 = gt.queryWithSlope(last, X[i], Y[i]);
        if (t2.size() >= 2)
            swap(t2[0], t2[1]);
        vector<pair<int, int>> p2;
        p2.push_back(gt.points[t2[0]]);
        if (t2.size() > 1)
            p2.push_back(gt.points[t2[1]]);

        assert(p1 == p2);
    }
}

void testDynamicConvexHull() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Dynamic Convex Hull ------------------------" << endl;

    auto cmp = [](const pair<int, int>& l, const pair<int, int>& r) {
        return l.first < r.first;
    };
    auto cmpEQ = [](const pair<int, int>& l, const pair<int, int>& r) {
        return l.first == r.first;
    };

    int N = 100;
    int MAXX = 1000;
    vector<pair<int, int>> in;
    for (int i = 0; i < N; i++) {
        in.emplace_back(rand() % MAXX, rand() % MAXX);
    }
    sort(in.begin(), in.end(), cmp);
    in.erase(unique(in.begin(), in.end(), cmpEQ), in.end());

    vector<int> X, Y;
    for (auto it : in) {
        X.push_back(it.first);
        Y.push_back(it.second);
    }

    testLower(X, Y);
    testUpper(X, Y);

    cout << "OK!" << endl;
}
