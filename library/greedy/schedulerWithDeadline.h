#pragma once

// https://leetcode.com/problems/course-schedule-iii/description/

struct TaskSchedulerWithDeadline {
    // the maximum number of tasks that can be taken, O(N*logN)
    // - tasks = { (duration, lastDay), ... }
    static int scheduleForMaxWorks(vector<pair<int,int>>& tasks) {
        sort(tasks.begin(), tasks.end(), [](const pair<int,int>& w1, const pair<int,int>& w2) {
            return w1.second < w2.second;
        });

        priority_queue<int> Q; // duration
        int durationSum = 0;
        for (int i = 0; i < int(tasks.size()); i++) {
            durationSum += tasks[i].first;
            Q.push(tasks[i].first);
            if (durationSum > tasks[i].second) {
                durationSum -= Q.top();
                Q.pop();
            }
        }

        return int(Q.size());
    }
};
