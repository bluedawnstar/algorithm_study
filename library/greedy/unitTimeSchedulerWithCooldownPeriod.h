#pragma once

struct UnitTimeTaskSchedulerWithCooldownPeriod {
    static bool isValidSchedule(const vector<int>& unitTimeTasks, int cooldownPeriod) {
        if (unitTimeTasks.empty())
            return true;

        unordered_map<int, int> lastPos;
        for (int i = 0; i < int(unitTimeTasks.size()); i++) {
            if (unitTimeTasks[i] < 0)   // idle
                continue;

            auto it = lastPos.find(unitTimeTasks[i]);
            if (it != lastPos.end() && (i - it->second < cooldownPeriod + 1))
                return false;
            lastPos[unitTimeTasks[i]] = i;
        }

        return true;
    }

    //--- any order (reordering)
    // https://leetcode.com/problems/task-scheduler/description/

    // O(N)
    // - unitTimeTasks = { taskType, ... }
    // - return the least number of units of times that the CPU will take to finish all the given tasks
    static int leastInterval(const vector<int>& unitTimeTasks, int cooldownPeriod) {
        if (unitTimeTasks.empty())
            return 0;

        unordered_map<int, int> taskCount;
        for (auto task : unitTimeTasks)
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

        return max(int(unitTimeTasks.size()), (maxSize - 1) * (cooldownPeriod + 1) + maxSizeCnt);
    }

    // O(N*logN)
    // - unitTimeTasks = { taskType, ... }
    // - return scheduled tasks
    static vector<int> scheduleForLeastInterval(const vector<int>& unitTimeTasks, int cooldownPeriod) {
        if (unitTimeTasks.empty())
            return {};

        unordered_map<int, int> taskCount;  // taskCount[task] = count
        for (auto task : unitTimeTasks)
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
            if (i > 0 && int(group[i].size()) < (cooldownPeriod + 1)) {
                for (int cnt = cooldownPeriod + 1 - int(group[i].size()); cnt > 0; cnt--)
                    res.push_back(-1);  // idle
            }
            for (int j = int(group[i].size()) - 1; j >= 0; j--)
                res.push_back(group[i][j]);
        }

        return res;
    }

    //--- fixed order

    // the minimum number of idles required to have a cooldown period with a fixed order
    // O(N)
    static int calcRequiredIdlesWithFixedOrder(const vector<int>& unitTimeTasks, int cooldownPeriod) {
        if (unitTimeTasks.empty())
            return 0;

        int res = 0;
        unordered_map<int, int> lastPos;
        for (int i = 0; i < int(unitTimeTasks.size()); i++) {
            if (unitTimeTasks[i] < 0)   // idle
                continue;

            int curr = i + res;
            auto last = lastPos.find(unitTimeTasks[i]);
            if (last != lastPos.end() && (curr - last->second < cooldownPeriod + 1))
                res += (cooldownPeriod + 1) - (curr - last->second);

            lastPos[unitTimeTasks[i]] = i + res;
        }

        return res;
    }

};

/*
    Q) 만일 reordering할 수 없고 idle만 넣을 수 있다면?
*/