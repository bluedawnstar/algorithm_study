#pragma once

// Burkhard-Keller Tree
// https://en.wikipedia.org/wiki/BK-tree
// https://www.geeksforgeeks.org/bk-tree-introduction-implementation/

template <int MaxDistance = 20, typename DistanceOp = function<int(const string&, const string&)>>
struct BKTree {
    struct Node {
        string word;

        int next[MaxDistance];  // 

        Node() {
            fill(next, next + MaxDistance, -1);
        }

        explicit Node(string x) : word(x) {
            fill(next, next + MaxDistance, -1);
        }
    };

    vector<Node> nodes;
    DistanceOp distOp;

    explicit BKTree(const DistanceOp& distOp) : distOp(distOp) {
    }

    void clear() {
        nodes.clear();
    }

    int add(const string& s) {
        if (nodes.empty()) {
            nodes.emplace_back(s);
            return 0;
        }

        int curr = 0;
        while (true) {
            int dist = distOp(nodes[curr].word, s);
            if (nodes[curr].next[dist] < 0) {
                auto node = createNode(s);
                nodes[curr].next[dist] = node;
                curr = node;
                break;
            }
            curr = nodes[curr].next[dist];
        }

        return curr;
    }

    vector<string> searchSimilarWords(const string& s, int threshold = 2) const {
        vector<string> res;
        if (!nodes.empty()) {
            vector<int> selected;
            dfsSearchSimilarWords(selected, 0, s, threshold);
            for (auto i : selected)
                res.push_back(nodes[i].word);
        }
        return res;
    }

private:
    int createNode(const string& s) {
        int res = int(nodes.size());
        nodes.emplace_back(s);
        return res;
    }

    void dfsSearchSimilarWords(vector<int>& res, int curr, const string& s, int threshold) const {
        if (curr < 0)
            return;

        int dist = distOp(nodes[curr].word, s);
        if (dist <= threshold)
            res.push_back(curr);

        int start = max(0, dist - threshold);
        int end = min(MaxDistance, dist + threshold + 1);
        for (int i = start; i < end; i++)
            dfsSearchSimilarWords(res, nodes[curr].next[i], s, threshold);
    }
};
