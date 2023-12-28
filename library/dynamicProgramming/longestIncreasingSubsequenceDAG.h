#pragma once

// 
// PRECONDITION: N * K < 2^63.
// 
template <typename T>
struct LisDAG {
    struct NodeT {
        T value;
        int index;              // index in original data
        long long count;        // the number of LIS in sub dag
        pair<int, int> next;    // [first, last)

        NodeT()
            : value(0), index(0), count(0), next(0x3f3f3f3f, 0) {
        }

        NodeT(T value, int index)
            : value(value), index(index), count(0), next(0x3f3f3f3f, 0) {
        }
    };

    long long countLimit;

    int N;
    int lisLength;
    long long totalCount;
    vector<vector<NodeT>> dag;
    vector<vector<pair<int, int>>> prev;    // prev[i] = { (first, last), ... }

    explicit LisDAG(long long countLimit)
        : countLimit(countLimit), N(0), lisLength(0), totalCount(0) {
    }

    LisDAG(long long countLimit, const T A[], int n)
        : countLimit(countLimit) {
        build(A, n);
    }

    explicit LisDAG(long long countLimit, const vector<T>& A)
        : countLimit(countLimit) {
        build(A);
    }

    // O(N*logN)
    void build(const T A[], int n) {
        N = n;
        dag.clear();
        prev.clear();

        vector<T> dp;
        auto compValue = [](const T& a, const NodeT& b) {
            return a > b.value;
        };

        // step #1 : make reverse DAG (forward)
        for (int i = 0, j = 0; i < N; i++) {
            if (dp.empty() || dp.back() < A[i]) {
                j = int(dp.size());
                dp.push_back(A[i]);
                dag.push_back({});
                prev.push_back({});
            } else {
                j = int(lower_bound(dp.begin(), dp.end(), A[i]) - dp.begin());
                dp[j] = A[i];
            }

            if (j > 0) {
                int first = int(upper_bound(dag[j - 1].begin(), dag[j - 1].end(), A[i], compValue) - dag[j - 1].begin());
                int last = int(dag[j - 1].size());
                prev[j].emplace_back(first, last);
            }
            dag[j].emplace_back(A[i], i);
        }

        // step #2 : update the number of path and DAG (backward)
        lisLength = int(dp.size());
        for (auto& it : dag.back())
            it.count = 1ll;

        for (int i = lisLength - 2; i >= 0; i--) {
            vector<long long> delta(dag[i].size() + 1);
            for (int j = 0; j < int(dag[i + 1].size()); j++) {
                delta[prev[i + 1][j].first] += dag[i + 1][j].count;
                delta[prev[i + 1][j].second] -= dag[i + 1][j].count;
            }

            long long cnt = 0;
            int lo = 0, hi = 0;
            for (int j = 0; j < int(dag[i].size()); j++) {
                while ((lo < int(dag[i + 1].size())) && (dag[i + 1][lo].index < dag[i][j].index))
                    lo++;
                while ((hi < int(dag[i + 1].size())) && (dag[i + 1][hi].value > dag[i][j].value))
                    hi++;

                cnt += delta[j];
                dag[i][j].count = min(countLimit, cnt);
                dag[i][j].next = make_pair(lo, hi);
            }
        }

        totalCount = 0;
        for (auto& it : dag[0]) {
            totalCount = add(totalCount, it.count, countLimit);
        }
    }

    void build(const vector<T>& A) {
        build(A.data(), int(A.size()));
    }

    //---

    int getLength() const {
        return lisLength;
    }

    long long getTotalCount() const {
        return totalCount;
    }

    // kth >= 1, O(N)
    vector<T> findKthOfDistinctValues(long long K) const {
        vector<T> res;

        int rangeLo = 0, rangeHi = int(dag[0].size());
        for (int i = 0; i < lisLength; i++) {
            bool found = false;
            for (int j = rangeHi - 1; j >= rangeLo; j--) {
                if (K <= dag[i][j].count) {
                    res.push_back(dag[i][j].value);
                    tie(rangeLo, rangeHi) = dag[i][j].next;
                    found = true;
                    break;
                }
                K -= dag[i][j].count;
            }
            if (!found)
                return {};
        }
        return res;
    }

