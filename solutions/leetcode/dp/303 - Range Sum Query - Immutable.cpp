// initialize: O(n)
// query: O(1)
class NumArray {
    vector<int> sum;
public:
    NumArray(vector<int> nums) : sum(nums.size() + 1) {
        for (int i = 1; i <= (int)nums.size(); i++) {
            sum[i] = sum[i - 1] + nums[i - 1];
        }
    }

    int sumRange(int i, int j) {
        if (j < 0 || i >= (int)sum.size() - 1)
            return 0;
        return sum[min((int)sum.size() - 1, j + 1)] - sum[max(0, i)];
    }
};

/**
* Your NumArray object will be instantiated and called as such:
* NumArray obj = new NumArray(nums);
* int param_1 = obj.sumRange(i,j);
*/