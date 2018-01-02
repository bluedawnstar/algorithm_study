class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        if (nums.empty())
            return vector<int>{};

        sort(nums.begin(), nums.end());
        priority_queue<pair<int, int>> Q;

        int curr = nums[0];
        int cnt = 1;
        for (int i = 1; i < (int)nums.size(); i++) {
            if (curr == nums[i])
                cnt++;
            else {
                Q.emplace(cnt, curr);
                curr = nums[i];
                cnt = 1;
            }
        }
        Q.emplace(cnt, curr);

        vector<int> res;
        for (int i = 0; i < k; i++) {
            res.push_back(Q.top().second);
            Q.pop();
        }

        return res;
    }
};

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        if (nums.empty())
            return vector<int>{};

        unordered_map<int, int> M;
        for (int x : nums)
            ++M[x];

        priority_queue<pair<int, int>> Q;
        for (auto it : M)
            Q.emplace(it.second, it.first);

        vector<int> res;
        for (int i = 0; i < k; i++) {
            res.push_back(Q.top().second);
            Q.pop();
        }

        return res;
    }
};
