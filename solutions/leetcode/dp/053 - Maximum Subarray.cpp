class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.empty())
            return 0;

        int res = nums[0];
        int sum = nums[0];
        for (int i = 1; i < (int)nums.size(); i++) {
            sum = max(sum + nums[i], nums[i]);
            res = max(res, sum);
        }

        return res;
    }
};