#include <vector>
#include <functional>

using namespace std;

#include "gcdOnSubsegments.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static vector<vector<pair<int, int>>> buildL2R(const vector<int>& v) {
    int N = int(v.size());

    vector<vector<pair<int, int>>> res(N);
    for (int i = 0; i < N; i++) {
        int g = v[i];
        for (int j = i + 1; j < N; j++) {
            int g2 = GcdOnSubsegments::gcd(g, v[j]);
            if (g != g2) {
                res[i].emplace_back(j, g);
                g = g2;
            }
        }
        res[i].emplace_back(N, g);
    }

    return res;
}

static vector<vector<pair<int, int>>> buildR2L(const vector<int>& v) {
    int N = int(v.size());

    vector<vector<pair<int, int>>> res(N);
    for (int i = 0; i < N; i++) {
        int g = v[i];
        for (int j = i - 1; j >= 0; j--) {
            int g2 = GcdOnSubsegments::gcd(g, v[j]);
            if (g != g2) {
                res[i].emplace_back(j, g);
                g = g2;
            }
        }
        res[i].emplace_back(-1, g);
    }

    return res;
}

void testGcdOnSubsegments() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- GCD on Subsegments -------------------" << endl;
    {
        int N = 10000;
        vector<int> A(N);
        for (int i = 0; i < N; i++)
            A[i] = RandInt32::get() % N;

        auto ansl2r = GcdOnSubsegments::buildLeftAligned(A);
        auto ansr2l = GcdOnSubsegments::buildRightAligned(A);
        auto gtl2r = buildL2R(A);
        auto gtr2l = buildR2L(A);

        if (ansl2r != gtl2r || ansr2l != gtr2l)
            cout << "Mismatched!" << endl;

        assert(ansl2r == gtl2r);
        assert(ansr2l == gtr2l);
    }

    cout << "OK!" << endl;
}
