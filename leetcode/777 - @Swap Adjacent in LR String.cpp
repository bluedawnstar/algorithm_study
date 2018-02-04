// O(n)
class Solution {
public:
    bool canTransform(string start, string end) {
        if (start.length() != end.length())
            return false;

        // remove 'X's
        vector<pair<int, int>> s;
        vector<pair<int, int>> e;
        for (int i = 0; i < (int)start.length(); i++) {
            if (start[i] != 'X')
                s.emplace_back(start[i], i);
            if (end[i] != 'X')
                e.emplace_back(end[i], i);
        }

        // compare the sizes of 'L's and 'R's
        if (s.size() != e.size())
            return false;

        // compare the positions of 'L's and 'R's
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i].first != e[i].first)
                return false;
            else if (s[i].first == 'L' && s[i].second < e[i].second)
                return false;
            else if (s[i].first == 'R' && s[i].second > e[i].second)
                return false;
        }

        return true;
    }
};
