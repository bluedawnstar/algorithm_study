#pragma once

template <typename T>
struct GlobalMinCutNaive64 {
    int N;
    vector<vector<pair<int, T>>> edges;
    vector<T> cost;

    GlobalMinCutNaive64() : N(0) {
    }

    explicit GlobalMinCutNaive64(int n) : N(n), edges(n), cost(n, 0) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, T>>>(n);
        cost.assign(n, 0);
    }

    // add edges to undirected graph
    void addEdge(int u, int v, T w) {
        edges[max(u, v)].emplace_back(min(u, v), w);
        cost[u] += w;
        cost[v] += w;
    }

    //-------------------------------------------------------------------------

    // call this before solve()
    void mergeEdges() {
        for (int i = 0; i < (int)edges.size(); i++) {
            auto& vec = edges[i];
            sort(vec.begin(), vec.end());

            int j = 0;
            for (int i = 1; i < (int)vec.size(); i++) {
                if (vec[j].first == vec[i].first)
                    vec[j].second += vec[i].second;
                else if (j != i)
                    vec[++j] = vec[i];
            }
            vec.resize(j + 1);
        }
    }

    pair<T, unsigned long long> solve() {
        int idx = int(min_element(cost.begin(), cost.end()) - cost.begin());

        ans = cost[idx];
        ansGroup = 1ull << idx;

        dfsSolve(0ull, 0ull, 0, false, 0);
        return make_pair(ans, ansGroup);
    }

private:
    T ans;
    unsigned long long ansGroup;

    void dfsSolve(unsigned long long leftSet, unsigned long long rightSet, int level, bool right, T cut) {
        if (level >= N) {
            if (leftSet && rightSet) {
                ans = cut;
                ansGroup = min(leftSet, rightSet);
            }
        } else {
            T c = cut;
            if (right) {
                for (const auto& e : edges[level]) {
                    if (leftSet & (1ull << e.first))
                        c += e.second;
                }
                rightSet |= (1ull << level);
            } else {
                for (const auto& e : edges[level]) {
                    if (rightSet & (1ull << e.first))
                        c += e.second;
                }
                leftSet |= (1ull << level);
            }
            if (c >= ans)
                return;

            dfsSolve(leftSet, rightSet, level + 1, right, c);
            dfsSolve(leftSet, rightSet, level + 1, !right, c);
        }
    }
};

template <typename T, const T INF = 0x3f3f3f3f>
struct GlobalMinCutNaive {
    int N;
    vector<vector<pair<int, T>>> edges;
    vector<T> cost;

    GlobalMinCutNaive() : N(0) {
    }

    explicit GlobalMinCutNaive(int n) : N(n), edges(n), cost(n, 0) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, T>>>(n);
        cost.assign(n, 0);
    }

    // add edges to undirected graph
    void addEdge(int u, int v, T w) {
        edges[max(u, v)].emplace_back(min(u, v), w);
        cost[u] += w;
        cost[v] += w;
    }

    //-------------------------------------------------------------------------

    // call this before solve()
    void mergeEdges() {
        for (int i = 0; i < (int)edges.size(); i++) {
            auto& vec = edges[i];
            sort(vec.begin(), vec.end());

            int j = 0;
            for (int i = 1; i < (int)vec.size(); i++) {
                if (vec[j].first == vec[i].first)
                    vec[j].second += vec[i].second;
                else if (j != i)
                    vec[++j] = vec[i];
            }
            vec.resize(j + 1);
        }
    }

    T solve() {
        int idx = int(min_element(cost.begin(), cost.end()) - cost.begin());

        ans = cost[idx];

        //ansGroup.assign(N, false);
        //ansGroup[idx] = true;

        vector<bool> leftSet(N);
        dfsSolve(leftSet, 0, 0, true, 0);

        return ans;
    }

private:
    T ans;
    //vector<bool> ansGroup;

    void dfsSolve(vector<bool>& leftSet, int leftCnt, int level, bool left, T cut) {
        if (level >= N) {
            if (!leftSet.empty() && leftCnt != N) {
                ans = cut;
                //ansGroup = leftSet;
            }
        } else {
            T c = cut;
            if (!left) {
                for (const auto& e : edges[level]) {
                    if (leftSet[e.first])
                        c += e.second;
                }
            } else {
                for (const auto& e : edges[level]) {
                    if (!leftSet[e.first])
                        c += e.second;
                }
            }

            if (c >= ans)
                return;

            leftCnt += left;
            leftSet[level] = left;

            dfsSolve(leftSet, leftCnt, level + 1, left, c);
            dfsSolve(leftSet, leftCnt, level + 1, !left, c);
        }
    }
};
