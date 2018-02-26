#pragma once

vector<int> countUniqueNumbers(const vector<int>& in, const vector<pair<int, int>>& rangeQuery) {
    int N = (int)in.size();

    //--- compaction of input values

    vector<int> t(in.begin(), in.end());
    sort(t.begin(), t.end());
    t.erase(unique(t.begin(), t.end()), t.end());

    unordered_map<int, int> M;
    for (int i = 0; i < (int)t.size(); i++) {
        M[t[i]] = i;
    }

    vector<int> A(N);
    for (int i = 0; i < N; i++)
        A[i] = M[in[i]];

    //--- query

    int Q = (int)rangeQuery.size();

    vector<int> ans(Q);
    vector<vector<pair<int, int>>> qry(N);   // [left] -> (right, ans index)
    for (int i = 0; i < Q; i++)
        qry[rangeQuery[i].first].emplace_back(rangeQuery[i].second, i);

    FenwickTree<int> tree(N + 1);

    vector<int> first(N, -1);
    vector<int> last(N, -1);
    vector<int> next(N + 1, N);

    for (int i = 0; i < N; i++) {
        auto l = last[A[i]];
        if (l >= 0)
            next[l] = i;
        else
            first[A[i]] = i;
        last[A[i]] = i;
    }
    for (int i = 0; i < N; i++) {
        if (first[i] >= 0) {
            tree.add(first[i], 1);
            tree.add(next[first[i]], -1);
        }
    }

    for (int i = 0; i < N; i++) {
        for (auto it : qry[i])
            ans[it.second] = tree.sumRange(i, it.first);

        tree.add(next[i], 2);
        tree.add(next[next[i]], -1);
    }

    return ans;
}
