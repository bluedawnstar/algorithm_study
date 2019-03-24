#include <memory>
#include <chrono>
#include <random>
#include <numeric>
#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "kmeans.h"
#include "simpleClustering.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerearth.com/practice/basic-programming/implementation/basics-of-implementation/practice-problems/approximate/connection-optimization-48323b75/

void testKMeans() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- k-means -------------" << endl;
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime = chrono::high_resolution_clock::now();

        KMeans<double> kmeans([&startTime]() -> bool {
            auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime).count() / 1000.0;
            return (elapsed >= 1.0);
        });


        vector<pair<double, double>> P{
            { 0.0 - 0.1, 0.0 - 0.1 }, { 0.0 + 0.1, 0.0 - 0.1 }, { 0.0 - 0.1, 0.0 + 0.1 }, { 0.0 + 0.1, 0.0 + 0.1 }, 
            { 1.0 - 0.1, 0.0 - 0.1 }, { 1.0 + 0.1, 0.0 - 0.1 }, { 1.0 - 0.1, 0.0 + 0.1 }, { 1.0 + 0.1, 0.0 + 0.1 }, 
            { 0.0 - 0.1, 1.0 - 0.1 }, { 0.0 + 0.1, 1.0 - 0.1 }, { 0.0 - 0.1, 1.0 + 0.1 }, { 0.0 + 0.1, 1.0 + 0.1 }, 
            { 1.0 - 0.1, 1.0 - 0.1 }, { 1.0 + 0.1, 1.0 - 0.1 }, { 1.0 - 0.1, 1.0 + 0.1 }, { 1.0 + 0.1, 1.0 + 0.1 }, 
        };

        vector<pair<double, double>> C{
            { 0.0 + 0.0, 0.0 - 0.1 }, { 1.0 + 0.0, 0.0 - 0.1 }, { 0.0 + 0.0, 1.0 - 0.1 }, { 1.0 + 0.0, 1.0 - 0.1 }, 
        };

        kmeans.doKMeans(P, C);

        sort(kmeans.centers.begin(), kmeans.centers.end());
        cout << kmeans.centers << endl;
        assert(abs(kmeans.centers[0].first - 0.0) < 1e-6 && abs(kmeans.centers[0].second - 0.0) < 1e-6);
        assert(abs(kmeans.centers[1].first - 0.0) < 1e-6 && abs(kmeans.centers[1].second - 1.0) < 1e-6);
        assert(abs(kmeans.centers[2].first - 1.0) < 1e-6 && abs(kmeans.centers[2].second - 0.0) < 1e-6);
        assert(abs(kmeans.centers[3].first - 1.0) < 1e-6 && abs(kmeans.centers[3].second - 1.0) < 1e-6);
    }
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime = chrono::high_resolution_clock::now();

        KMeans<double> kmeans([&startTime]() -> bool {
            auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime).count() / 1000.0;
            return (elapsed >= 1.0);
        });


        vector<pair<double, double>> P{
            { 0.0 - 0.1, 0.0 - 0.1 }, { 0.0 + 0.1, 0.0 - 0.1 }, { 0.0 - 0.1, 0.0 + 0.1 }, { 0.0 + 0.1, 0.0 + 0.1 }, 
            { 1.0 - 0.1, 0.0 - 0.1 }, { 1.0 + 0.1, 0.0 - 0.1 }, { 1.0 - 0.1, 0.0 + 0.1 }, { 1.0 + 0.1, 0.0 + 0.1 }, 
            { 0.0 - 0.1, 1.0 - 0.1 }, { 0.0 + 0.1, 1.0 - 0.1 }, { 0.0 - 0.1, 1.0 + 0.1 }, { 0.0 + 0.1, 1.0 + 0.1 }, 
            { 1.0 - 0.1, 1.0 - 0.1 }, { 1.0 + 0.1, 1.0 - 0.1 }, { 1.0 - 0.1, 1.0 + 0.1 }, { 1.0 + 0.1, 1.0 + 0.1 }, 
        };

        vector<pair<double, double>> C;
        vector<vector<int>> groups;
        SimpleClustering<double>::doClustering(true, P, 10, 4, C, groups, [](double distSum, int cnt) { return distSum; });

        kmeans.doKMeans(P, C);

        sort(kmeans.centers.begin(), kmeans.centers.end());
        cout << kmeans.centers << endl;
        assert(abs(kmeans.centers[0].first - 0.0) < 1e-6 && abs(kmeans.centers[0].second - 0.0) < 1e-6);
        assert(abs(kmeans.centers[1].first - 0.0) < 1e-6 && abs(kmeans.centers[1].second - 1.0) < 1e-6);
        assert(abs(kmeans.centers[2].first - 1.0) < 1e-6 && abs(kmeans.centers[2].second - 0.0) < 1e-6);
        assert(abs(kmeans.centers[3].first - 1.0) < 1e-6 && abs(kmeans.centers[3].second - 1.0) < 1e-6);
    }

    cout << "OK!" << endl;
}
