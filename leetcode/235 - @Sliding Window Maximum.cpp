// with deque, O(n)
/*
  Values in deque are stored by descending order
   |
   |  |  |
   |  |  |  |
*/
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        if (nums.empty())
            return vector<int>{};

        deque<int> Q;
        for (int i = 0; i < k; i++) {
            while (!Q.empty() && Q.back() < nums[i])
                Q.pop_back();
            Q.push_back(nums[i]);
        }

        vector<int> res;
        res.push_back(Q.front());
        for (int i = k; i < (int)nums.size(); i++) {
            if (!Q.empty() && Q.front() == nums[i - k])
                Q.pop_front();
            while (!Q.empty() && Q.back() < nums[i])
                Q.pop_back();
            Q.push_back(nums[i]);
            res.push_back(Q.front());
        }

        return res;
    }
};

// with multiset, O(nlogn)
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        if (nums.empty())
            return vector<int>{};

        multiset<int> S;
        for (int i = 0; i < k; i++)
            S.insert(nums[i]);

        vector<int> res;
        res.push_back(*S.rbegin());
        for (int i = k; i < (int)nums.size(); i++) {
            S.erase(S.find(nums[i - k]));
            S.insert(nums[i]);
            res.push_back(*S.rbegin());
        }

        return res;
    }
};
