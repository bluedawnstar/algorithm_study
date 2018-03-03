#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreeLineSegment2DMax.h"
#include "segmentTreeLineSegment2DMin.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSegmentTreeLineSegment2D() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Segment Tree for 2D Line Segments -----------------------------" << endl;
    {
        int N = 100;
        int MAXX = 1000;

        int L = 10, R = 200;

        vector<SegmentTreeLineSegment2DMax::Line> lines;
        vector<pair<int, int>> ranges;

        lines.push_back(SegmentTreeLineSegment2DMax::Line{ 0ll, -65535ll });
        ranges.emplace_back(L, R);
        for (int i = 1; i < N; i++) {
            int L = rand() % MAXX;
            int R = rand() % MAXX;
            lines.push_back(SegmentTreeLineSegment2DMax::Line{ (long long)rand() % 100 - 50, (long long)rand() - RAND_MAX / 2 });
            ranges.emplace_back(min(L, R), max(L, R));
        }

        SegmentTreeLineSegment2DMax tree(MAXX);
        for (int i = 0; i < N; i++)
            tree.add(ranges[i].first, ranges[i].second, lines[i].m, lines[i].b);

        vector<long long> gt(R - L + 1, -tree.INF);
        vector<long long> ans(R - L + 1);

        for (int i = 0; i < N; i++) {
            int minX = max(L, ranges[i].first);
            int maxX = min(R, ranges[i].second);
            for (int x = minX; x <= maxX; x++)
                gt[x - L] = max(gt[x - L], lines[i].get(x));
        }

        for (int i = L; i <= R; i++)
            ans[i - L] = tree.query(i);

        assert(gt == ans);
    }
    {
        int N = 100;
        int MAXX = 1000;

        int L = 10, R = 200;

        vector<SegmentTreeLineSegment2DMin::Line> lines;
        vector<pair<int, int>> ranges;

        lines.push_back(SegmentTreeLineSegment2DMin::Line{ 0ll, 65535ll });
        ranges.emplace_back(L, R);
        for (int i = 1; i < N; i++) {
            int L = rand() % MAXX;
            int R = rand() % MAXX;
            lines.push_back(SegmentTreeLineSegment2DMin::Line{ (long long)rand() % 100 - 50, (long long)rand() - RAND_MAX / 2 });
            ranges.emplace_back(min(L, R), max(L, R));
        }

        SegmentTreeLineSegment2DMin tree(MAXX);
        for (int i = 0; i < N; i++)
            tree.add(ranges[i].first, ranges[i].second, lines[i].m, lines[i].b);

        vector<long long> gt(R - L + 1, tree.INF);
        vector<long long> ans(R - L + 1);

        for (int i = 0; i < N; i++) {
            int minX = max(L, ranges[i].first);
            int maxX = min(R, ranges[i].second);
            for (int x = minX; x <= maxX; x++)
                gt[x - L] = min(gt[x - L], lines[i].get(x));
        }

        for (int i = L; i <= R; i++)
            ans[i - L] = tree.query(i);

        assert(gt == ans);
    }

    cout << "OK!" << endl;
}
