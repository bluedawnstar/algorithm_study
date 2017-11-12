#pragma once

#include <vector>
#include <functional>

//--------- Persistent Segment Tree ----------------------------------------------

//[CAUTION!] This class doesn't free created nodes

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

    PersistentSegmentTree(T dflt = T())
        : N(0), trees(), mergeOp(), defaultValue(dflt) {
    }

    PersistentSegmentTree(BinOp op, T dflt = T())
        : N(0), trees(), mergeOp(op), defaultValue(dflt) {
    }

    int getHistorySize() const {
        return (int)trees.size();
    }

    // inclusive
    T init(T value, int N) {
        this->N = N;
        trees.clear();

        auto t = initSub(value, 0, N - 1);
        trees.push_back(t.second);

        return t.first;
    }

    // inclusive
    T build(const vector<T>& v, int left, int right) {
        N = right - left + 1;
        trees.clear();

        auto t = buildSub(v, left, right);
        trees.push_back(t.second);

        return t.first;
    }
    // inclusive
    T build(const T arr[], int left, int right) {
        N = right - left + 1;
        trees.clear();

        auto t = buildSub(arr, left, right);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive
    T query(int left, int right) {
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
    T query(int historyIndex, int left, int right) {
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
    Node* createNode(T value) {
        return new Node{ value, nullptr, nullptr };
    }
    Node* createNode(T value, Node* left, Node* right) {
        return new Node{ value, left, right };
    }

    // inclusive
    pair<T, Node*> initSub(T initValue, int left, int right) {
        if (left > right)
            return make_pair(defaultValue, nullptr);

        if (left == right)
            return make_pair(initValue, createNode(initValue));

        int mid = left + (right - left) / 2;
        auto L = initSub(initValue, left, mid);
        auto R = initSub(initValue, mid + 1, right);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(value, L.second, R.second));
    }

    // inclusive
    template <typename U>
    pair<T,Node*> buildSub(const U& arr, int left, int right) {
        if (left > right)
            return make_pair(defaultValue, nullptr);

        if (left == right)
            return make_pair(arr[left], createNode(arr[left]));

        int mid = left + (right - left) / 2;
        auto L = buildSub<U>(arr, left, mid);
        auto R = buildSub<U>(arr, mid + 1, right);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(value, L.second, R.second));
    }

    T querySub(Node* node, int left, int right, int nodeLeft, int nodeRight) {
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
    return std::move(PersistentSegmentTree<T, BinOp>(op, dfltValue));
}

template <typename T, typename BinOp>
PersistentSegmentTree<T, BinOp> makePersistentSegmentTree(int size, BinOp op, T dfltValue = T()) {
    auto segTree = PersistentSegmentTree<T, BinOp>(op, dfltValue);
    segTree.init(dfltValue, 0, size - 1);
    return std::move(segTree);
}

template <typename T, typename BinOp>
PersistentSegmentTree<T, BinOp> makePersistentSegmentTree(const vector<T>& v, BinOp op, T dfltValue = T()) {
    auto segTree = PersistentSegmentTree<T, BinOp>(op, dfltValue);
    segTree.build(v, 0, (int)v.size() - 1);
    return std::move(segTree);
}

template <typename T, typename BinOp>
PersistentSegmentTree<T, BinOp> makePersistentSegmentTree(const T arr[], int size, BinOp op, T dfltValue = T()) {
    auto segTree = PersistentSegmentTree<T, BinOp>(op, dfltValue);
    segTree.build(arr, 0, size - 1);
    return std::move(segTree);
}
