#pragma once

// based on https://github.com/yosupo06/Algorithm/blob/master/src/string/onlinez.hpp

struct OnlineZAlgorithm {
    string s;
    vector<int> z;

    vector<int> pi, next;
    int j;
    vector<pair<int, int>> updateUndo;

    OnlineZAlgorithm() {
        pi.emplace_back(-1);
        j = -1;
    }

    void extend(char c) {
        undo();

        s.push_back(c);
        z.push_back(0);

        if (pi.back() == -1)
            next.push_back(-1);
        else
            next.push_back(s[pi.back()] != c ? pi.back() : next[pi.back()]);

        while (j >= 0 && s.back() != s[j])
            j = pi[j];
        pi.push_back(++j);

        vector<int> res;
        int u = next.back();
        while (u != -1) {
            if (s[u] != c) {
                z[int(s.size()) - 1 - u] = u;
                u = pi[u];
            } else {
                u = next[u];
            }
        }
    }

    void update() {
        if (!updateUndo.empty())
            return;
        updateUndo.emplace_back(-1, j);

        z[0] = int(s.length());

        if (pi.back() == -1)
            next.push_back(-1);
        else
            next.push_back(pi.back());

        while (j >= 0)
            j = pi[j];
        pi.push_back(++j);

        vector<int> res;
        int u = next.back();
        while (u != -1) {
            if (u > 0) {
                updateUndo.emplace_back(int(s.size()) - u, z[int(s.size()) - u]);
                z[int(s.size()) - u] = u;
            }
            u = pi[u];
        }
    }

    // return LCP(s[0:N], s[start:N]), O(1)
    int query(int start) {
        if (updateUndo.empty())
            update();
        return z[start];
    }

private:
    void undo() {
        if (!updateUndo.empty()) {
            next.pop_back();
            pi.pop_back();

            j = updateUndo[0].second;
            for (int i = 1; i < int(updateUndo.size()); i++)
                z[updateUndo[i].first] = updateUndo[i].second;

            updateUndo.clear();
        }
    }
};
