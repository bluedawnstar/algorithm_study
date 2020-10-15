#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePrimeFactorXorRollingHash.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerearth.com/problem/algorithm/a-kth-root-subarray-09f931a5/description/
struct KthRootSubarraySolver {
    SegmentTreePrimeFactorXorRollingHash segTree;

    explicit KthRootSubarraySolver(int maxX) : segTree(maxX) {
    }

    void prepare(const vector<int>& A, int K) {
        segTree.build(A, K);
    }

    void update(int L, int R, int X) {
        segTree.update(L, R, X);
    }

    void update(int L, int R, int X, int Y) {
        segTree.add(L, R, X, Y);
    }

    bool query(int L, int R) {
        auto res = segTree.query(L, R);
        return res.first == 0 && res.second == 0;
    }
};

void testSegmentTreePrimeFactorXorRollingHash() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Segment Tree with Xor Rolling Hash for Prime Factors ---------------------------" << endl;
    {
        vector<int> A{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 };
        KthRootSubarraySolver solver(100);
        solver.prepare(A, 2);

        assert(solver.query(9, 10) == false);
        assert(solver.query(3, 6) == true);
        solver.update(4, 5, 3);
        assert(solver.query(3, 6) == true);
        solver.update(2, 4, 3, 1);
        assert(solver.query(2, 5) == false);
        solver.update(4, 4, 3);
        assert(solver.query(2, 5) == true);
        assert(solver.query(1, 5) == false);
    }

    cout << "OK!" << endl;
}
