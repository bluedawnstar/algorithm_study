#pragma once

// Burkhard-Keller Tree
// https://en.wikipedia.org/wiki/BK-tree
// https://www.geeksforgeeks.org/bk-tree-introduction-implementation/

template <typename DistanceOp = function<int(const string&, const string&)>>
struct BKTreeAM {
    static int popcnt(unsigned x) {
#ifndef __GNUC__
        return int(__popcnt(x));
        /*
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return int(x & 0x0000003F);
        */
#else
        return __builtin_popcount(x);
#endif
    }

    struct Node {
        string word;

        unsigned nextSet;
        vector<int> next;   // 

        Node() : nextSet(0u) {
        }

        explicit Node(string x) : word(x), nextSet(0u) {
        }

        bool isEmpty() const {
            return nextSet == 0;
        }

        bool hasNext(int chIdx) const {
            return (nextSet & (1u << chIdx)) != 0;
        }

        int getNext(int chIdx) const {
            if ((nextSet & (1u << chIdx)) == 0)
                return -1;
            int idx = popcnt(nextSet & ((1u << chIdx) - 1));
            return next[idx];
        }

        void setNext(int chIdx, int node) {
            int idx = popcnt(nextSet & ((1u << chIdx) - 1));
            if ((nextSet & (1u << chIdx)) != 0) {
                next[idx] = node;
            } else {
                next.insert(next.begin() + idx, node);
                nextSet |= (1u << chIdx);
            }
        }

        void eraseNext(int chIdx) {
            int idx = popcnt(nextSet & ((1 << chIdx) - 1));
            if ((nextSet & (1u << chIdx)) != 0) {
                next.erase(next.begin() + idx);
                nextSet &= ~(1u << chIdx);
            }
        }
    };

    vector<Node> nodes;
    DistanceOp distOp;

    explicit BKTreeAM(const DistanceOp& distOp) : distOp(distOp) {
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
            if (!nodes[curr].hasNext(dist)) {
                auto node = createNode(s);
                nodes[curr].setNext(dist, node);
                curr = node;
                break;
            }
            curr = nodes[curr].getNext(dist);
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
        for (int i = start; i < end; i++)
            dfsSearchSimilarWords(res, nodes[curr].getNext(i), s, threshold);
    }
};
