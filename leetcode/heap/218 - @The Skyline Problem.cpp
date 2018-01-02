// with sorting & multiset
class Solution {
public:
    vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
        if (buildings.empty())
            return vector<pair<int, int>>{};

        vector<pair<int, int>> vlines;   // (x, h)
        for (int i = 0; i < (int)buildings.size(); i++) {
            vlines.emplace_back(buildings[i][0], -buildings[i][2]);
            vlines.emplace_back(buildings[i][1], buildings[i][2]);
        }
        sort(vlines.begin(), vlines.end());

        vector<pair<int, int>> res;
        multiset<int> heights;
        heights.insert(0);
        for (int i = 0; i < (int)vlines.size(); i++) {
            if (vlines[i].second < 0) {
                int h = -vlines[i].second;
                if (*heights.rbegin() < h)
                    res.emplace_back(vlines[i].first, h);
                heights.insert(h);
            } else {
                int h = vlines[i].second;
                heights.erase(heights.find(h));
                if (*heights.rbegin() < h)
                    res.emplace_back(vlines[i].first, *heights.rbegin());
            }
        }

        return res;
    }
};

// with priority queue & multiset
class Solution {
public:
    vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
        if (buildings.empty())
            return vector<pair<int, int>>{};

        priority_queue<pair<int, int>> Q; // (x, h)
        for (int i = 0; i < (int)buildings.size(); i++) {
            Q.emplace(-buildings[i][0], buildings[i][2]);
            Q.emplace(-buildings[i][1], -buildings[i][2]);
        }

        vector<pair<int, int>> res;
        multiset<int> heights;
        heights.insert(0);
        while (!Q.empty()) {
            int x = -Q.top().first;
            int h = Q.top().second;
            Q.pop();

            if (h > 0) {
                if (*heights.rbegin() < h)
                    res.emplace_back(x, h);
                heights.insert(h);
            } else {
                h = -h;
                heights.erase(heights.find(h));
                if (*heights.rbegin() < h)
                    res.emplace_back(x, *heights.rbegin());
            }
        }

        return res;
    }
};