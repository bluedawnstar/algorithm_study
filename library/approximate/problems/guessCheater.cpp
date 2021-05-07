#include <memory>
#include <chrono>
#include <random>
#include <numeric>
#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "guessCheater.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"
#include "../../common/rand.h"

static random_device rd;
static mt19937 gen(rd());
static uniform_real_distribution<> distrib(0.0, 1.0);
static uniform_int_distribution<> distrib2(0, 1);
static uniform_int_distribution<> distrib100(0, 99);

static double prob(double S, double Q) {
    return 1.0 / (1.0 + exp(Q - S));
}

static int generateTestCase(vector<string>& out) {
    int sel = distrib100(gen);

    vector<double> Q(10000);
    for (int i = 0; i < 10000; i++)
        Q[i] = distrib(gen) * 6.0 - 3.0;

    out.clear();
    for (int i = 0; i < 100; i++) {
        double S = distrib(gen) * 6.0 - 3.0;

        string t;
        if (i == sel) {
            // cheater
            for (int j = 0; j < 10000; j++) {
                auto bit = distrib2(gen);
                auto p = distrib(gen);
                if (bit == 1 || p < prob(S, Q[j]))
                    t.push_back('1');
                else
                    t.push_back('0');
            }
        } else {
            for (int j = 0; j < 10000; j++) {
                auto p = distrib(gen);
                if (p < prob(S, Q[j]))
                    t.push_back('1');
                else
                    t.push_back('0');
            }
        }
        out.push_back(t);
    }

    return sel;
}

void testGuessCheater() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Guess Cheater -------------" << endl;
    {
        vector<string> testCases;

        const int T = 100;

        int count = 0;
        for (int i = 0; i < T; i++) {
            if (((i + 1) & 3) == 0)
                cerr << "... " << i << '\r';
            int gt = generateTestCase(testCases);

            int ans = guessCheater(testCases);
            if (gt == ans)
                count++;
        }

        double prob = double(count) / T;
        cout << "... prob = " << prob << endl;

        assert(prob > 0.9);
    }

    cout << "OK!" << endl;
}