    // kth >= 1, O(N)
    vector<T> findKth(long long K) const {
        vector<T> res;

        vector<vector<long long>> pathRep(lisLength);   // repeat
        pathRep[0].assign(dag[0].size(), 1ll);

        int rangeLo = 0, rangeHi = int(dag[0].size());
        for (int i = 1; i < lisLength; i++) {
            vector<long long> delta(dag[i].size() + 1);

            bool found = false;
            for (int j = rangeHi - 1; j >= rangeLo; ) {
                T val = dag[i - 1][j].value;

                long long cnt = 0;
                vector<tuple<int, int, long long>> ranges;
                for (; j >= 0 && dag[i - 1][j].value == val; j--) {
                    if (dag[i - 1][j].count <= 0)
                        continue;

                    cnt += mult(dag[i - 1][j].count, pathRep[i - 1][j], countLimit);
                    ranges.emplace_back(dag[i - 1][j].next.first, dag[i - 1][j].next.second, pathRep[i - 1][j]);
                }

                if (K <= cnt) {
                    rangeLo = int(dag[i].size());
                    rangeHi = 0;
                    for (auto& it : ranges) {
                        delta[get<0>(it)] += get<2>(it);
                        delta[get<1>(it)] -= get<2>(it);
                        rangeLo = min(rangeLo, get<0>(it));
                        rangeHi = max(rangeHi, get<1>(it));
                    }
                    res.push_back(val);
                    found = true;
                    break;
                }
                K -= cnt;
            }
            if (!found)
                return {};

            pathRep[i].resize(dag[i].size());
            long long cnt = 0;
            for (int j = rangeLo; j < rangeHi; j++) {
                cnt += delta[j];
                pathRep[i][j] = min(countLimit, cnt);
            }
        }

        for (int j = rangeHi - 1; j >= rangeLo; j--) {
            long long cnt = mult(dag[lisLength - 1][j].count, pathRep[lisLength - 1][j], countLimit);
            if (K <= cnt) {
                res.push_back(dag[lisLength - 1][j].value);
                return res;
            }
            K -= cnt;
        }

        return {};
    }

    template <typename FuncT = function<void(int, const vector<T>&)>>
    void iterateAll(const FuncT& f) const {
        vector<T> vec;
        for (int i = int(dag[0].size()) - 1; i >= 0; i--) {
            if (dag[0][i].count > 0)
                dfsIterate(0, i, vec, f);
        }
    }

    vector<vector<T>> findAll() const {
        vector<vector<T>> res;
        iterateAll([&res](const vector<T>& vec) {
            res.push_back(vec);
        });
        return res;
    }

private:
    template <typename U>
    static long long add(long long a, U b, long long countLimit) {
        long long x = a + b;
        if (x > countLimit)
            return countLimit;
        return x;
    }

    template <typename U>
    static long long mult(long long a, U b, long long countLimit) {
        if (a != 0) {
            /*
                  a * b >= countLimit
              <=> b >= countLimit / a
            */
            if (b >= (countLimit + a - 1) / a)
                return countLimit;
        }
        return a * b;
    }

    template <typename FuncT = function<void(int, const vector<T>&)>>
    void dfsIterate(int depth, int index, vector<T>& vec, const FuncT& f) const {
        vec.push_back(dag[depth][index].value);
        if (depth + 1 >= lisLength) {
            f(vec);
        } else {
            for (int i = dag[depth][index].next.second - 1; i >= dag[depth][index].next.first; i--) {
                if (dag[depth + 1][i].count > 0)
                    dfsIterate(depth + 1, i, vec, f);
            }
        }
        vec.pop_back();
    }
};
