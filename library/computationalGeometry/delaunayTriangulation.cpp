#include <cmath>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#include "vec2D.h"
#include "delaunayTriangulation.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

template <typename T>
static ostream& operator <<(ostream& os, const Vec2D<T>& v) {
    os << "(" << v.first << "," << v.second << ")";
    return os;
}

static void dumpTriangles(const DelaunayTriangulation<double>& dt, const vector<DelaunayTriangulation<double>::Triangle>& tr) {
    for (auto& it : tr) {
        cout << "{" << dt.points[it.p[0]] << "," << dt.points[it.p[1]] << "," << dt.points[it.p[2]] << "},";
    }
    cout << endl;
}

void testDelaunayTriangulation() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Delaunay Triangulation ---------------------------------" << endl;

    DelaunayTriangulation<double> tr(100000000.0);
    tr.add(0, 0);
    tr.add(4, 0);
    tr.add(2, 2);

    auto ansAll = tr.getTriangles(false);
    auto ansAdded = tr.getTriangles(true);

    cout << "all: ";
    dumpTriangles(tr, ansAll);

    cout << "added only: ";
    dumpTriangles(tr, ansAdded);

    cout << "OK!" << endl;
}
