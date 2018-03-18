#pragma once

#include <vector>
#include <functional>

//--------- Persistent Segment Tree ----------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct PersistentSegmentTree {
    struct Node {
        T       value;
        Node*   left;
        Node*   right;
    };
    vector<Node*>   trees;
    int             N;              // the size of array
    T               defaultValue;
    BinOp           mergeOp;

    explicit PersistentSegmentTree(BinOp op, T dflt = T())
        : N(0), trees(), mergeOp(op), defaultValue(dflt) {
    }

    PersistentSegmentTree(int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        init(n);
    }

    PersistentSegmentTree(T value, int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    PersistentSegmentTree(const T arr[], int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    PersistentSegmentTree(const vector<T>& v, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }

    PersistentSegmentTree(PersistentSegmentTree&& rhs)
        : N(rhs.N), trees(move(rhs.trees)), mergeOp(rhs.mergeOp), defaultValue(rhs.defaultValue),
          nodes(move(rhs.nodes)) {
    }

    ~PersistentSegmentTree() {
        for (auto* p : nodes)
            delete p;
    }


    int getHistorySize() const {
        return (int)trees.size();
    }


    void init(int n) {
        N = n;
        trees.clear();
    }

    T build(T value, int n) {
        init(n);

        auto t = buildSub(value, 0, N - 1);
        trees.push_back(t.second);

        return t.first;
    }

    T build(const T arr[], int n) {
        init(n);

        auto t = buildSub(arr, 0, n -1);
        trees.push_back(t.second);
        return t.first;
    }

    T build(const vector<T>& v) {
        return build(&v[0], (int)v.size());
    }


    // inclusive
    T query(int left, int right) const {
        return querySub(trees.back(), left, right, 0, N - 1);
    }

    // inclusive
    T update(int index, T newValue) {
        return updateSub(trees.back(), index, newValue, 0, N - 1);
    }

    // inclusive
    T updateRange(int left, int right, T newValue) {
        return updateRangeSub(trees.back(), left, right, newValue, 0, N - 1);
    }

    // inclusive
    T upgrade(int index, T newValue) {
        auto t = upgradeSub(trees.back(), index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive
    T upgradeRange(int left, int right, T newValue) {
        auto t = upgradeRangeSub(trees.back(), left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    //--- with history ---

    // inclusive
    T query(int historyIndex, int left, int right) const {
        return querySub(trees[historyIndex], left, right, 0, N - 1);
    }

    // inclusive
    T update(int historyIndex, int index, T newValue) {
        return updateSub(trees[historyIndex], index, newValue, 0, N - 1);
    }

    // inclusive
    T updateRange(int historyIndex, int left, int right, T newValue) {
        return updateRangeSub(trees[historyIndex], left, right, newValue, 0, N - 1);
    }

    // inclusive
    T upgrade(int historyIndex, int index, T newValue) {
        auto t = upgradeSub(trees[historyIndex], index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive
    T upgradeRange(int historyIndex, int left, int right, T newValue) {
        auto t = upgradeRangeSub(trees[historyIndex], left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

private:
    vector<Node*>   nodes;          // allocated nodes

    Node* createNode(T value) {
        auto* p = new Node{ value, nullptr, nullptr };
        nodes.push_back(p);
        return p;
    }
    Node* createNode(T value, Node* left, Node* right) {
        auto* p = new Node{ value, left, right };
        nodes.push_back(p);
        return p;
    }

    // inclusive
    pair<T, Node*> buildSub(T initValue, int left, int right) {
        if (left > right)
            return make_pair(defaultValue, nullptr);

        if (left == right)
            return make_pair(initValue, createNode(initValue));

        int mid = left + (right - left) / 2;
        auto L = buildSub(initValue, left, mid);
        auto R = buildSub(initValue, mid + 1, right);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(value, L.second, R.second));
    }

    // inclusive
    pair<T, Node*> buildSub(const T arr[], int left, int right) {
        if (left > right)
            return make_pair(defaultValue, nullptr);

        if (left == right)
            return make_pair(arr[left], createNode(arr[left]));

        int mid = left + (right - left) / 2;
        auto L = buildSub(arr, left, mid);
        auto R = buildSub(arr, mid + 1, right);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(value, L.second, R.second));
    }

    T querySub(Node* node, int left, int right, int nodeLeft, int nodeRight) const {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return node->value;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return mergeOp(querySub(node->left, left, right, nodeLeft, mid),
                       querySub(node->right, left, right, mid + 1, nodeRight));
    }

    T updateSub(Node* node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return node ? node->value : defaultValue;

        if (nodeLeft == nodeRight)
            return node->value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return node->value = mergeOp(updateSub(node->left, index, newValue, nodeLeft, mid),
                                     updateSub(node->right, index, newValue, mid + 1, nodeRight));
    }

    T updateRangeSub(Node* node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return node ? node->value : defaultValue;

        if (nodeLeft == nodeRight)
            return node->value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return node->value = mergeOp(updateRangeSub(node->left, left, right, newValue, nodeLeft, mid),
                                     updateRangeSub(node->right, left, right, newValue, mid + 1, nodeRight));
    }

    pair<T, Node*> upgradeSub(Node* node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return make_pair(node ? node->value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, createNode(newValue, node->left, node->right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto L = upgradeSub(node->left, index, newValue, nodeLeft, mid);
        auto R = upgradeSub(node->right, index, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(value, L.second, R.second));
    }

    pair<T, Node*> upgradeRangeSub(Node* node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return make_pair(node ? node->value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, createNode(newValue, node->left, node->right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto L = upgradeRangeSub(node->left, left, right, newValue, nodeLeft, mid);
        auto R = upgradeRangeSub(node->right, left, right, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(value, L.second, R.second));
    }
};

template <typename T, typename BinOp>
PersistentSegmentTree<T, BinOp> makePersistentSegmentTree(BinOp op, T dfltValue = T()) {
    return PersistentSegmentTree<T, BinOp>(op, dfltValue);
}

template <typename T, typename BinOp>
PersistentSegmentTree<T, BinOp> makePersistentSegmentTree(const vector<T>& v, BinOp op, T dfltValue = T()) {
    auto segTree = PersistentSegmentTree<T, BinOp>(v, op, dfltValue);
    return segTree;
}

template <typename T, typename BinOp>
PersistentSegmentTree<T, BinOp> makePersistentSegmentTree(const T arr[], int size, BinOp op, T dfltValue = T()) {
    auto segTree = PersistentSegmentTree<T, BinOp>(arr, size, op, dfltValue);
    return segTree;
}
