class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int ans, minV, maxV;

        ans = minV = maxV = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            int a = nums[i] * maxV;
            int b = nums[i] * minV;
            minV = min(nums[i], min(a, b));
            maxV = max(nums[i], max(a, b));
            ans = max(ans, maxV);
        }

        return ans;
    }
};