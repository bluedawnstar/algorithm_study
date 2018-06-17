// O(nlogn)
class Solution {
public:
    int nthSuperUglyNumber(int n, vector<int>& primes) {
        sort(primes.begin(), primes.end());

        vector<int> dp;
        dp.reserve(n);
        dp.push_back(1);

        priority_queue<pair<long long, int>> Q;
        for (int i = 0; i < (int)primes.size(); i++)
            Q.emplace(-primes[i], i);   // (value, prime index)

        vector<int> idx(primes.size());

        long long value;
        int primeIdx;
        for (int i = 1; i < n; i++) {
            value = -Q.top().first;
            dp.push_back((int)value);

            while (value == -Q.top().first) {
                primeIdx = Q.top().second;
                Q.pop();
                Q.emplace(-1ll * dp[++idx[primeIdx]] * primes[primeIdx], primeIdx);
            }
        }

        return dp.back();
    }
};