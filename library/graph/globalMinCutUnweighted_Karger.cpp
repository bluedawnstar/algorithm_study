#include <chrono>
#include <random>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "globalMinCutUnweighted_Karger.h"
#include "globalMinCutUnweighted_KargerStein.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGlobalMinCutUnweightedKarger() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Global Minimum Cut for Unweighted Graph (Karger's Algorithm) -------" << endl;
    {
        const int N = 10;
        vector<pair<int, int>> edges;
        for (int i = 0; i < N / 2; i++) {
            for (int j = i + 1; j < N / 2; j++) {
                edges.emplace_back(i, j);
                edges.emplace_back(i + N / 2, j + N / 2);
            }
        }
        vector<int> gt;
        gt.push_back(int(edges.size()));
        edges.emplace_back(2, 7);
        gt.push_back(int(edges.size()));
        edges.emplace_back(1, 9);
        gt.push_back(int(edges.size()));
        edges.emplace_back(4, 6);

        {
            GlobalMinCutKarger karger(N);
            for (auto& it : edges)
                karger.addEdge(it.first, it.second);

            vector<int> ans;

            const int MAXREPEAT = N * N * 4;
            int repeat = 0;
            for (; repeat < MAXREPEAT; repeat++) {
                vector<int> cut;
                int r = karger.findMinCut(cut);
                if (r == int(gt.size())) {
                    ans = cut;
                    break;
                }
            }
            cout << "repeat = " << repeat << endl;

            sort(ans.begin(), ans.end());
            if (ans != gt) {
                cout << "Mincut not found! : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
        {
            GlobalMinCutKargerStein kargerStein(N);
            for (auto& it : edges)
                kargerStein.addEdge(it.first, it.second);

            vector<int> ans;

            const int MAXREPEAT = 4 * 4;
            int repeat = 0;
            for (; repeat < MAXREPEAT; repeat++) {
                vector<int> cut;
                int r = kargerStein.findMinCut(cut);
                if (r == int(gt.size())) {
                    ans = cut;
                    break;
                }
            }
            cout << "repeat = " << repeat << endl;

            sort(ans.begin(), ans.end());
            if (ans != gt) {
                cout << "Mincut not found! : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    {
        const int N = 20;
        vector<pair<int, int>> edges;
        for (int i = 0; i < N / 2; i++) {
            for (int j = i + 1; j < N / 2; j++) {
                edges.emplace_back(i, j);
                edges.emplace_back(i + N / 2, j + N / 2);
            }
        }
        vector<int> gt;
        gt.push_back(int(edges.size()));
        edges.emplace_back(2, 17);
        gt.push_back(int(edges.size()));
        edges.emplace_back(1, 19);
        gt.push_back(int(edges.size()));
        edges.emplace_back(4, 16);
        gt.push_back(int(edges.size()));
        edges.emplace_back(6, 17);
        gt.push_back(int(edges.size()));
        edges.emplace_back(9, 10);
        gt.push_back(int(edges.size()));
        edges.emplace_back(3, 11);
        gt.push_back(int(edges.size()));
        edges.emplace_back(1, 11);
        gt.push_back(int(edges.size()));
        edges.emplace_back(4, 15);

        {
            GlobalMinCutKarger karger(N);
            for (auto& it : edges)
                karger.addEdge(it.first, it.second);

            vector<int> ans;

            const int MAXREPEAT = N * N * 7;
            int repeat = 0;
            for (; repeat < MAXREPEAT; repeat++) {
                vector<int> cut;
                int r = karger.findMinCut(cut);
                if (r == int(gt.size())) {
                    ans = cut;
                    break;
                }
            }
            cout << "repeat = " << repeat << endl;

            sort(ans.begin(), ans.end());
            if (ans != gt) {
                cout << "Mincut not found! : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
        {
            GlobalMinCutKargerStein kargerStein(N);
            for (auto& it : edges)
                kargerStein.addEdge(it.first, it.second);

            vector<int> ans;

            const int MAXREPEAT = 7 * 7;
            int repeat = 0;
            for (; repeat < MAXREPEAT; repeat++) {
                vector<int> cut;
                int r = kargerStein.findMinCut(cut);
                if (r == int(gt.size())) {
                    ans = cut;
                    break;
                }
            }
            cout << "repeat = " << repeat << endl;

            sort(ans.begin(), ans.end());
            if (ans != gt) {
                cout << "Mincut not found! : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK" << endl;
}
