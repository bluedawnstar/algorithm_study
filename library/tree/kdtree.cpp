#include <cassert>
#include <limits>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "kdtree.h"

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

static vector<int> findKNN(const vector<vector<int>>& vec3D, const vector<int>& point3D, int K) {
    double dist = numeric_limits<double>::max();

    priority_queue<pair<double, int>> pq;

    for (int i = 0; i < (int)vec3D.size(); i++) {
        double d = 0.0;
        for (int j = 0; j < 3; j++) {
            d += double(vec3D[i][j] - point3D[j]) * double(vec3D[i][j] - point3D[j]);
        }
        d = sqrt(d);

        pq.emplace(d, i);
        if ((int)pq.size() > K)
            pq.pop();
    }

    vector<int> res(pq.size());
    for (int i = (int)pq.size() - 1; i >= 0; i--) {
        res[i] = pq.top().second;
        pq.pop();
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

void testKDTree() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- KD-Tree ----------------------------------" << endl;
    cout << "*** insert" << endl;
    {
        int points[][2] = {
            { 3, 6 }, { 17, 15 }, { 13, 15 }, { 6, 12 },
            { 9, 1 }, { 2, 7 }, { 10, 19 }
        };
        int N = sizeof(points) / sizeof(points[0]);

        KDTree<int, 2> tree;
        for (int i = 0; i < N; i++)
            tree.insert(points[i]);

        assert(tree.find(vector<int>{ 10, 19 }) != nullptr);
        assert(tree.find(vector<int>{ 12, 19 }) == nullptr);
    }
    //----------------------------------------------
    cout << "*** delete" << endl;
    {
        int points[][2] = {
            { 30, 40 }, { 5, 25 }, { 70, 70 },
            { 10, 12 }, { 50, 30 }, { 35, 45 }
        };
        int N = sizeof(points) / sizeof(points[0]);

        KDTree<int, 2> tree;
        for (int i = 0; i < N; i++)
            tree.insert(points[i]);

        assert(tree.findMin(0)->point[0] == 5);
        assert(tree.findMin(1)->point[1] == 12);

        //tree.deleteNode(points[0]);
        //assert(tree.root->point[0] == 35 && tree.root->point[1] == 45);
        tree.deleteNode(points[2]);
        assert(tree.root->point[0] == 30 && tree.root->point[1] == 40);
        assert(tree.root->right->point[0] == 50 && tree.root->right->point[1] == 30);
        assert(tree.root->right->right->point[0] == 35 && tree.root->right->right->point[1] == 45);
    }
    //----------------------------------------------
    cout << "*** search nearest neighbor" << endl;
    {
        //srand(time(nullptr));

        KDTree<int, 3> tree;

        int N = 1000;
        vector<vector<int>> in(N, vector<int>(3));
        for (int i = 0; i < N; i++) {
            in[i][0] = rand();
            in[i][1] = rand();
            in[i][2] = rand();
            tree.insert(in[i]);
        }

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
            auto* p = tree.searchNearest(qry[i])->point;
            out1[i] = vector<int>(p, p + 3);
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
    //----------------------------------------------
//#undef PROFILE_START
//#define PROFILE_START(n)
//#undef PROFILE_STOP
//#define PROFILE_STOP(n)
    cout << "*** delete & search nearest neighbor" << endl;
    {
        //for (int seed = 0; seed < 100; seed++)
        {
            //srand(seed);

            KDTree<int, 3> tree;

            int N = 1000;
            vector<vector<int>> in(N, vector<int>(3));
            for (int i = 0; i < N; i++) {
                in[i][0] = rand();
                in[i][1] = rand();
                in[i][2] = rand();
                tree.insert(in[i]);
            }

            int QN = 1000;

            vector<bool> erased(N);
            for (int i = 0; i < N / 2; i++) {
                erased[(rand() * rand()) % N] = true;
            }
            for (int i = 0; i < N; i++) {
                if (erased[i])
                    tree.deleteNode(in[i]);
            }

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
                auto* p = tree.searchNearest(qry[i])->point;
                out1[i] = vector<int>(p, p + 3);
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            for (int i = 0; i < QN; i++) {
                out2[i] = in[findNearest(in, erased, qry[i])];
            }
            PROFILE_STOP(1);

            if (out1 != out2) {
                cout << "KDTree.deleteNode() failed" << endl;
            }
            assert(out1 == out2);
        }
    }
    //----------------------------------------------
    cout << "*** search region" << endl;
    {
        //for (int seed = 0; seed < 1000; seed++)
        {
            //srand(seed);

            KDTree<int, 3> tree;

            int N = 1000;
            vector<vector<int>> in(N, vector<int>(3));
            for (int i = 0; i < N; i++) {
                in[i][0] = rand();
                in[i][1] = rand();
                in[i][2] = rand();
                tree.insert(in[i]);
            }

            int QN = 1000;

//#define TEST_WIDE

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

            vector<vector<KDTree<int, 3>::Node*>> out1(QN);
            vector<vector<KDTree<int, 3>::Node*>> out2(QN);
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
                sort(out1[i].begin(), out1[i].end(), [](const KDTree<int, 3>::Node* lhs, const KDTree<int, 3>::Node* rhs) {
                    if (lhs->point[0] != rhs->point[0])
                        return lhs->point[0] < rhs->point[0];
                    else if (lhs->point[1] != rhs->point[1])
                        return lhs->point[1] < rhs->point[1];
                    else
                        return lhs->point[2] < rhs->point[2];
                });
                sort(out2[i].begin(), out2[i].end(), [](const KDTree<int, 3>::Node* lhs, const KDTree<int, 3>::Node* rhs) {
                    if (lhs->point[0] != rhs->point[0])
                        return lhs->point[0] < rhs->point[0];
                    else if (lhs->point[1] != rhs->point[1])
                        return lhs->point[1] < rhs->point[1];
                    else
                        return lhs->point[2] < rhs->point[2];
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
                    cout << "KDTree.searchRegion() failed" << endl;
                    assert(false);
                    break;
                }

                for (int j = 0; j < (int)out1[i].size(); j++) {
                    if (out1[i][j]->point[0] != out2[i][j]->point[0] ||
                        out1[i][j]->point[1] != out2[i][j]->point[1] ||
                        out1[i][j]->point[2] != out2[i][j]->point[2]) {
                        equal = false;
                        break;
                    }
                    if (out1[i][j]->point[0] != out3[i][j][0] ||
                        out1[i][j]->point[1] != out3[i][j][1] ||
                        out1[i][j]->point[2] != out3[i][j][2]) {
                        equal = false;
                        break;
                    }
                }

                if (!equal) {
                    cout << "KDTree.searchRegion() failed" << endl;
                    assert(false);
                    break;
                }
            }
        }
    }
    //----------------------------------------------
    cout << "*** search KNN" << endl;
    {
        //srand(time(nullptr));

        KDTree<int, 3> tree;

        int N = 10000000;
        vector<vector<int>> in(N, vector<int>(3));
        for (int i = 0; i < N; i++) {
            in[i][0] = rand();
            in[i][1] = rand();
            in[i][2] = rand();
            tree.insert(in[i]);
        }

        int QN = 1000;

        vector<vector<int>> qry(QN, vector<int>(3));
        for (int i = 0; i < QN; i++) {
            qry[i][0] = rand();
            qry[i][1] = rand();
            qry[i][2] = rand();
        }

        vector<vector<KDTree<int,3>::Node*>> out1(QN);
        vector<vector<int>> out2(QN);

        const int K = 100;

        PROFILE_START(0);
        for (int i = 0; i < QN; i++) {
            out1[i] = tree.searchKNN(qry[i], K);
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < QN; i++) {
            out2[i] = findKNN(in, qry[i], K);
        }
        PROFILE_STOP(1);

        vector<vector<vector<int>>> out11(QN);
        vector<vector<vector<int>>> out22(QN);
        for (int i = 0; i < QN; i++) {
            out11[i].resize(out1[i].size());
            for (int j = 0; j < (int)out1[i].size(); j++) {
                out11[i][j].assign(out1[i][j]->point, out1[i][j]->point + 3);
            }
            out22[i].resize(out2[i].size());
            for (int j = 0; j < (int)out2[i].size(); j++) {
                out22[i][j] = in[out2[i][j]];
            }
            sort(out11[i].begin(), out11[i].end());
            sort(out22[i].begin(), out22[i].end());
        }
        if (out11 != out22) {
            cout << "KDTree.searchKNN() failed" << endl;
        }
        assert(out11 == out22);
    }
#if 0
    //----------------------------------------------
    cout << "*** speed test of nearest neighbor" << endl;
    {
        //srand(time(nullptr));

        KDTree<int, 3> tree;

        int N = 10000000;
        vector<vector<int>> in(N, vector<int>(3));

        PROFILE_START(0);
        for (int i = 0; i < N; i++) {
            in[i][0] = rand();
            in[i][1] = rand();
            in[i][2] = rand();
            tree.insert(in[i]);
        }
        PROFILE_STOP(0);

        int QN = 1000;

        vector<vector<int>> qry(QN, vector<int>(3));
        for (int i = 0; i < QN; i++) {
            qry[i][0] = rand();
            qry[i][1] = rand();
            qry[i][2] = rand();
        }

        vector<vector<int>> out1(QN);
        vector<vector<int>> out2(QN);

        PROFILE_START(1);
        for (int i = 0; i < QN; i++) {
            auto* p = tree.searchNearest(qry[i])->point;
            out1[i] = vector<int>(p, p + 3);
        }
        PROFILE_STOP(1);

        if (out1 == out2)
            cerr << "Dummy message to prevent optimization" << endl;
    }
#endif

    cout << "OK!" << endl;
}
