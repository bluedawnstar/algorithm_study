#pragma once

template <typename T>
struct QuadTree {
    static const T MAXX = 0x3f3f3f3f;
    static const T MAXY = 0x3f3f3f3f;

    struct Node {
        int count;
        Node *lt, *rt, *lb, *rb;
    };

    Node* root;
    vector<Node*> nodes;

    QuadTree() : root(nullptr) {
    }

    ~QuadTree() {
        for (auto* p : nodes)
            delete p;
    }

    void insert(T x, T y) {
        root = insert(root, 0, 0, MAXX, MAXY, x, y);
    }

    // O(min(n, MAXX + MAXY)), n = the number of points
    // the number of points in [(x1, y1), (x2,y2)], inclusive
    int count(T x1, T y1, T x2, T y2) const {
        return count(root, 0, 0, MAXX, MAXY, x1, y1, x2, y2);
    }

private:
    Node* insert(Node* node, T x1, T y1, T x2, T y2, T x, T y) {
        if (x < x1 || x > x2 || y < y1 || y > y2)
            return node;

        if (node == nullptr)
            nodes.push_back(node = new Node());

        node->count++;
        if (x1 == x2 && y1 == y2)
            return node;

        T midX = x1 + (x2 - x1) / 2;
        T midY = y1 + (y2 - y1) / 2;

        node->lb = insert(node->lb, x1, y1, midX, midY, x, y);
        node->lt = insert(node->lt, x1, midY + 1, midX, y2, x, y);
        node->rb = insert(node->rb, midX + 1, y1, x2, midY, x, y);
        node->rt = insert(node->rt, midX + 1, midY + 1, x2, y2, x, y);

        return node;
    }

    int count(Node* node, T x1, T y1, T x2, T y2, T qryX1, T qryY1, T qryX2, T qryY2) const {
        if (node == nullptr || x1 > qryX2 || qryX1 > x2 || y1 > qryY2 || qryY1 > y2)
            return 0;

        if (qryX1 <= x1 && x2 <= qryX2 && qryY1 <= y1 && y2 <= qryY2)
            return node->count;

        T midX = x1 + (x2 - x1) / 2;
        T midY = y1 + (y2 - y1) / 2;

        int res = 0;
        res += count(node->lb, x1, y1, midX, midY, qryX1, qryY1, qryX2, qryY2);
        res += count(node->lt, x1, midY + 1, midX, y2, qryX1, qryY1, qryX2, qryY2);
        res += count(node->rb, midX + 1, y1, x2, midY, qryX1, qryY1, qryX2, qryY2);
        res += count(node->rt, midX + 1, midY + 1, x2, y2, qryX1, qryY1, qryX2, qryY2);

        return res;
    }
};
