#pragma once

template <int R = 26, int BaseChar = 'a'>
struct PersistentTrie {
    static constexpr int toIndex(char ch) {
        return ch - BaseChar;
    }

    struct Node {
        bool terminal;
        int  children[R];

        Node() {
            init();
        }

        void init() {
            terminal = false;
            memset(children, -1, sizeof(children));
        }
    };

    int initRoot;
    vector<Node> nodes;

    PersistentTrie() {
        initRoot = 0;
        nodes.emplace_back(Node{});
    }

    int init() {
        nodes.clear();
        nodes.emplace_back(Node{});
        return initRoot = 0;
    }

    // return new root
    int insert(int root, const string& s) {
        if (s.empty())
            return root;

        root = cloneNode(root);
        int cur = root;
        for (int i = 0; i < int(s.length()); i++) {
            int next = toIndex(s[i]);
            int nextNode = cloneNode(nodes[cur].children[next]);
            nodes[cur].children[next] = nextNode;
            cur = nextNode;
        }
        nodes[cur].terminal = true;
        return root;
    }

    // return exactly matched word
    int find(int root, const string& s) const {
        auto res = findLongestPrefix(root, s);
        if (res.first != int(s.length()))
            return -1;
        return nodes[res.second].terminal ? res.second : -1;
    }

    // prefix matching
    // return (prefix_matching_length, node_id)
    pair<int, int> findLongestPrefix(int root, const string& s) const {
        int cur = root;
        if (s.empty())
            return make_pair(0, cur);

        int i;
        for (i = 0; i < int(s.length()); i++) {
            int next = toIndex(s[i]);
            if (nodes[cur].children[next] < 0)
                break;
            cur = nodes[cur].children[next];
        }
        return make_pair(i, cur);
    }

private:
    int allocNode() {
        int res = int(nodes.size());
        nodes.emplace_back(Node{});
        return res;
    }

    int cloneNode(int src) {
        int res = int(nodes.size());
        nodes.resize(nodes.size() + 1);
        if (src >= 0)
            nodes.back() = nodes[src];
        return res;
    }
};
