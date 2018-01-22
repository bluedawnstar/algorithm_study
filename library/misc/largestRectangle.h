#pragma once

int largestRectangle(vector<int>& heights) {
    stack<pair<int, int>> stk;

    stk.emplace(-1, -1);    // (height, index)
    heights.push_back(0);

    int res = 0;
    for (int i = 0; i < (int)heights.size(); i++) {
        auto R = i - 1;
        while (stk.top().first >= heights[i]) {
            int h = stk.top().first;
            stk.pop();
            res = max(res, h * (R - stk.top().second));
        }
        stk.emplace(heights[i], i);
    }

    return res;
}

int largestRectangle(const vector<vector<bool>>& matrix) {
    int rows = matrix.size();
    if (rows == 0)
        return 0;
    int cols = matrix[0].size();

    vector<int> height(cols + 1, 0);    // heights

    int res = 0;
    for (int i = 0; i < rows; i++) {
        stack<pair<int, int>> stk;       // (height, index)
        stk.emplace(-1, -1);

        for (int j = 0; j <= cols; j++) {
            if (j >= cols || !matrix[i][j])
                height[j] = 0;
            else
                height[j]++;

            int R = j - 1;
            while (stk.top().first >= height[j]) {
                int h = stk.top().first;
                stk.pop();
                res = max(res, (R - stk.top().second) * h);
            }
            stk.emplace(height[j], j);
        }
    }

    return res;
}
