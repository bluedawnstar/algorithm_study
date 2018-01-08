// O((n + k)logn)
class Solution {
public:
    int findMaximizedCapital(int k, int W, vector<int>& Profits, vector<int>& Capital) {
        priority_queue<pair<int, int>> Q;
        for (int i = 0; i < (int)Profits.size(); i++)
            Q.emplace(-Capital[i], Profits[i]);

        priority_queue<int> PQ;
        int ans = W;
        while (k-- > 0) {
            while (!Q.empty() && -Q.top().first <= ans) {
                PQ.push(Q.top().second);
                Q.pop();
            }
            if (!PQ.empty()) {
                ans += PQ.top();
                PQ.pop();
            }
        }

        return ans;
    }
};