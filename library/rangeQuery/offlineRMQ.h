#pragma once

// Similar to convex hull trick
/*
    1. groupping queries by the end position of a range
       head[end_position] = the last of the queries with the same end position
       next[query_index] = the previous of the queries with the same end position

    2. it
 */

template <typename T, typename Compare = less<T>>
struct OfflineRMQ {
    Compare compare;

    vector<int> next;   // the previous of the queries with the same end position
    vector<int> head;   // the last of the queries with the same end position
    vector<int> parent; // 

    // O(N + Q)
    // query[i] = { left, right }, inclusive
    vector<T> solve(const vector<T>& value, const vector<pair<int,int>>& query) {
        int N = int(value.size());
        int Q = int(query.size());

        next.assign(Q, 0);      // 
        head.assign(N, -1);     // 
        parent.assign(N, -1);

        for (int i = 0; i < Q; ++i) {
            next[i] = head[query[i].second];
            head[query[i].second] = i;
        }

        vector<int> stack(N);
        vector<T> result(Q);

        // update the values of the queries with right 'i'
        for (int i = 0, top = 0; i < N; ++i) {
            while (top > 0 && compare(value[i], value[stack[top - 1]]))
                parent[stack[--top]] = i;

            stack[top++] = i;
            for (int it = head[i]; it >= 0; it = next[it])
                result[it] = value[findRoot(query[it].first)];
        }

        return result;
    }

private:
    int findRoot(int x) {
        if (parent[x] == -1)
            return x;
        return parent[x] = findRoot(parent[x]);
    };
};
