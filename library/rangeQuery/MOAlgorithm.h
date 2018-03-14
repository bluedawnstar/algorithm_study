#pragma once

// counting unique values in index range [L, R], inclusive
struct MOAlgorithm {
    vector<int> cnt;    // value to count

    vector<int> query(int maxValue, const vector<int>& in, const vector<pair<int, int>>& Q) {
        vector<int> ans(Q.size());

        vector<tuple<int, int, int>> qry;
        qry.reserve(Q.size());

        for (int i = 0; i < (int)Q.size(); i++)
            qry.emplace_back(Q[i].first, Q[i].second, i);

        int blockN = (int)sqrt(Q.size());
        sort(qry.begin(), qry.end(), [blockN](const auto& l, const auto& r) {
            if (get<0>(l) / blockN != get<0>(r) / blockN)
                return get<0>(l) / blockN < get<0>(r) / blockN;
            return get<1>(l) < get<1>(r);
        });

        cnt.assign(maxValue + 1, 0);

        int L = 1;
        int R = 0;

        int curAns = 0;
        for (auto& q : qry) {
            int left = get<0>(q);
            int right = get<1>(q);

            while (L < left)
                curAns += remove(in[L++]);
            while (L > left)
                curAns += add(in[--L]);

            while (R < right)
                curAns += add(in[++R]);
            while (R > right)
                curAns += remove(in[R--]);

            ans[get<2>(q)] = curAns;
        }

        return ans;
    }

private:
    int add(int x) {
        return (++cnt[x] == 1) ? 1 : 0;
    }

    int remove(int x) {
        return (--cnt[x] == 0) ? -1 : 0;
    }
};
