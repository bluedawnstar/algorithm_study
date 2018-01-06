class Solution {
public:
    int trapRainWater(vector<vector<int>>& heightMap) {
        if (heightMap.empty() || heightMap[0].empty())
            return 0;

        int rowN = (int)heightMap.size();
        int colN = (int)heightMap[0].size();
        if (rowN < 3 || colN < 3)
            return 0;

        vector<vector<bool>> visited(rowN, vector<bool>(colN));
        priority_queue<tuple<int, int, int>> Q;   // (-height, row, col)

        for (int c = 0; c < colN; c++) {
            Q.emplace(-heightMap[0][c], 0, c);
            visited[0][c] = true;
            Q.emplace(-heightMap[rowN - 1][c], rowN - 1, c);
            visited[rowN - 1][c] = true;
        }
        for (int r = 0; r < rowN; r++) {
            Q.emplace(-heightMap[r][0], r, 0);
            visited[r][0] = true;
            Q.emplace(-heightMap[r][colN - 1], r, colN - 1);
            visited[r][colN - 1] = true;
        }

        static int offset[][2] = {
            { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 }
        };

        int ans = 0;
        while (!Q.empty()) {
            int h = -get<0>(Q.top());
            int r = get<1>(Q.top());
            int c = get<2>(Q.top());
            Q.pop();

            for (int i = 0; i < 4; i++) {
                int r2 = r + offset[i][0];
                int c2 = c + offset[i][1];
                if (r2 < 0 || r2 >= rowN || c2 < 0 || c2 >= colN || visited[r2][c2])
                    continue;

                int h2 = max(h, heightMap[r2][c2]);
                ans += h2 - heightMap[r2][c2];

                Q.emplace(-h2, r2, c2);
                visited[r2][c2] = true;
            }
        }

        return ans;
    }
};