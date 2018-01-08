class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());

        vector<int> prefixSum(nums.size());
        for (int i = 1; i < (int)nums.size(); i++)
            prefixSum[i] = prefixSum[i - 1] + nums[i] - nums[i - 1];

        int lo = 0, hi = (int)prefixSum.back();
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;

            int cnt = 0;
            for (int i = (int)prefixSum.size() - 2, j = i + 1; i >= 0; i--) {
                while (i < j && prefixSum[j] - prefixSum[i] > mid)
                    j--;
                cnt += j - i;
            }

            if (cnt >= k)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return lo;
    }
};