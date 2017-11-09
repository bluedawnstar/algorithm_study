#include <vector>
#include <limits>
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

void testKDTree() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- KD-Tree ----------------------------------" << endl;
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

        tree.deleteNode(points[0]);
        assert(tree.root->point[0] == 35 && tree.root->point[1] == 45);
    }

    cout << "OK!" << endl;
}
