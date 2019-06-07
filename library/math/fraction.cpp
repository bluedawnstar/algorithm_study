#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

#include "fraction.h"

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
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

template <typename T>
ostream& operator <<(ostream& os, const Fraction<T>& f) {
    os << "(" << f.num << " / " << f.denom << ")";
    return os;
}

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

template <typename T>
static T gcd(T p, T q) {
    if (p < q)
        swap(p, q);

    T t;
    while (q != 0) {
        t = q;
        q = p % q;
        p = t;
    }

    return p;
}

void testFraction() {
    return; //TODO: if you want to test, make this line a comment.
    
    cout << "--------- testFraction() -----------------" << endl;
    {
        int N = 1000000;
        int T = 100;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % N + 1;
            int b = RandInt32::get() % N + 1;
            int n = RandInt32::get() % N;

            int g = gcd(a, b);
            a /= g;
            b /= g;

            long long ans = sumFloor(n, a, b);
            long long gt = 0;
            for (int j = 0; j <= n; j++)
                gt += 1ll * a * j / b;

            if (ans != gt)
                cout << "Mismatched : sumFloor(" << n << ", " << a << ", " << b << ") = " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        Fraction<int> f1(6, 21);
        assert(f1.copyReduce() == Fraction<int>(2, 7));

        Fraction<int> f2(9, 12);

        assert(f1 + f2 == Fraction<int>(87, 84));
        assert(f1 - f2 == Fraction<int>(-39, 84));
        assert(f1 * f2 == Fraction<int>(54, 252));
        assert(f1 / f2 == Fraction<int>(72, 189));
        assert(-f1 == Fraction<int>(-6, 21));
        assert(+f1 == f1);

        Fraction<int> f3(2, 7);
        assert(f1 == f3);
        Fraction<int> f4(3, 7);
        assert(f1 < f4);
        assert(f4 > f1);
    }

    //--- test continued fraction ---------------------------------------------
    cout << "*** PI's fractions" << endl;
    {
        // [3; 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14, 2, 1, 1, 2, 2, 2, 2, 1, 84, 2]
        vector<long long> piA = vector<long long>{ 3, 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14, 2, 1, 1, 2, 2, 2, 2, 1, 84, 2, };
        auto v = continuedFraction(piA);
        for (auto it : v) {
            cout << it.first << "/" << it.second << endl;
        }
    }
    cout << "OK!" << endl;
}
