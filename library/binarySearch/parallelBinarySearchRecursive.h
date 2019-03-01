#pragma once

/*
 <Usage>

    RecursiveParallelBinarySearch<int> solver(
        [](int midValue) {
            //... operations to add
        },
        [](int midValue) {
            //... operations to remove
        },
        [](int index, int midValue) -> bool {
            //... operations to check possibility
            return true; // or false
        }
    );

    //...

    auto ans = solver.solve(N, maxValue);
*/

// https://www.acmicpc.net/problem/8217
// 0-based index
template <typename T = int, typename AddOp = function<void(T)>, typename DelOp = function<void(T)>, typename PossibleOp = function<bool(int, T)>>
struct RecursiveParallelBinarySearch {
    AddOp addOp;
    DelOp delOp;
    PossibleOp possibleOp;

    RecursiveParallelBinarySearch(AddOp addOp, DelOp delOp, PossibleOp possibleOp)
        : addOp(addOp), delOp(delOp), possibleOp(possibleOp) {
    }

    vector<int> solve(int N, T lo, T hi) {
        vector<T> ans(N, -1);

        T currValue = -1;
        vector<int> indexes(N);
        iota(indexes.begin(), indexes.end(), 0);

        dfs(ans, lo, hi, indexes, currValue);
        return ans;
    }

    vector<int> solve(int N, T hi) {
        return solve(N, 0, hi);
    }

private:
    void dfs(vector<T>& ans, T lo, T hi, vector<int>& indexes, T& currValue) {
        if (indexes.empty())
            return;

        T mid = lo + (hi - lo) / 2;
        while (currValue < mid)
            addOp(++currValue);

        while (currValue > mid)
            delOp(currValue--);

        vector<int> leftIndexes;
        vector<int> rightIndexes;
        for (int idx : indexes) {
            if (possibleOp(idx, mid)) {
                leftIndexes.push_back(idx);
                ans[idx] = hi;
            } else {
                rightIndexes.push_back(idx);
            }
        }
        indexes.clear();

        if (lo < hi) {
            dfs(ans, lo, mid, leftIndexes, currValue);
            dfs(ans, mid + 1, hi, rightIndexes, currValue);
        }
    }
};
