// O(n)
class Solution {
public:
    string frequencySort(string s) {
        vector<int> cnt(256);
        for (char c : s)
            ++cnt[c];

        vector<vector<char>> bucket(s.length() + 1);

        for (int i = 0; i < (int)cnt.size(); i++)
            if (cnt[i])
                bucket[cnt[i]].push_back(char(i));

        string res;
        res.reserve(s.length());
        for (int n = (int)bucket.size() - 1; n > 0; n--) {
            for (char c : bucket[n])
                res.append(n, c);
        }

        return res;
    }
};

// O(nlogn)
class Solution {
public:
    string frequencySort(string s) {
        unordered_map<char, int> M;
        for (char c : s)
            ++M[c];

        priority_queue<pair<int, char>> Q;
        for (auto it : M)
            Q.emplace(it.second, -it.first);

        string res;
        while (!Q.empty()) {
            res.append(Q.top().first, -Q.top().second);
            Q.pop();
        }

        return res;
    }
};