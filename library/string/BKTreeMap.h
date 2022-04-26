#pragma once

// Burkhard-Keller Tree
// https://en.wikipedia.org/wiki/BK-tree
// https://www.geeksforgeeks.org/bk-tree-introduction-implementation/

template <typename DistanceOp = function<int(const string&, const string&)>>
struct BKTreeMap {
    struct Node {
        string word;

        map<int, int> next;

        Node() {
        }

        explicit Node(string x) : word(x) {
        }
    };

    vector<Node> nodes;
    DistanceOp distOp;

    explicit BKTreeMap(const DistanceOp& distOp) : distOp(distOp) {
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
            auto it = nodes[curr].next.find(dist);
            if (it == nodes[curr].next.end()) {
                auto node = createNode(s);
                nodes[curr].next[dist] = node;
                curr = node;
                break;
            }
            curr = it->second;
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
        int end = dist + threshold + 1;
        for (auto it = nodes[curr].next.lower_bound(start); it != nodes[curr].next.end() && it->first < end; ++it)
            dfsSearchSimilarWords(res, it->second, s, threshold);
    }
};
