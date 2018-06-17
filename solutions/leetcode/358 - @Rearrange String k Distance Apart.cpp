// O(nlogn)
class Solution {
public:
    string rearrangeString(string s, int k) {
        if (s.empty() || k <= 1)
            return s;

        unordered_map<char, int> cnt;
        for (char c : s)
            ++cnt[c];

        priority_queue<pair<int, char>> Q;
        for (auto it : cnt)
            Q.emplace(it.second, -it.first);

        string res(s.length(), ' ');
        for (int i = 0; i < (int)s.length(); i += k) {
            int n = min((int)s.length() - i, k);

            vector<pair<int, char>> tmp;
            for (int j = 0; j < n; j++) {
                if (Q.empty())
                    return string();

                int n = Q.top().first;
                char c = -Q.top().second;
                Q.pop();

                res[i + j] = c;

                if (n > 1)
                    tmp.emplace_back(n - 1, -c);
            }

            while (!tmp.empty()) {
                Q.push(tmp.back());
                tmp.pop_back();
            }
        }

        return res;
    }
};