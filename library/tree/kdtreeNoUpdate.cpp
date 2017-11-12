#include <cassert>
#include <vector>
#include <limits>
#include <numeric>
#include <algorithm>

using namespace std;

#include "kdtreeNoUpdate.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static int findNearest(const vector<vector<int>>& vec3D, const vector<bool>& erased, const vector<int>& point3D) {
    int res = -1;
    double dist = numeric_limits<double>::max();

    for (int i = 0; i < (int)vec3D.size(); i++) {
        if (erased[i])
            continue;

        double d = 0.0;
        for (int j = 0; j < 3; j++) {
            d += double(vec3D[i][j] - point3D[j]) * double(vec3D[i][j] - point3D[j]);
        }
        d = sqrt(d);

        if (d < dist) {
            dist = d;
            res = i;
        }
    }

    return res;
}

static int findNearest(const vector<vector<int>>& vec3D, const vector<int>& point3D) {
    int res = -1;
    double dist = numeric_limits<double>::max();

    for (int i = 0; i < (int)vec3D.size(); i++) {
        double d = 0.0;
        for (int j = 0; j < 3; j++) {
            d += double(vec3D[i][j] - point3D[j]) * double(vec3D[i][j] - point3D[j]);
        }
        d = sqrt(d);

        if (d < dist) {
            dist = d;
            res = i;
        }
    }

    return res;
}

static vector<vector<int>> searchRegion(const vector<vector<int>>& vec3D, const vector<int>& pointMin, const vector<int>& pointMax) {
    vector<vector<int>> res;

    for (int i = 0; i < (int)vec3D.size(); i++) {
        bool isIn = true;
        for (int j = 0; j < 3; j++) {
            if (vec3D[i][j] < pointMin[j] || pointMax[j] < vec3D[i][j]) {
                isIn = false;
                break;
            }
        }
        if (isIn)
            res.push_back(vec3D[i]);
    }

    return res;
}

static int countRegion(const vector<vector<int>>& vec3D, const vector<int>& pointMin, const vector<int>& pointMax) {
    int res = 0;

    for (int i = 0; i < (int)vec3D.size(); i++) {
        bool isIn = true;
        for (int j = 0; j < 3; j++) {
            if (vec3D[i][j] < pointMin[j] || pointMax[j] < vec3D[i][j]) {
                isIn = false;
                break;
            }
        }
        if (isIn)
            res++;
    }

    return res;
}

