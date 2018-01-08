// O(n)
class Solution {
public:
    bool isPossible(vector<int>& nums) {
        vector<pair<int, int>> cnt;
        cnt.emplace_back(nums[0], 1);
        for (int i = 1; i < (int)nums.size(); i++) {
            if (cnt.back().first == nums[i])
                cnt.back().second++;
            else
                cnt.emplace_back(nums[i], 1);
        }

        int len1 = cnt[0].second, len2 = 0;
        for (int i = 1; i < (int)cnt.size(); i++) {
            if (cnt[i].first != cnt[i - 1].first + 1) {
                if (len1 || len2)
                    return false;

                len1 = cnt[i].second;
                len2 = 0;
            } else {
                int n = min(cnt[i].second, cnt[i - 1].second);
                if (len1 + len2 > n)
                    return false;

                len2 = len1;
                len1 = max(0, cnt[i].second - cnt[i - 1].second);
            }
        }

        return !len1 && !len2;
    }
};

// with hash map, O(n)
class Solution {
public:
    bool isPossible(vector<int>& nums) {
        unordered_map<int, priority_queue<int>> M;   // number -> lens

        for (int i = 0; i < (int)nums.size(); i++) {
            int x = nums[i];

            int len = 0;
            if (!M[x - 1].empty()) {
                len = -M[x - 1].top();
                M[x - 1].pop();
            }
            M[x].push(-(len + 1));
        }

        for (auto& it : M) {
            if (!it.second.empty() && -it.second.top() < 3)
                return false;
        }

        return true;
    }
};
