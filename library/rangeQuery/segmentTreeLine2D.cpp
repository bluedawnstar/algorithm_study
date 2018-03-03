#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreeLine2DMax.h"
#include "segmentTreeLine2DMin.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSegmentTreeLine2D() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Segment Tree for 2D Lines -----------------------------" << endl;
    {
        int N = 100;
        int T = 200;
        int L = -1000, R = 1000;

        vector<SegmentTreeLine2DMax::Line> lines;
        for (int i = 0; i < N; i++)
            lines.push_back(SegmentTreeLine2DMax::Line{ (long long)rand() - RAND_MAX / 2, (long long)rand() - RAND_MAX / 2 });
        
        SegmentTreeLine2DMax tree(L, R);
        for (int i = 0; i < N; i++)
            tree.add(lines[i].m, lines[i].b);

        vector<long long> gt(T, -tree.INF);
        vector<long long> ans(T);
        for (int i = 0; i < T; i++) {
            for (int j = 0; j < N; j++)
                gt[i] = max(gt[i], lines[j].get(i));
            ans[i] = tree.query(i);
        }

        assert(gt == ans);
    }
    {
        int N = 100;
        int T = 200;
        int L = -1000, R = 1000;

        vector<SegmentTreeLine2DMin::Line> lines;
        for (int i = 0; i < N; i++)
            lines.push_back(SegmentTreeLine2DMin::Line{ (long long)rand() - RAND_MAX / 2, (long long)rand() - RAND_MAX / 2 });

        SegmentTreeLine2DMin tree(L, R);
        for (int i = 0; i < N; i++)
            tree.add(lines[i].m, lines[i].b);

        vector<long long> gt(T, tree.INF);
        vector<long long> ans(T);
        for (int i = 0; i < T; i++) {
            for (int j = 0; j < N; j++)
                gt[i] = min(gt[i], lines[j].get(i));
            ans[i] = tree.query(i);
        }

        assert(gt == ans);
    }

    cout << "OK!" << endl;
}
