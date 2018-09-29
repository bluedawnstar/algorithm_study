#pragma once

template <typename T, typename Compare = less<T>>
struct OfflineRMQ {
    Compare compare;

    vector<int> prev;
    vector<int> head;
    vector<int> parent;

    // O(N + Q)
    // query[i] = { left, right }, inclusive
    vector<T> solve(const vector<T>& value, const vector<pair<int,int>>& query) {
        int N = int(value.size());
        int Q = int(query.size());

        prev.assign(Q, 0);
        head.assign(N, -1);
        parent.assign(N, -1);

        for (int i = 0; i < Q; ++i) {
            prev[i] = head[query[i].second];    // right
            head[query[i].second] = i;
        }

        vector<int> stack(N);
        vector<T> result(Q);

        for (int i = 0, top = 0; i < N; ++i) {
            while (top > 0 && compare(value[i], value[stack[top - 1]]))
                parent[stack[--top]] = i;

            stack[top++] = i;
            for (int it = head[i]; it >= 0; it = prev[it])
                result[it] = value[findRoot(query[it].first)];
        }

        return result;
    }

private:
    // Disjoint-Set
    int findRoot(int x) {
        if (parent[x] == -1)
            return x;
        return parent[x] = findRoot(parent[x]);
    };
};
