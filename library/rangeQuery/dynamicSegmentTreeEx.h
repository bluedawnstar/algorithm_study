#pragma once

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct DynamicSegmentTreeLazyEx {
    enum LazyT {
        lzNone,
        lzSet,
        lzAdd
    };

    struct Node {
        T       value;

        LazyT   lazyType;
        T       lazy;

        Node*   left;
        Node*   right;

        void init(T x) {
            value = x;
            lazyType = lzNone;
            lazy = T();

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

    DynamicSegmentTreeLazyEx(int rangeMin, int rangeMax, MergeOp mop, BlockOp bop, T dflt = T())
        : rangeMin(rangeMin), rangeMax(rangeMax), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        root = createNode(defaultValue);
    }

    ~DynamicSegmentTreeLazyEx() {
        for (auto it : nodes)
            delete it;
    }

    T update(int left, int right, T value) {
        return update(left, right, value, root, rangeMin, rangeMax);
    }

    T add(int left, int right, T value) {
        return add(left, right, value, root, rangeMin, rangeMax);
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

        if (left <= nodeLeft && nodeRight <= right) {
            pushDown(lzSet, value, node, nodeLeft, nodeRight);
            return node->value;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (!node->left)
            node->left = createNode(defaultValue);
        if (!node->right)
            node->right = createNode(defaultValue);

        if (node->lazyType != lzNone) {
            pushDown(node->lazyType, node->lazy, node->left, nodeLeft, mid);
            pushDown(node->lazyType, node->lazy, node->right, mid + 1, nodeRight);
            node->lazyType = lzNone;
        }

        return node->value = mergeOp(update(left, right, value, node->left, nodeLeft, mid),
                                     update(left, right, value, node->right, mid + 1, nodeRight));
    }

    T add(int left, int right, T value, Node* node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return node->value;

        if (nodeLeft == nodeRight)
            return node->value += value;

        if (left <= nodeLeft && nodeRight <= right) {
            pushDown(lzAdd, value, node, nodeLeft, nodeRight);
            return node->value;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (!node->left)
            node->left = createNode(defaultValue);
        if (!node->right)
            node->right = createNode(defaultValue);

        if (node->lazyType != lzNone) {
            pushDown(node->lazyType, node->lazy, node->left, nodeLeft, mid);
            pushDown(node->lazyType, node->lazy, node->right, mid + 1, nodeRight);
            node->lazyType = lzNone;
        }

        return node->value = mergeOp(add(left, right, value, node->left, nodeLeft, mid),
                                     add(left, right, value, node->right, mid + 1, nodeRight));
    }

    T query(int left, int right, Node* node, int nodeLeft, int nodeRight) {
        if (!node || right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return node->value;

        if (node->lazyType == lzSet)
            return blockOp(node->lazy, min(right, nodeRight) - max(left, nodeLeft) + 1);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (!node->left)
            node->left = createNode(defaultValue);
        if (!node->right)
            node->right = createNode(defaultValue);

        if (node->lazyType != lzNone) {
            pushDown(node->lazyType, node->lazy, node->left, nodeLeft, mid);
            pushDown(node->lazyType, node->lazy, node->right, mid + 1, nodeRight);
            node->lazyType = lzNone;
        }

        return mergeOp(query(left, right, node->left, nodeLeft, mid),
                       query(left, right, node->right, mid + 1, nodeRight));
    }

    void pushDown(LazyT type, T value, Node* node, int nodeLeft, int nodeRight) {
        if (!node || type == lzNone)
            return;

        if (nodeLeft == nodeRight) {
            if (type == lzSet)
                node->value = value;
            else
                node->value += value;
            return;
        }

        if (type == lzSet) {
            node->lazyType = lzSet;
            node->lazy = value;
            node->value = blockOp(value, nodeRight - nodeLeft + 1);
        } else {
            if (node->lazyType == lzNone) {
                node->lazyType = lzAdd;
                node->lazy = value;
            } else {
                node->lazy += value;
            }
            node->value += blockOp(value, nodeRight - nodeLeft + 1);
        }
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline DynamicSegmentTreeLazyEx<T, MergeOp, BlockOp>
makeDynamicSegmentTreeLazyEx(int left, int right, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return DynamicSegmentTreeLazyEx<T, MergeOp, BlockOp>(left, right, mop, bop, dfltValue);
}
