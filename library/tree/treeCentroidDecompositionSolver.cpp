#include <memory.h>
#include <functional>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeCentroidDecompositionSolver.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

template <typename T>
struct KthValueInDistanceSolver : public TreeCentroidDecompositionSolver<T> {
    struct SimplePersistentSegmentTree {
        int N;
        vector<T> value;
        vector<int> L;
        vector<int> R;
        int root;       // first root;

        SimplePersistentSegmentTree() {
        }

        explicit SimplePersistentSegmentTree(int n) {
            build(n);
        }

        // return root node index
        int build(int n) {
            N = n;
            value.reserve(N * 4);
            L.reserve(N * 4);
            R.reserve(N * 4);
            return root = dfsBuild(0, N - 1);
        }

        // return root node index
        int add(int root, int index, T val) {
            return dfsAdd(root, 0, N - 1, index, val);
        }

    private:
        int dfsBuild(int left, int right) {
            int l = -1, r = -1;
            if (left < right) {
                int mid = (left + right) >> 1;
                l = dfsBuild(left, mid);
                r = dfsBuild(mid + 1, right);
            }
            value.push_back(0);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsAdd(int node, int left, int right, int index, T val) {
            T newValue;
            int l = -1, r = -1;
            if (left == right) {
                newValue = value[node] + val;           //TODO: merge operation
            } else {
                int mid = (left + right) >> 1;
                if (index <= mid) {
                    l = dfsAdd(L[node], left, mid, index, val);
                    r = R[node];
                } else {
                    l = L[node];
                    r = dfsAdd(R[node], mid + 1, right, index, val);
                }
                newValue = value[l] + value[r];         //TODO: merge operation
            }
            value.push_back(newValue);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }
    };

    SimplePersistentSegmentTree distSegTree;
    vector<vector<pair<int, int>>> addRootByDist; // roots of persistent segment tree, addRootByDist[i][j] = { distance, root }
    vector<vector<pair<int, int>>> subRootByDist; // roots of persistent segment tree, subRootByDist[i][j] = { distance, root }

    // O(N*(logN)^2)
    void build() {
        TreeCentroidDecompositionSolver<T>::build(
            [this]() {
                addRootByDist.resize(N);
                subRootByDist.resize(N);
                distSegTree.build(M);
            },
            [this](int v, const vector<pair<int, T>>& nodes) {
                int root = distSegTree.add(distSegTree.root, nodes[0].second, 1);
                for (int i = 1; i < int(nodes.size()); i++) {
                    if (nodes[i - 1].first != nodes[i].first)
                        addRootByDist[v].emplace_back(nodes[i - 1].first, root);
                    root = distSegTree.add(root, nodes[i].second, 1);
                }
                addRootByDist[v].emplace_back(nodes.back().first, root);
            },
            [this](int v, const vector<pair<int, T>>& nodes) {
                int root = distSegTree.add(distSegTree.root, nodes[0].second, 1);
                for (int i = 1; i < int(nodes.size()); i++) {
                    if (nodes[i - 1].first != nodes[i].first)
                        subRootByDist[v].emplace_back(nodes[i - 1].first, root);
                    root = distSegTree.add(root, nodes[i].second, 1);
                }
                subRootByDist[v].emplace_back(nodes.back().first, root);
            }
        );
    }

    //--- query

    // 0 <= node < N, 0 <= d < N, 1 <= kth < N
    // O((logN)^2)
    T query(int u, int maxDist, int kth) {
        vector<pair<int, int>> roots = findSegmentTreeRoots(u, maxDist);
        auto ans = findKthValue(roots, kth);
        return (ans >= 0) ? uniqueValues[ans] : -1;  // uncompress
    }

private:
    // d = max distance, 0 <= u < N, 0 <= d < N
    // return the roots in the segment tree, for all ancestors in the centroid tree
    vector<pair<int, int>> findSegmentTreeRoots(int u, int maxDist) {
        vector<pair<int, int>> segTrees;

        int v = u, last = -1, at = 0;
        while (v >= 0) {
            int h = maxDist - ctDist[u][at];
            auto it = lower_bound(begin(addRootByDist[v]), end(addRootByDist[v]), make_pair(h + 1, 0));
            if (it != begin(addRootByDist[v])) {
                it--;
                segTrees.emplace_back(it->second, 1);
            }

            if (last >= 0) {
                it = lower_bound(begin(subRootByDist[last]), end(subRootByDist[last]), make_pair(h + 1, 0));
                if (it != begin(subRootByDist[last])) {
                    it--;
                    segTrees.emplace_back(it->second, -1);
                }
            }

            last = v;
            v = ctParent[v];
            at++;
        }

        return segTrees;
    }

    // 1 <= kth
    int findKthValue(vector<pair<int, int>>& segTreeRoots, int kth) {
        int lo = 0, hi = M - 1;
        while (lo < hi) {
            int qt = 0;
            for (auto& r : segTreeRoots)
                qt += r.second * distSegTree.value[distSegTree.L[r.first]];

            int mid = (lo + hi) >> 1;
            if (kth <= qt) {
                for (auto& r : segTreeRoots)
                    r.first = distSegTree.L[r.first];
                hi = mid;
            } else {
                kth -= qt;
                for (auto& r : segTreeRoots)
                    r.first = distSegTree.R[r.first];
                lo = mid + 1;
            }
        }

        int qt = 0;
        for (auto& r : segTreeRoots)
            qt += r.second * distSegTree.value[r.first];

        return kth <= qt ? lo : -1;
    }
};


// https://www.hackerrank.com/contests/w38/challenges/neighborhood-queries/problem
static vector<int> solve(const vector<int>& A, const vector<pair<int, int>>& edges, const vector<tuple<int, int, int>>& query) {
    //step1: initialize
    KthValueInDistanceSolver<int> solver;
    solver.init(A);

    //step2: add edges
    for (auto it : edges)
        solver.addEdge(it.first, it.second);

    //step3: prepare searching
    solver.build();

    vector<int> res;
    for (auto& it : query) {
        int u, d, k;
        tie(u, d, k) = it;
        res.push_back(solver.query(u, d, k));
    }

    return res;
}


void testCentroidDecompositionSolver() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Centroid Decomposition Solver -----------------------------" << endl;
    {
        vector<int> A = { 3, 10, 11, 1, 1 };
        vector<pair<int, int>> edges = { { 0, 1 },{ 0, 2 },{ 2, 3 },{ 2, 4 } };
        vector<tuple<int, int, int>> query = { { 3, 2, 3 },{ 4, 0, 1 },{ 4, 0, 2 },{ 2, 1, 4 },{ 2, 2, 4 } };
        auto ans = solve(A, edges, query);
        vector<int> gt = { 3, 1, -1, 11, 10 };
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
