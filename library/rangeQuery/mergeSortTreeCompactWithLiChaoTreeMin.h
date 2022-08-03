#pragma once

struct CompactMergeSortTreeWithLiChaoTreeMin {
    const int INF = 0x3f3f3f3f;
    const long long LINF = 0x3f3f3f3f3f3f3f3fll;

    struct Line {
        long long m, b;             // f(x) = m * x + b

        long long get(int x) const {
            return 1ll * m * x + b;
        }
    };

    struct Node {
        Line  line;
        Node* left;
        Node* right;
    };

    int minX, maxX;
    vector<Node*> nodes;

    int N;
    vector<Node*> tree;

    CompactMergeSortTreeWithLiChaoTreeMin() : minX(-INF), maxX(INF), N(0) {
    }

    CompactMergeSortTreeWithLiChaoTreeMin(int minX, int maxX) : minX(minX), maxX(maxX), N(0) {
    }

    CompactMergeSortTreeWithLiChaoTreeMin(int minX, int maxX, const pair<long long, long long> lines[], int n)
        : minX(minX), maxX(maxX), N(0) {
        build(lines, n);
    }

    explicit CompactMergeSortTreeWithLiChaoTreeMin(int minX, int maxX, const vector<pair<long long, long long>>& lines)
        : minX(minX), maxX(maxX), N(0) {
        build(lines);
    }

    ~CompactMergeSortTreeWithLiChaoTreeMin() {
        for (auto* p : nodes)
            delete p;
    }


    // O(N*logN)
    void build(const pair<long long, long long> lines[], int n) {
        N = n + (n & 1);
        tree = vector<Node*>(N * 2);

        vector<vector<pair<long long, long long>>> vec(N * 2);
        for (int i = 0; i < n; i++) {
            vec[N + i].emplace_back(lines[i].first, lines[i].second);
            add(N + i, lines[i].first, lines[i].second);
        }

        for (int i = N - 1; i >= 1; i--) {
            auto& L = vec[i * 2];
            auto& R = vec[i * 2 + 1];
            vec[i].insert(vec[i].end(), L.begin(), L.end());
            vec[i].insert(vec[i].end(), R.begin(), R.end());
            for (auto& it : vec[i])
                add(i, it.first, it.second);
        }
    }

    void build(const vector<pair<long long, long long>>& lines) {
        build(lines.data(), int(lines.size()));
    }

    // O((logN)^2)
    long long query(int left, int right, int X) const {
        long long res = LINF;

        for (int L = left + N, R = right + N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1)
                res = min(res, query(L++, X));
            if ((R & 1) == 0)
                res = min(res, query(R--, X));
        }

        return res;
    }

private:
    Node* createNode(const Line& l) {
        auto* p = new Node{ l, nullptr, nullptr };
        nodes.push_back(p);
        return p;
    }

    // O(logN)
    void add(int index, long long m, long long b) {
        Line l{ m, b };
        insert(l, minX, maxX, tree[index]);
    }

    // O(logN)
    void insert(Line& l, int left, int right, Node*& node) {
        if (!node) {
            node = createNode(l);
            return;
        }

        long long trl = node->line.get(left);
        long long trr = node->line.get(right);
        long long vl = l.get(left);
        long long vr = l.get(right);

        if (trl <= vl && trr <= vr)
            return;

        if (trl > vl && trr > vr) {
            node->line = l;
            return;
        }

        int mid = left + (right - left) / 2;
        if (trl > vl)
            swap(node->line, l);

        if (l.get(mid) > node->line.get(mid))
            insert(l, mid + 1, right, node->right);
        else {
            swap(node->line, l);
            insert(l, left, mid, node->left);
        }
    }

    // O(logN)
    long long query(int index, int x) const {
        return query(x, minX, maxX, tree[index]);
    }

    // O(logN)
    long long query(int x, int left, int right, const Node* node) const {
        if (!node)
            return LINF;

        if (left == right)
            return node->line.get(x);

        int mid = left + (right - left) / 2;
        if (x <= mid)
            return min(node->line.get(x), query(x, left, mid, node->left));
        else
            return min(node->line.get(x), query(x, mid + 1, right, node->right));
    }
};
