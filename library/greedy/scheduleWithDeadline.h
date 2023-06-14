#pragma once

// https://leetcode.com/problems/course-schedule-iii/description/

struct ScheduleWithDeadline {
    // the maximum number of works that can be taken, O(N*logN)
    // - works = { (duration, lastDay), ... }
    static int schedule(vector<pair<int,int>>& works) {
        sort(works.begin(), works.end(), [](const pair<int,int>& w1, const pair<int,int>& w2) {
            return w1.second < w2.second;
        });

        priority_queue<int> Q; // duration
        int durationSum = 0;
        for (int i = 0; i < int(works.size()); i++) {
            durationSum += works[i].first;
            Q.push(works[i].first);
            if (durationSum > works[i].second) {
                durationSum -= Q.top();
                Q.pop();
            }
        }

        return int(Q.size());
    }
};
