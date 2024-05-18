#pragma once

struct TaskSchedulerWithCoolingTimeWithoutIdle {
    // check whether the given task sequence violates the cooling time.
    //  ex)   valid : A -> B -> A -> ..., coolingTime = 1
    //  ex) invalid : A -> B -> A -> ..., coolingTime = 2
    static bool isValidTaskSequence(const vector<int>& taskSequence, int coolingTime) {
        if (taskSequence.empty())
            return true;

        unordered_map<int, int> lastPos;
        for (int i = 0; i < int(taskSequence.size()); i++) {
            auto it = lastPos.find(taskSequence[i]);
            if (it != lastPos.end() && (i - it->second < coolingTime + 1))
                return false;
            lastPos[taskSequence[i]] = i;
        }

        return true;
    }

    //--- any order

    // https://leetcode.com/problems/maximum-number-of-weeks-for-which-you-can-work/description/

    // O(N)
    // - taskCount = { task0_count, task1_count, task2_count, ... }
    // - returns the maximum number of tasks that can be completed without idle.
    static long long countPossibleTasksWithCoolingTime1(const vector<int>& taskCount) {
        int N = int(taskCount.size());
        if (N == 1)
            return 1;

        long long total = 0;
        int maxCnt = 0;
        for (auto cnt : taskCount) {
            total += cnt;
            maxCnt = max(maxCnt, cnt);
        }

        //return total - max(0ll, maxCnt - (total - maxCnt) - 1);
        //return total - max(0ll, maxCnt * 2 - total - 1);
        auto other = total - maxCnt;
        if (other >= maxCnt - 1)
            return total;
        return other * 2 + 1;
    }

    static long long countPossibleTasks(vector<int> taskCount, int coolingTime) {
        int N = int(taskCount.size());
        if (N == 1)
            return 1;

        sort(taskCount.begin(), taskCount.end(), greater<int>());

        long long total = 0;
        vector<pair<int, int>> groups;
        for (auto cnt : taskCount) {
            total += cnt;
            if (groups.empty() || groups.back().first != cnt)
                groups.emplace_back(cnt, 1);
            else
                ++groups.back().second;
        }

        int cntSum = 0;
        long long sum = 0;
        for (int i = 0; i < int(groups.size()); i++) {
            auto size = (total - sum) / (coolingTime + 1 - cntSum);
            if (size >= groups[i].first) {
                if (i == 0)
                    return total;
                auto rem = (total - sum) % (coolingTime + 1 - cntSum);
                return 1ll * size * (coolingTime + 1) + cntSum + rem;
            }

            cntSum += groups[i].second;
            sum += 1ll * groups[i].first * groups[i].second;
        }

        return cntSum;
    }

    static vector<int> generateTaskSequence(vector<int> taskCount, int coolingTime) {
        int N = int(taskCount.size());
        if (N == 1)
            return { 0 };

        vector<pair<int, int>> freq;    // { (val, index), ... }
        for (int i = 0; i < int(taskCount.size()); i++)
            freq.emplace_back(taskCount[i], i);
        sort(freq.begin(), freq.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            if (a.first != b.first)
                return a.first > b.first;
            return a.second < b.second;
        });

        long long total = 0;
        vector<pair<int, vector<int>>> groups;
        for (auto it : freq) {
            total += it.first;
            if (groups.empty() || groups.back().first != it.first)
                groups.emplace_back(it.first, vector<int>{ it.second });
            else
                groups.back().second.push_back(it.second);
        }

        vector<vector<int>> vv;

        int cntSum = 0;
        long long sum = 0;
        for (int i = 0; i < int(groups.size()); i++) {
            auto size = (total - sum) / (coolingTime + 1 - cntSum);
            if (size >= groups[i].first) {
                auto rem = (total - sum) % (coolingTime + 1 - cntSum);

                if (int(vv.size()) < size + (rem > 0))
                    vv.resize(size + (rem > 0));

                vector<pair<int, int>> values;  // { (value, count}, ... }
                for (int t = i; t < int(groups.size()); t++) {
                    for (int j = int(groups[t].second.size()) - 1; j >= 0; j--)
                        values.emplace_back(groups[t].second[j], groups[t].first);
                }

                for (int k = 0; !values.empty(); ) {
                    vv[k].push_back(values.back().first);
                    if (--values.back().second <= 0)
                        values.pop_back();
                    if (++k >= size + (rem > 0)) {
                        k = 0;
                        rem--;
                    }
                }

                vector<int> res;
                for (auto& it : vv) {
                    res.insert(res.end(), it.begin(), it.end());
                    if (it.size() <= coolingTime)
                        break;
                }

                return res;
            }

            cntSum += int(groups[i].second.size());
            sum += 1ll * groups[i].first * int(groups[i].second.size());

            //---

            if (vv.size() < groups[i].first)
                vv.resize(groups[i].first);

            for (auto x : groups[i].second) {
                for (int j = 0; j < groups[i].first; j++)
                    vv[j].push_back(x);
            }
        }

        vector<int> res;
        for (int i = 0; i < N; i++)
            res.push_back(i);
        return res;
    }
};
