#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "mergeSortTree2DPoint_Dynamic.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int countSlow(const vector<pair<int, int>>& v, int N, int x1, int y1, int x2, int y2) {
    int res = 0;
    for (int i = 0; i < N; i++) {
        if (x1 <= v[i].first && v[i].first <= x2 && y1 <= v[i].second && v[i].second <= y2)
            res++;
    }
    return res;
}

void testMergeSortTree2DPoint_Dynamic() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Dynamic Merge Sort Tree of 2D Point ---------------------------------------" << endl;
    {
        int H = 1000000;
        int N = 10000;
        int T = 10000;

#ifdef _DEBUG
        N = 1000;
        T = 1000;
#endif

        vector<pair<int, int>> points;
        points.reserve(N);
        for (int i = 0; i < N; i++)
            points.emplace_back(RandInt32::get() % N, RandInt32::get() % H);

        DynamicMergeSortTree2DPoint tree(N);

        for (int i = 0, j = 0; i < T; i++) {
            int n = RandInt32::get() % 100;
            int n2 = min(j + n, N);
            while (j < n2) {
                tree.add(points[j].first, points[j].second);
                j++;
            }

            int x1 = RandInt32::get() % N;
            int x2 = RandInt32::get() % N;
            int y1 = RandInt32::get() % H;
            int y2 = RandInt32::get() % H;
            if (x1 > x2)
                swap(x1, x2);
            if (y1 > y2)
                swap(y1, y2);

            int gt = countSlow(points, n2, x1, y1, x2, y2);
            int ans = tree.count(x1, y1, x2, y2);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        for (int i = 0; i < T; i++) {
            if (points.empty())
                break;

            auto it = points.begin() + RandInt32::get() % points.size();
            tree.remove(it->first, it->second);
            points.erase(it);

            int x1 = RandInt32::get() % N;
            int x2 = RandInt32::get() % N;
            int y1 = RandInt32::get() % H;
            int y2 = RandInt32::get() % H;
            if (x1 > x2)
                swap(x1, x2);
            if (y1 > y2)
                swap(y1, y2);

            int gt = countSlow(points, int(points.size()), x1, y1, x2, y2);
            int ans = tree.count(x1, y1, x2, y2);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
