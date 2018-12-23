#pragma once

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct DynamicSegmentTreeLazy {
    struct Node {
        T       value;

        bool    lazyExist;
        T       lazy;

        Node*   left;
        Node*   right;

        void init(int x) {
            value = x;
            lazyExist = false;
            lazy = 0;

            left = nullptr;
            right = nullptr;
        }
    };

    Node*   root;
    vector<Node*> nodes;

    int     rangeMin;
    int     rangeMax;

    T       defaultValue;
    MergeOp mergeOp;
    BlockOp blockOp;

    DynamicSegmentTreeLazy(int rangeMin, int rangeMax, MergeOp mop, BlockOp bop, T dflt = T())
        : rangeMin(rangeMin), rangeMax(rangeMax), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        root = createNode(defaultValue);
    }

    ~DynamicSegmentTreeLazy() {
        for (auto it : nodes)
            delete it;
    }

    T update(int left, int right, T value) {
        return update(left, right, value, root, rangeMin, rangeMax);
    }

    T query(int left, int right) {
        return query(left, right, root, rangeMin, rangeMax);
    }

private:
    Node* createNode(T val) {
        nodes.push_back(new Node());
        nodes.back()->init(val);
        return nodes.back();
    }

    T update(int left, int right, T value, Node* node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return node->value;

        if (nodeLeft == nodeRight)
            return node->value = value;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (!node->left)
            node->left = createNode(defaultValue);
        if (!node->right)
            node->right = createNode(defaultValue);

        if (node->lazyExist) {
            pushDown(node->lazy, node->left, nodeLeft, mid);
            pushDown(node->lazy, node->right, mid + 1, nodeRight);
            node->lazyExist = false;
        }

        if (left <= nodeLeft && nodeRight <= right) {
            node->lazyExist = true;
            node->lazy = value;
            return node->value = blockOp(value, nodeRight - nodeLeft + 1);
        }

        return node->value = mergeOp(update(left, right, value, node->left, nodeLeft, mid),
                                     update(left, right, value, node->right, mid + 1, nodeRight));
    }

#if 0
    T query(int left, int right, Node* node, int nodeLeft, int nodeRight) {
        if (!node || right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return node->value;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (!node->left)
            node->left = createNode(defaultValue);
        if (!node->right)
            node->right = createNode(defaultValue);

        if (node->lazyExist) {
            pushDown(node->lazy, node->left, nodeLeft, mid);
            pushDown(node->lazy, node->right, mid + 1, nodeRight);
            node->lazyExist = false;
        }

        return mergeOp(query(left, right, node->left, nodeLeft, mid),
                       query(left, right, node->right, mid + 1, nodeRight));
    }
#endif
    T query(int left, int right, Node* node, int nodeLeft, int nodeRight) {
        if (!node || right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return node->value;

        if (node->lazyExist)
            return blockOp(node->lazy, min(right, nodeRight) - max(left, nodeLeft) + 1);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return mergeOp(query(left, right, node->left, nodeLeft, mid),
                       query(left, right, node->right, mid + 1, nodeRight));
    }

    void pushDown(T value, Node *node, int nodeLeft, int nodeRight) {
        if (!node)
            return;

        if (nodeLeft == nodeRight)
            node->value = value;
        else {
            node->lazyExist = true;
            node->lazy = value;
            node->value = blockOp(value, nodeRight - nodeLeft + 1);
        }
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline DynamicSegmentTreeLazy<T, MergeOp, BlockOp>
makeDynamicSegmentTreeLazy(int left, int right, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return SegmentTreeLazy<T, MergeOp, BlockOp>(left, right, mop, bop, dfltValue);
}
