#include <vector>
#include <algorithm>

using namespace std;

//#define MOD 1000000007

#include "segmentTreeLineSegment2DMaxSum.h"
#include "segmentTreeLineSegment2DMinSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testSegmentTreeLineSegment2DSum() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Segment Tree for Sum of 2D Line Segments -----------------------------" << endl;
    {
        int N = 100;
        int MAXX = 1000;
        int L = 0, R = 80;

        vector<SegmentTreeLineSegment2DMaxSum::Line> lines;
        lines.push_back(SegmentTreeLineSegment2DMaxSum::Line{ 0ll, -65535ll });
        for (int i = 1; i < N; i++) {
            lines.push_back(SegmentTreeLineSegment2DMaxSum::Line{ (long long)RandInt32::get() % 100 - 50,
                                                                  (long long)RandInt32::get() % 65536 - 32768 });
        }

        SegmentTreeLineSegment2DMaxSum tree(MAXX);

        // method #1 : O(N^2)
        vector<long long> gt1(R + 1);
        for (int i = 0; i <= R; i++) {
            long long maxV = -tree.INF;
            for (int j = i; j <= R; j++) {
                maxV = max(maxV, lines[j].get(i));
                gt1[j] += maxV;
            }
        }

        // method #2 : O(N^2)
        vector<long long> gt2(R + 1);
        {
            vector<long long> maxV(R + 1, -tree.INF);
            for (int j = 0; j <= R; j++) {
                for (int i = 0; i <= j; i++) {
                    maxV[i] = max(maxV[i], lines[j].get(i));
                    gt2[j] += maxV[i];
                }
            }
        }

        // method #3 (from method #2) : O(NlogN)
        vector<long long> ans(R + 1);
        for (int j = 0; j <= R; j++) {
            tree.add(0, j, lines[j].m, lines[j].b);
            ans[j] = tree.querySum(0, j).first;
        }

        assert(gt1 == ans);
        assert(gt2 == ans);
    }
    {
        int N = 100;
        int MAXX = 1000;
        int L = 0, R = 80;

        vector<SegmentTreeLineSegment2DMinSum::Line> lines;
        lines.push_back(SegmentTreeLineSegment2DMinSum::Line{ 0ll, 65535ll });
        for (int i = 1; i < N; i++) {
            lines.push_back(SegmentTreeLineSegment2DMinSum::Line{ (long long)RandInt32::get() % 100 - 50,
                                                                  (long long)RandInt32::get() % 65536 - 32768 });
        }

        SegmentTreeLineSegment2DMinSum tree(MAXX);

        // method #1 : O(N^2)
        vector<long long> gt1(R + 1);
        for (int i = 0; i <= R; i++) {
            long long minV = tree.INF;
            for (int j = i; j <= R; j++) {
                minV = min(minV, lines[j].get(i));
                gt1[j] += minV;
            }
        }

        // method #2 : O(N^2)
        vector<long long> gt2(R + 1);
        {
            vector<long long> minV(R + 1, tree.INF);
            for (int j = 0; j <= R; j++) {
                for (int i = 0; i <= j; i++) {
                    minV[i] = min(minV[i], lines[j].get(i));
                    gt2[j] += minV[i];
                }
            }
        }

        // method #3 (from method #2) : O(NlogN)
        vector<long long> ans(R + 1);
        for (int j = 0; j <= R; j++) {
            tree.add(0, j, lines[j].m, lines[j].b);
            ans[j] = tree.querySum(0, j).first;
        }

        cout << gt1 << endl;
        cout << gt2 << endl;
        cout << ans << endl;
        assert(gt1 == ans);
        assert(gt2 == ans);
    }

    cout << "OK!" << endl;
}
