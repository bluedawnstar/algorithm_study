#include <cmath>
#include <limits>
#include <numeric>
#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "sternBrocotTree.h"

// 
// <Related Links>
// http://www.sciencedirect.com/science/article/pii/S0166218X09000894
// https://en.wikipedia.org/wiki/Continued_fraction
// https://en.wikipedia.org/wiki/Farey_sequence
// https://en.wikipedia.org/wiki/Mediant_(mathematics)
// https://en.wikipedia.org/wiki/Stern%E2%80%93Brocot_tree
// 
// <HackerRank's Question & Solutions>
// https://www.hackerrank.com/contests/w29/challenges/minimal-distance-to-pi/submissions/code/1300697342
// https://www.hackerrank.com/contests/w29/challenges/minimal-distance-to-pi/submissions/code/1300700017
// https://codepair.hackerrank.com/paper/OqDsIuXQ?b=eyJyb2xlIjoiY2FuZGlkYXRlIiwibmFtZSI6ImJsdWVkYXduc3RhciIsImVtYWlsIjoieW91bmdtYW4ucm9AZ21haWwuY29tIn0%3D
// https://codepair.hackerrank.com/paper/PLC7kLzG?b=eyJyb2xlIjoiY2FuZGlkYXRlIiwibmFtZSI6ImJsdWVkYXduc3RhciIsImVtYWlsIjoieW91bmdtYW4ucm9AZ21haWwuY29tIn0%3D
// 
// <PI>
// http://qin.laya.com/tech_projects_approxpi.html
// https://oeis.org/A001203
// 

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

static struct TestData {
    long long from, to;
    long long num, denom;
} sIn[] = {
    { 1, 10, 22, 7 },
    { 482480, 1196809, 3126535, 995207 },
    { 359407, 835582, 1146408, 364913 },
    { 622459, 1367716, 4272943, 1360120 },
    { 646699, 1108000, 3126535, 995207 },
    { 819345, 1327749, 3126535, 995207 },
    { 476934, 927416, 2292816, 729826 },
    { 724574, 1347684, 3126535, 995207 },
    { 766766, 1488720, 4272943, 1360120 },
    { 16706, 61598, 104348, 33215 }
};

void testSternBrocotTree() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Stern Brocot Tree -------------------------" << endl;
    // tree iteration
    {
        vector<pair<int, int>> F5{
            { 0, 1 }, { 1, 4 }, { 1, 3 }, { 2, 5 }, { 1, 2 }, { 3, 5 }, { 2, 3 }, { 3, 4 },
            { 1, 1 }, { 4, 3 }, { 3, 2 }, { 5, 3 }, { 2, 1 }, { 5, 2 }, { 3, 1 }, { 4, 1 },
            { 1, 0 }
        };

        int maxLevel = 4;
        vector<pair<int, int>> ans;

        ans.emplace_back(0, 1);
        SternBrocotTree::build<int>([&ans](int x, int y, int level) {
            ans.emplace_back(x, y);
        }, maxLevel);
        ans.emplace_back(1, 0);

        sort(ans.begin(), ans.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.first * b.second < b.first * a.second;
        });

        for (auto it : ans)
            cout << " " << it.first << "/" << it.second;
        cout << endl;

        assert(ans == F5);
    }
    // tree path
    {
        vector<tuple<int, int, string>> test{
            { 1, 4, "LLL" },
            { 1, 3, "LL"  },
            { 2, 5, "LLR" },
            { 1, 2, "L"   },
            { 3, 5, "LRL" },
            { 2, 3, "LR"  },
            { 3, 4, "LRR" },
            { 1, 1, ""    },
            { 4, 3, "RLL" },
            { 3, 2, "RL"  },
            { 5, 3, "RLR" },
            { 2, 1, "R"   },
            { 5, 2, "RRL" },
            { 3, 1, "RR"  },
            { 4, 1, "RRR" },
        };
        for (auto& it : test) {
            int x, y;
            string gt;
            tie(x, y, gt) = it;
            assert(SternBrocotTree::findPath(x, y) == gt);
            assert(SternBrocotTree::fractionFromPath<int>(gt) == make_pair(x, y));
        }
    }
    // nearest fractions
    {
        for (int i = 0; i < sizeof(sIn) / sizeof(sIn[0]); i++) {
            auto f = SternBrocotTree::findNearestFraction<long long>(sIn[i].from, sIn[i].to, 899125804609ll, 286200632530ll);
            assert(f.first == sIn[i].num && f.second == sIn[i].denom);
        }
        for (int i = 0; i < sizeof(sIn) / sizeof(sIn[0]); i++) {
            auto f = SternBrocotTree::findNearestFraction<long long>(sIn[i].from, sIn[i].to, 1.0 * 899125804609ll / 286200632530ll);
            assert(f.first == sIn[i].num && f.second == sIn[i].denom);
        }
    }

    cout << "OK!" << endl;
}
