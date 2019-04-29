#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

#include "sternBrocot.h"

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
    { 450214, 458009, 1438862, 458004 },
    { 476934, 927416, 2292816, 729826 },
    { 724574, 1347684, 3126535, 995207 },
    { 766766, 1488720, 4272943, 1360120 },
    { 16706, 61598, 104348, 33215 }
};

void testSternBrocot() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Stern Brocot Tree -------------------------" << endl;
    {
        for (int i = 0; i < sizeof(sIn) / sizeof(sIn[0]); i++) {
            long long lo = sIn[i].from, hi = sIn[i].to;

            auto cnt = sumFloorRange(lo, hi, 899125804609ll, 286200632530ll);
            auto c1 = findKthSternBrocot(lo, hi, cnt);
            auto c2 = findKthSternBrocot(lo, hi, cnt + 1);

            long long scale1 = (lo + c1.second - 1) / c1.second;
            c1.first *= scale1;
            c1.second *= scale1;

            long long scale2 = (lo + c2.second - 1) / c2.second;
            c2.first *= scale2;
            c2.second *= scale2;

            assert((c1.first == sIn[i].num || c2.first == sIn[i].num) &&
                (c1.second == sIn[i].denom || c2.second == sIn[i].denom));
        }
    }
    cout << "OK!" << endl;
}
