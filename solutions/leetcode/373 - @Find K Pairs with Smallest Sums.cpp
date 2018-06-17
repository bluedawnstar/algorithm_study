class Solution {
public:
    vector<pair<int, int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<pair<int, int>> res;

        if (nums1.empty() || nums2.empty())
            return res;

        bool reverse = nums1.size() < nums2.size();
        if (reverse)
            swap(nums1, nums2);

        // O(min(n, m) * log min(n, m))
        priority_queue<tuple<int, int, int>> Q;
        for (int i = 0; i < (int)nums2.size(); i++)
            Q.emplace(-(nums1[0] + nums2[i]), 0, i);

        // O(k * log min(n, m))
        while (k-- > 0 && !Q.empty()) {
            int idx1 = get<1>(Q.top());
            int idx2 = get<2>(Q.top());
            Q.pop();

            if (reverse)
                res.emplace_back(nums2[idx2], nums1[idx1]);
            else
                res.emplace_back(nums1[idx1], nums2[idx2]);

            if (++idx1 < (int)nums1.size())
                Q.emplace(-(nums1[idx1] + nums2[idx2]), idx1, idx2);
        }

        return res;
    }
};