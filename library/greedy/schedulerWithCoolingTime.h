#pragma once

struct TaskSchedulerWithCoolingTime {
    // check whether the given task sequence violates the cooling time.
    //  ex)   valid : A -> B -> A -> ..., coolingTime = 1
    //  ex) invalid : A -> B -> A -> ..., coolingTime = 2
    static bool isValidTaskSequence(const vector<int>& taskSequence, int coolingTime) {
        if (taskSequence.empty())
            return true;

        unordered_map<int, int> lastPos;
        for (int i = 0; i < int(taskSequence.size()); i++) {
            if (taskSequence[i] < 0)   // idle
                continue;

            auto it = lastPos.find(taskSequence[i]);
            if (it != lastPos.end() && (i - it->second < coolingTime + 1))
                return false;
            lastPos[taskSequence[i]] = i;
        }

        return true;
    }

    //--- any order (reordering), idle possible

    // https://leetcode.com/problems/task-scheduler/description/

    // O(N)
    // - taskSequence = { taskType, ... }, coolingTime > 0
    // - return the least number of units of times that the CPU will take to finish all the given tasks
    static int leastIntervalToFinishAllTasks(const vector<int>& taskSequence, int coolingTime) {
        if (taskSequence.empty())
            return 0;

        unordered_map<int, int> taskCount;
        for (auto task : taskSequence)
            ++taskCount[task];

        int maxSize = 0;
        int maxSizeCnt = 1;
        for (auto& it : taskCount) {
            if (it.second == maxSize)
                maxSizeCnt++;
            else if (maxSize < it.second) {
                maxSize = it.second;
                maxSizeCnt = 1;
            }
        }

        return max(int(taskSequence.size()), (maxSize - 1) * (coolingTime + 1) + maxSizeCnt);
    }

    // O(N*logN)
    // - taskSequence = { taskType, ... }, coolingTime > 0
    // - return scheduled tasks
    static vector<int> scheduleForLeastIntervalToFinishAllTasks(const vector<int>& taskSequence, int coolingTime) {
        if (taskSequence.empty())
            return {};

        unordered_map<int, int> taskCount;  // taskCount[task] = count
        for (auto task : taskSequence)
            ++taskCount[task];

        vector<pair<int, int>> freqToTask;  // { (count, task), ... }
        freqToTask.reserve(taskCount.size());
        for (auto& it : taskCount)
            freqToTask.emplace_back(it.second, it.first);
        sort(freqToTask.begin(), freqToTask.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            if (a.first != b.first)
                return a.first > b.first;
            return a.second < b.second;
        });

        // grouping
        int groupN = freqToTask[0].first;
        vector<vector<int>> group(groupN);
        for (int j = 0; j < groupN; j++)
            group[j].push_back(freqToTask[0].second);

        for (int i = 1, g = 0; i < int(freqToTask.size()); i++) {
            int start = (freqToTask[i].first != groupN);
            if (g < start)
                g = start;

            for (int j = 0; j < int(freqToTask[i].first); j++) {
                group[g].push_back(freqToTask[i].second);
                if (++g >= groupN)
                    g = start;
            }
        }

        // assign tasks
        vector<int> res;
        for (int i = 0; i < groupN; i++) {
            if (i > 0 && int(group[i].size()) < (coolingTime + 1)) {
                for (int cnt = coolingTime + 1 - int(group[i].size()); cnt > 0; cnt--)
                    res.push_back(-1);  // idle
            }
            for (int j = int(group[i].size()) - 1; j >= 0; j--)
                res.push_back(group[i][j]);
        }

        return res;
    }

    //--- fixed order, idle possible

    // O(N)
    // the minimum number of idles required to have a cooling time with a fixed order
    static int calcRequiredIdlesToFinishAllTasksWithFixedOrder(const vector<int>& taskSequence, int coolingTime) {
        if (taskSequence.empty())
            return 0;

        int res = 0;
        unordered_map<int, int> lastPos;
        for (int i = 0; i < int(taskSequence.size()); i++) {
            if (taskSequence[i] < 0)   // idle
                continue;

            int curr = i + res;
            auto last = lastPos.find(taskSequence[i]);
            if (last != lastPos.end() && (curr - last->second < coolingTime + 1))
                res += (coolingTime + 1) - (curr - last->second);

            lastPos[taskSequence[i]] = i + res;
        }

        return res;
    }
};
