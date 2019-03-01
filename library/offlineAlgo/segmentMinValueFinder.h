#pragma once

// https://www.hackerrank.com/contests/hourrank-31/challenges/basketball-tournament-1
/*
                   R
    { x | x = min{max(value[i]), f(L, R) >= threshold}, [L, R] is any subarray in [left, right] }
                  i=L
    - f() is monotonically increasing 
    - qry[i] = (left, right, resultThreshold)
*/
template <typename T = int, typename ResultT = long long>
struct SegmentMinValueFinder {
    struct UnionFindSegment {
        vector<int> parent;
        vector<int> rank;

        vector<int> left;
        vector<int> right;

        UnionFindSegment() {
        }

        explicit UnionFindSegment(int N) {
            init(N);
        }

        void init(int N) {
            parent.resize(N);
            rank.assign(N, 0);
            left.resize(N);
            right.resize(N);
            for (int i = 0; i < N; i++) {
                parent[i] = -1;
                left[i] = i;
                right[i] = i;
            }
        }

        int find(int x) {
            if (parent[x] == x)
                return x;
            return parent[x] = find(parent[x]);
        }


        bool isAdded(int x) const {
            return parent[x] != -1;
        }

        void add(int x) {
            parent[x] = x;
            if (x - 1 >= 0 && parent[x - 1] != -1)
                merge(x - 1, x);
            if (x + 1 < int(parent.size()) && parent[x + 1] != -1)
                merge(x, x + 1);
        }

        pair<int, int> getSegment(int x) {
            if (!isAdded(x))
                return{ -1, -1 };

            x = find(x);
            return{ left[x], right[x] };
        }

    private:
        int merge(int x, int y) {
            int xset = find(x);
            int yset = find(y);
            if (xset == yset)
                return xset;

            if (rank[xset] < rank[yset]) {
                parent[xset] = yset;
                swap(xset, yset);
            } else {
                parent[yset] = xset;
                if (rank[xset] == rank[yset])
                    rank[xset]++;
            }
            left[xset] = min(left[xset], left[yset]);
            right[xset] = max(right[xset], right[yset]);

            return xset;
        }
    };

    static const T INF = numeric_limits<T>::max();

    // qry[i] = (left, right, resultThreshold)
    // ResultT calcOp(int L, int R)
    static vector<int> solve(const T v[], int N, const vector<tuple<int, int, ResultT>>& qry, const function<ResultT(int,int)>& calcOp) {
        int M = int(qry.size());

        vector<int> ans(M, INF);

        // Step1: preprocess queries
        CompactSegmentTree<T> treeMax(v, N, [](T a, T b) { return max(a, b); });
        vector<tuple<ResultT, int, int, int>> Q;    // (value, L, R, index)
        Q.reserve(M);
        for (int i = 0; i < M; i++) {
            int L, R;
            ResultT X;
            tie(L, R, X) = qry[i];

            // left side
            {
                int lo = L, hi = R;
                while (lo <= hi) {
                    int mid = lo + (hi - lo) / 2;
                    auto val = calcOp(L, mid);
                    if (val >= X)
                        hi = mid - 1;
                    else
                        lo = mid + 1;
                }
                if (lo <= R)
                    ans[i] = treeMax.query(L, lo);
            }
            // right side
            if (ans[i] < INF) {
                int lo = L, hi = R;
                while (lo <= hi) {
                    int mid = lo + (hi - lo) / 2;
                    auto val = calcOp(mid, R);
                    if (val < X)
                        hi = mid - 1;
                    else
                        lo = mid + 1;
                }
                if (hi >= L) {
                    ans[i] = min(ans[i], treeMax.query(hi, R));
                    R = hi - 1; // adjust the range
                }
            }
            Q.emplace_back(X, L, R, i);
        }

        // Step2: make events
        vector<int> idx(N);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&v](int a, int b) { return v[a] < v[b]; });

        vector<tuple<ResultT, int, T>> events;      // (result, L, value)
        events.reserve(N);
        UnionFindSegment dsu(N);
        for (int i = 0; i < N; i++) {
            dsu.add(idx[i]);
            auto seg = dsu.getSegment(idx[i]);

            int L = seg.first;
            int R = seg.second;
            events.emplace_back(calcOp(L, R), L, v[idx[i]]);
        }

        // Step3: process internal segments of queries
        sort(events.begin(), events.end(), [](const tuple<long long, int, T>& l, const tuple<long long, int, T>& r) {
            return get<0>(l) > get<0>(r);
        });
        sort(Q.begin(), Q.end(), [](const tuple<long long, int, int, int>& l, const tuple<long long, int, int, int>& r) {
            return get<0>(l) > get<0>(r);
        });

        CompactSegmentTree<T> treeMin(N, [](T a, T b) { return min(a, b); }, INF);
        int evt = 0;
        for (int i = 0; i < M; i++) {
            int L, R, index;
            ResultT X;
            tie(X, L, R, index) = Q[i];

            while (evt < N && get<0>(events[evt]) >= X) {
                treeMin.update(get<1>(events[evt]), get<2>(events[evt]));   // left -> h
                evt++;
            }

            if (ans[index] >= INF || L > R)
                continue;

            ans[index] = min(ans[index], treeMin.query(L, R));
        }

        return ans;
    }

    static vector<int> solve(const vector<T>& v, const vector<tuple<int, int, ResultT>>& qry, const function<ResultT(int, int)>& calcOp) {
        return solve(&v[0], int(v.size()), qry, calcOp);
    }
};