void testKDTreeNoUpdate() {
    return; //TODO: if you want to test a split function, make this line a comment.

//#undef PROFILE_START
//#define PROFILE_START(n)
//#undef PROFILE_STOP
//#define PROFILE_STOP(n)

    cout << "--- KD-Tree - No Update ----------------------------------" << endl;
    cout << "*** search nearest neighbor" << endl;
    {
        //for (int seed = 0; seed < 100; seed++)
        auto seed = 5;
        {
            srand(seed);

            KDTreeNoUpdate<int, 3> tree;

            int N = 1000;
            vector<vector<int>> in(N, vector<int>(3));
            for (int i = 0; i < N; i++) {
                in[i][0] = rand();
                in[i][1] = rand();
                in[i][2] = rand();
            }
            tree.build(in);

            int QN = 1000;

            vector<vector<int>> qry(QN, vector<int>(3));
            for (int i = 0; i < QN; i++) {
                qry[i][0] = rand();
                qry[i][1] = rand();
                qry[i][2] = rand();
            }

            vector<vector<int>> out1(QN);
            vector<vector<int>> out2(QN);

            PROFILE_START(0);
            for (int i = 0; i < QN; i++) {
                int res = tree.searchNearest(qry[i]);
                out1[i] = tree.points[res];
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < QN; i++) {
                out2[i] = in[findNearest(in, qry[i])];
            }
            PROFILE_STOP(1);

            if (out1 != out2) {
                cout << "KDTree.searchNearest() failed" << endl;
            }
            assert(out1 == out2);
        }
    }
    //----------------------------------------------
//#define TEST_WIDE
    cout << "*** search region" << endl;
    {
        //for (int seed = 0; seed < 1000; seed++)
        auto seed = 5;
        {
            srand(seed);

            KDTreeNoUpdate<int, 3> tree;

            int N = 1000;
            vector<vector<int>> in(N, vector<int>(3));
            for (int i = 0; i < N; i++) {
                in[i][0] = rand();
                in[i][1] = rand();
                in[i][2] = rand();
            }
            tree.build(in);

            int QN = 1000;

            vector<vector<int>> qryMin(QN, vector<int>(3));
            vector<vector<int>> qryMax(QN, vector<int>(3));
            for (int i = 0; i < QN; i++) {
#ifdef TEST_WIDE
                int x = rand() - 32768 / 2, y = rand() + 32768 / 2;
#else
                int x = rand(), y = x + 100;
#endif
                qryMin[i][0] = min(x, y); qryMax[i][0] = max(x, y);

#ifdef TEST_WIDE
                x = rand() - 32768 / 2, y = rand() + 32768 / 2;
#else
                x = rand(), y = x + 100;
#endif
                qryMin[i][1] = min(x, y); qryMax[i][1] = max(x, y);

#ifdef TEST_WIDE
                x = rand() - 32768 / 2, y = rand() + 32768 / 2;
#else
                x = rand(), y = x + 100;
#endif
                qryMin[i][2] = min(x, y); qryMax[i][2] = max(x, y);
            }

            vector<vector<int>> out1(QN);
            vector<vector<int>> out2(QN);
            vector<vector<vector<int>>> out3(QN);

            PROFILE_START(0);
            for (int i = 0; i < QN; i++) {
                out1[i] = tree.searchRegion(qryMin[i], qryMax[i]);
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < QN; i++) {
                out2[i] = tree.searchRegionFast(qryMin[i], qryMax[i]);
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < QN; i++) {
                out3[i] = searchRegion(in, qryMin[i], qryMax[i]);
            }
            PROFILE_STOP(2);

            for (int i = 0; i < QN; i++) {
                sort(out1[i].begin(), out1[i].end(), [&tree](int lhs, int rhs) {
                    if (tree.points[lhs][0] != tree.points[rhs][0])
                        return tree.points[lhs][0] < tree.points[rhs][0];
                    else if (tree.points[lhs][1] != tree.points[rhs][1])
                        return tree.points[lhs][1] < tree.points[rhs][1];
                    else
                        return tree.points[lhs][2] < tree.points[rhs][2];
                });
                sort(out2[i].begin(), out2[i].end(), [&tree](int lhs, int rhs) {
                    if (tree.points[lhs][0] != tree.points[rhs][0])
                        return tree.points[lhs][0] < tree.points[rhs][0];
                    else if (tree.points[lhs][1] != tree.points[rhs][1])
                        return tree.points[lhs][1] < tree.points[rhs][1];
                    else
                        return tree.points[lhs][2] < tree.points[rhs][2];
                });
                sort(out3[i].begin(), out3[i].end(), [](const vector<int>& lhs, const vector<int>& rhs) {
                    if (lhs[0] != rhs[0])
                        return lhs[0] < rhs[0];
                    else if (lhs[1] != rhs[1])
                        return lhs[1] < rhs[1];
                    else
                        return lhs[2] < rhs[2];
                });

                bool equal = (out1[i].size() == out2[i].size() && out1[i].size() == out3[i].size());
                if (!equal) {
                    cout << "KDTree.searchRegion() failed #1 (seed=" << seed << ")" << endl;
                    assert(false);
                    break;
                }

                for (int j = 0; j < (int)out1[i].size(); j++) {
                    if (tree.points[out1[i][j]][0] != tree.points[out2[i][j]][0] ||
                        tree.points[out1[i][j]][1] != tree.points[out2[i][j]][1] ||
                        tree.points[out1[i][j]][2] != tree.points[out2[i][j]][2]) {

                        cout << "KDTree.searchRegion() failed #2 (seed=" << seed << ")" << endl;
                        assert(false);
                        break;
                    }

                    if (tree.points[out1[i][j]][0] != out3[i][j][0] ||
                        tree.points[out1[i][j]][1] != out3[i][j][1] ||
                        tree.points[out1[i][j]][2] != out3[i][j][2]) {

                        cout << "KDTree.searchRegion() failed #3 (seed=" << seed << ")" << endl;
                        assert(false);
                        break;
                    }
                }
            }
        }
    }
    //----------------------------------------------
    cout << "*** count region" << endl;
    {
        //for (int seed = 0; seed < 1000; seed++)
        auto seed = 5;
        {
            srand(seed);

            KDTreeNoUpdate<int, 3> tree;

            int N = 1000;
            vector<vector<int>> in(N, vector<int>(3));
            for (int i = 0; i < N; i++) {
                in[i][0] = rand();
                in[i][1] = rand();
                in[i][2] = rand();
            }
            tree.build(in);

            int QN = 1000;

            vector<vector<int>> qryMin(QN, vector<int>(3));
            vector<vector<int>> qryMax(QN, vector<int>(3));
            for (int i = 0; i < QN; i++) {
#ifdef TEST_WIDE
                int x = rand() - 32768 / 2, y = rand() + 32768 / 2;
#else
                int x = rand(), y = x + 100;
#endif
                qryMin[i][0] = min(x, y); qryMax[i][0] = max(x, y);

#ifdef TEST_WIDE
                x = rand() - 32768 / 2, y = rand() + 32768 / 2;
#else
                x = rand(), y = x + 100;
#endif
                qryMin[i][1] = min(x, y); qryMax[i][1] = max(x, y);

#ifdef TEST_WIDE
                x = rand() - 32768 / 2, y = rand() + 32768 / 2;
#else
                x = rand(), y = x + 100;
#endif
                qryMin[i][2] = min(x, y); qryMax[i][2] = max(x, y);
            }

            vector<int> out1(QN);
            vector<int> out2(QN);
            vector<int> out3(QN);

            PROFILE_START(0);
            for (int i = 0; i < QN; i++) {
                out1[i] = tree.countRegion(qryMin[i], qryMax[i]);
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < QN; i++) {
                out2[i] = tree.countRegionFast(qryMin[i], qryMax[i]);
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < QN; i++) {
                out3[i] = countRegion(in, qryMin[i], qryMax[i]);
            }
            PROFILE_STOP(2);

            if (out1 != out2) {
                cout << "KDTree.searchNearest() failed #1" << endl;
            }
            if (out1 != out3) {
                cout << "KDTree.searchNearest() failed #2" << endl;
            }
            assert(out1 == out2);
            assert(out1 == out3);
        }
    }

    cout << "OK!" << endl;
}
