#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "divideConquerOptimization_OptimalSquareDistance1D.h"
#include "divideConquerOptimization_OptimalKMeans1D.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static vector<int> makeRandomData(int N, int maxValue) {
    vector<int> res(N);
    for (int i = 0; i < N; i++) {
        res[i] = RandInt32::get() % maxValue;
    }
    sort(res.begin(), res.end());
    return res;
}

static vector<double> makeRandomData(int N, double maxValue) {
    vector<double> res(N);
    for (int i = 0; i < N; i++) {
        res[i] = RandInt32::get() * maxValue / numeric_limits<int>::max();
    }
    sort(res.begin(), res.end());
    return res;
}

static double calcDistanceL2(const vector<int>& vec, const vector<pair<int, int>>& groups) {
    vector<double> centroids(groups.size());
    for (int i = 0; i < (int)groups.size(); i++) {
        for (int j = groups[i].first; j <= groups[i].second; j++)
            centroids[i] += vec[j];
        centroids[i] /= (groups[i].second - groups[i].first + 1);
    }

    double res = 0.0;
    for (int i = 0; i < (int)groups.size(); i++) {
        for (int j = groups[i].first; j <= groups[i].second; j++)
            res += (vec[j] - centroids[i]) * (vec[j] - centroids[i]);
    }

    return res;
}

static double calcDistanceL2(const vector<double>& vec, const vector<pair<int, int>>& groups) {
    vector<double> centroids(groups.size());
    for (int i = 0; i < (int)groups.size(); i++) {
        for (int j = groups[i].first; j <= groups[i].second; j++)
            centroids[i] += vec[j];
        centroids[i] /= (groups[i].second - groups[i].first + 1);
    }

    double res = 0.0;
    for (int i = 0; i < (int)groups.size(); i++) {
        for (int j = groups[i].first; j <= groups[i].second; j++)
            res += (vec[j] - centroids[i]) * (vec[j] - centroids[i]);
    }

    return res;
}

void testOptimalSquareDistance1D() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- OptimalSquareDistance1D ------------------------------" << endl;
    {
        OptimalSquareDistance1D osd(vector<int>{ 1, 2, 4 });
        assert(osd.solve(2) == 0.5);
    }
    {
        int N = 100;
        int K = 10;
        auto data = makeRandomData(N, 10000.0);

        OptimalKMeans1D okm(data);
        double dist = okm.solve(K);

        auto groups = okm.getGroup();
        double dist2 = calcDistanceL2(data, groups);

        assert(groups.size() == K);

        cout << "groups = " << groups << endl;
        cout << "L2 distance = " << dist << ", " << dist2 << endl;
        cout << "diff = " << abs(dist - dist2) << endl;

        assert(abs(dist - dist2) / dist < 1e-6);
    }
    {
        int N = 1000000;
#ifdef _DEBUG
        N = 1000;
#endif
        int K = 128;
        auto data = makeRandomData(N, 5.0);

        PROFILE_START(0);
        OptimalKMeans1D okm(data);
        double dist = okm.solve(K);
        PROFILE_STOP(0);

        PROFILE_START(1);
        auto groups = okm.getGroup();
        double dist2 = calcDistanceL2(data, groups);
        PROFILE_STOP(1);

        if (dist == 0.0 || dist2 == 0.0)
            cout << "ERROR" << endl;
    }

    cout << "OK!" << endl;
}
