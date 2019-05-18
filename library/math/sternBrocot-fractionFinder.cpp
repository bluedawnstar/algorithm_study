#include <cmath>
#include <vector>
#include <limits>
#include <numeric>
#include <queue>
#include <algorithm>

using namespace std;

#include "sternBrocot-fractionFinder.h"
#include "sternBrocot-fractionFinder2.h"

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

void testSternBrocotFractionFinder() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fraction Finder with Stern Brocot Tree -------------------------" << endl;
    // fraction finder
    {
        FractionFinder<long long, double> ff;

        for (int i = 0; i < sizeof(sIn) / sizeof(sIn[0]); i++) {
            ff.setDenomRange(sIn[i].from, sIn[i].to);
            auto f = ff.solve(M_PI);
            assert(f.num == sIn[i].num && f.denom == sIn[i].denom);
        }
    }
    // fraction finder2
    {
        for (int i = 0; i < sizeof(sIn) / sizeof(sIn[0]); i++) {
            auto f = FractionFinder2<long long>::solve(sIn[i].from, sIn[i].to, 899125804609ll, 286200632530ll);
            assert((f.first.first == sIn[i].num && f.first.second == sIn[i].denom) ||
                   (f.second.first == sIn[i].num && f.second.second == sIn[i].denom));
        }
    }
    cout << "OK!" << endl;
}
