// with stack, O(n)
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        stack<pair<int, int>> stk;

        stk.emplace(-1, -1);    // (height, index)
        heights.push_back(0);

        int res = 0;
        for (int i = 0; i < (int)heights.size(); i++) {
            auto R = stk.top().second;
            while (stk.top().first >= heights[i]) {
                int h = stk.top().first;
                stk.pop();
                res = max(res, h * (R - stk.top().second));
            }
            stk.emplace(heights[i], i);
        }

        return res;
    }
};