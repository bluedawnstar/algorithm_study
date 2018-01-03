/**
* Definition for an interval.
* struct Interval {
*     int start;
*     int end;
*     Interval() : start(0), end(0) {}
*     Interval(int s, int e) : start(s), end(e) {}
* };
*/
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        vector<pair<int, int>> delta;
        for (int i = 0; i < (int)intervals.size(); i++) {
            delta.emplace_back(intervals[i].start, 1);
            delta.emplace_back(intervals[i].end, -1);
        }
        sort(delta.begin(), delta.end());

        int ans = 0;
        int cnt = 0;
        for (auto it : delta) {
            cnt += it.second;
            ans = max(ans, cnt);
        }

        return ans;
    }
};

/**
* Definition for an interval.
* struct Interval {
*     int start;
*     int end;
*     Interval() : start(0), end(0) {}
*     Interval(int s, int e) : start(s), end(e) {}
* };
*/
class Solution {
public:
    int minMeetingRooms(vector<Interval>& intervals) {
        sort(intervals.begin(), intervals.end(), [](const Interval& l, const Interval& r) {
            if (l.end != r.end)
                return l.end < r.end;
            return l.start < r.start;
        });

        multiset<int> S;

        int ans = 0;
        for (int i = 0; i < (int)intervals.size(); i++) {
            if (S.empty() || intervals[i].start < *S.begin())
                ans = max(ans, (int)S.size() + 1);
            else
                S.erase(--S.lower_bound(intervals[i].start + 1));
            S.insert(intervals[i].end);
        }

        return ans;
    }
};