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
    //return; //TODO: if you want to test, make this line a comment.
    
    cout << "--------- testFraction() -----------------" << endl;
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
        // https://oeis.org/A001203
        // [3; 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14, 2, 1, 1, 2, 2, 2, 2, 1, 84, 2]
        vector<long long> piA = vector<long long>{ 3, 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14, 2, 1, 1, 2, 2, 2, 2, 1, 84, 2, };
        auto v = continuedFraction(piA);
        for (auto it : v) {
            cout << it.first << "/" << it.second << endl;
        }

        vector<long long> coeff = getCoefficientsOfContinuedFraction(v.back().first, v.back().second, int(piA.size()));
        if (coeff != piA) {
            cout << "Mismatched : " << coeff << endl;
        }
        assert(coeff == piA);
    }
    {
        // https://oeis.org/A001203
        // [3; 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14]
        vector<int> gt = vector<int>{ 3, 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14 };

        vector<int> coeff = getCoefficientsOfContinuedFraction<double, int>(M_PI, int(gt.size()));
        if (coeff != gt) {
            cout << "Mismatched : " << coeff << endl;
        }
        assert(coeff == gt);
    }
    {
        vector<int> gt = vector<int>{ 4, 2, 6, 7 };

        vector<int> coeff = getCoefficientsOfContinuedFraction(415, 93, int(gt.size()));
        if (coeff != gt) {
            cout << "Mismatched : " << coeff << endl;
        }
        assert(coeff == gt);
    }
    cout << "OK!" << endl;
}
