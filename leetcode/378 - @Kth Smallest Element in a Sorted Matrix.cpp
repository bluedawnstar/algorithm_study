// O(n * log maxvalue)
class Solution {
public:
    int countLessAndEqual(vector<vector<int>>& matrix, int val) {
        int h = (int)matrix.size();
        int w = (int)matrix[0].size();

        int row = h - 1;
        int col = 0;

        int exclude = 0;
        while (row >= 0 && col < w) {
            if (matrix[row][col] <= val) {
                col++;
            } else {
                row--;
                exclude += w - col;
            }
        }

        return w * h - exclude;
    }

    int kthSmallest(vector<vector<int>>& matrix, int k) {
        if (matrix.empty() || matrix[0].empty())
            return 0;

        int lo = matrix[0][0], hi = matrix.back().back();

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (countLessAndEqual(matrix, mid) >= k)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return lo;
    }
};

// O(n^2 * logn)
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        if (matrix.empty() || matrix[0].empty())
            return 0;

        int h = (int)matrix.size();
        int w = (int)matrix[0].size();

        priority_queue<pair<int, int>> Q;
        for (int i = 0; i < h; i++)
            Q.emplace(matrix[i].back(), i);

        vector<int> idx(h, w - 1);

        int n = w * h - k;
        while (n-- > 0) {
            int val = Q.top().first;
            int row = Q.top().second;
            Q.pop();

            if (idx[row] > 0)
                Q.emplace(matrix[row][--idx[row]], row);
        }

        return Q.top().first;
    }
};