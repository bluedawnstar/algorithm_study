#pragma once

// Li Chao Segment Tree
// https://e-maxx-eng.appspot.com/geometry/convex_hull_trick.html
template <typename T>
struct SegmentTreeLine2DMax {
    static const int INF = 0x3f3f3f3f;
    static const long long LLINF = 0x3f3f3f3f3f3f3f3fll;

    struct Line {
        T m, b;         // f(x) = m * x + b

        long long get(T x) const {
            return 1ll * m * x + b;
        }
    };

    struct Node {
        Line  line;
        Node* left;
        Node* right;
    };

    T minX, maxX;
    Node* root;

    vector<Node*> nodes;

    SegmentTreeLine2DMax() : minX(-INF), maxX(INF), root(nullptr) {
    }

    // inclusive
    SegmentTreeLine2DMax(T _minX, T _maxX) : minX(_minX), maxX(_maxX), root(nullptr) {
    }

    ~SegmentTreeLine2DMax() {
        for (auto* p : nodes)
            delete p;
    }

    void init(T _minX, T _maxX) {
        minX = _minX;
        maxX = _maxX;
        root = nullptr;
    }

    void add(T m, T b) {
        Line l{ m, b };
        insert(l, minX, maxX, root);
    }

    long long query(T x) const {
        return query(x, minX, maxX, root);
    }

private:
    Node* createNode(const Line& l) {
        auto* p = new Node{ l, nullptr, nullptr };
        nodes.push_back(p);
        return p;
    }

    void insert(Line& l, T left, T right, Node*& node) {
        if (!node) {
            node = createNode(l);
            return;
        }

        long long trl = node->line.get(left);
        long long trr = node->line.get(right);
        long long vl = l.get(left);
        long long vr = l.get(right);

        if (trl >= vl && trr >= vr)
            return;

        if (trl < vl && trr < vr) {
            node->line = l;
            return;
        }

        T mid = left + (right - left) / 2;
        if (trl < vl)
            swap(node->line, l);

        if (node->line.get(mid) > l.get(mid))
            insert(l, mid + 1, right, node->right);
        else {
            swap(node->line, l);
            insert(l, left, mid, node->left);
        }
    }

    long long query(T x, T left, T right, const Node* node) const {
        if (!node)
            return -LLINF;

        if (left == right)
            return node->line.get(x);

        T mid = left + (right - left) / 2;
        if (x <= mid)
            return max(node->line.get(x), query(x, left, mid, node->left));
        else
            return max(node->line.get(x), query(x, mid + 1, right, node->right));
    }
};
