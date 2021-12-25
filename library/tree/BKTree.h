#pragma once

const int THRESHOLD = 2;    //* the lowest threshold we desire in our calculation of edit distance
const int MAX_DISTANCE = 20;//* maximum edit distance

typedef function<int(const string&, const string&)> DistanceOp;

// Burkhard-Keller Tree
// https://en.wikipedia.org/wiki/BK-tree
// https://www.geeksforgeeks.org/bk-tree-introduction-implementation/

#ifndef USE_MAP
//#define USE_MAP
#endif

struct BKTree {
    struct Node {
        string word;

#ifndef USE_MAP
        int next[MAX_DISTANCE];  // 

        Node() {
            fill(next, next + MAX_DISTANCE, -1);
        }

        explicit Node(string x) : word(x) {
            fill(next, next + MAX_DISTANCE, -1);
        }
#else
        map<int, int> next;

        Node() {
        }

        explicit Node(string x) : word(x) {
        }
#endif
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
#ifndef USE_MAP
            if (nodes[curr].next[dist] < 0) {
                auto node = createNode(s);
                nodes[curr].next[dist] = node;
                curr = node;
                break;
            }
            curr = nodes[curr].next[dist];
#else
            auto it = nodes[curr].next.find(dist);
            if (it == nodes[curr].next.end()) {
                auto node = createNode(s);
                nodes[curr].next[dist] = node;
                curr = node;
                break;
            }
            curr = it->second;
#endif
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
        int end = min(MAX_DISTANCE, dist + threshold + 1);
#ifndef USE_MAP
        for (int i = start; i < end; i++)
            dfsSearchSimilarWords(res, nodes[curr].next[i], s, threshold);
#else
        for (auto it = nodes[curr].next.lower_bound(start); it != nodes[curr].next.end() && it->first < end; ++it)
                dfsSearchSimilarWords(res, it->second, s, threshold);
#endif
    }
};
