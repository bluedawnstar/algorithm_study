#pragma once

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct PersistentSegmentTreeLazy {
    struct Node {
        int     id;
        T       value;
        Node*   left;
        Node*   right;
    };
    vector<Node*>   trees;
    vector<unordered_map<int,T>> treesLazy;

    int             N;              // the size of array
    T               defaultValue;
    MergeOp         mergeOp;
    BlockOp         blockOp;

    PersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dflt = T())
        : N(0), trees(), treesLazy(), defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    PersistentSegmentTreeLazy(int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        init(n);
    }

    PersistentSegmentTreeLazy(T value, int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(value, n);
    }

    PersistentSegmentTreeLazy(const T arr[], int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(arr, n);
    }

    PersistentSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(v);
    }

    PersistentSegmentTreeLazy(PersistentSegmentTreeLazy&& rhs)
        : N(rhs.N), trees(move(rhs.trees)), treesLazy(move(rhs.treesLazy)),
          defaultValue(rhs.defaultValue), mergeOp(rhs.mergeOp), blockOp(rhs.blockOp),
          nodes(move(rhs.nodes)) {
    }

    ~PersistentSegmentTreeLazy() {
        for (auto* p : nodes)
            delete p;
    }


    int getHistorySize() const {
        return int(trees.size());
    }


    void init(int n) {
        N = n;
        trees.clear();
        treesLazy.clear();
    }

    T build(T value, int n) {
        init(n);

        treesLazy.resize(1);
        auto t = initSub(value, 0, n - 1, 1);
        trees.push_back(t.second);

        return t.first;
    }

    T build(const T arr[], int n) {
        init(n);

        treesLazy.resize(1);
        auto t = buildSub(arr, 0, n - 1, 1);
        trees.push_back(t.second);

        return t.first;
    }

    T build(const vector<T>& v) {
        return build(&v[0], int(v.size()));
    }


    // inclusive
    T query(int left, int right) {
        return querySub(int(treesLazy.size()) - 1, trees.back(), left, right, 0, N - 1);
    }

    // inclusive
    T update(int index, T newValue) {
        return updateSub(int(treesLazy.size()) - 1, trees.back(), index, newValue, 0, N - 1);
    }

    // inclusive
    T updateRange(int left, int right, T newValue) {
        return updateRangeSub(int(treesLazy.size()) - 1, trees.back(), left, right, newValue, 0, N - 1);
    }

    // inclusive
    T upgrade(int index, T newValue) {
        treesLazy.push_back(treesLazy.back());
        auto t = upgradeSub(int(treesLazy.size()) - 1, trees.back(), index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive
    T upgradeRange(int left, int right, T newValue) {
        treesLazy.push_back(treesLazy.back());
        auto t = upgradeRangeSub(int(treesLazy.size()) - 1, trees.back(), left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    //--- with history ---

    // inclusive
    T query(int historyIndex, int left, int right) {
        return querySub(historyIndex, trees[historyIndex], left, right, 0, N - 1);
    }

    // inclusive
    T update(int historyIndex, int index, T newValue) {
        return updateSub(historyIndex, trees[historyIndex], index, newValue, 0, N - 1);
    }

    // inclusive
    T updateRange(int historyIndex, int left, int right, T newValue) {
        return updateRangeSub(historyIndex, trees[historyIndex], left, right, newValue, 0, N - 1);
    }

    // inclusive
    T upgrade(int historyIndex, int index, T newValue) {
        treesLazy.push_back(treesLazy[historyIndex]);
        auto t = upgradeSub(int(treesLazy.size()) - 1, trees[historyIndex], index, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

    // inclusive
    T upgradeRange(int historyIndex, int left, int right, T newValue) {
        treesLazy.push_back(treesLazy[historyIndex]);
        auto t = upgradeeRangeSub(int(treesLazy.size()) - 1, trees[historyIndex], left, right, newValue, 0, N - 1);
        trees.push_back(t.second);
        return t.first;
    }

private:
    vector<Node*>   nodes;          // allocated nodes

    Node* createNode(int id, T value) {
        auto* p = new Node{ id, value, nullptr, nullptr };
        nodes.push_back(p);
        return p;
    }
    Node* createNode(int id, T value, Node* left, Node* right) {
        auto* p = new Node{ id, value, left, right };
        nodes.push_back(p);
        return p;
    }

    // inclusive
    pair<T, Node*> initSub(T initValue, int left, int right, int node) {
        if (left > right)
            return make_pair(defaultValue, nullptr);

        if (left == right)
            return make_pair(initValue, createNode(node, initValue));

        int mid = left + (right - left) / 2;
        auto L = initSub<U>(initValue, left, mid, node * 2);
        auto R = initSub<U>(initValue, mid + 1, right, node * 2 + 1);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(node, value, L.second, R.second));
    }

    // inclusive
    pair<T, Node*> buildSub(const T arr[], int left, int right, int node) {
        if (left > right)
            return make_pair(defaultValue, nullptr);

        if (left == right)
            return make_pair(arr[left], createNode(node, arr[left]));

        int mid = left + (right - left) / 2;
        auto L = buildSub(arr, left, mid, node * 2);
        auto R = buildSub(arr, mid + 1, right, node * 2 + 1);
        auto value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(node, value, L.second, R.second));
    }

    T querySub(int historyIndex, Node* node, int left, int right, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return defaultValue;

        if (left <= nodeLeft && nodeRight <= right)
            return node->value;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(node->id);
        if (it != treesLazy[historyIndex].end()) {
            pushDown(historyIndex, node->left, it->second, nodeLeft, mid);
            pushDown(historyIndex, node->right, it->second, mid + 1, nodeRight);
            treesLazy[historyIndex].erase(it);
        }
        return mergeOp(querySub(historyIndex, node->left, left, right, nodeLeft, mid),
                       querySub(historyIndex, node->right, left, right, mid + 1, nodeRight));
    }

    T updateSub(int historyIndex, Node* node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return node ? node->value : defaultValue;

        if (nodeLeft == nodeRight)
            return node->value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(node->id);
        if (it != treesLazy[historyIndex].end()) {
            pushDown(historyIndex, node->left, it->second, nodeLeft, mid);
            pushDown(historyIndex, node->right, it->second, mid + 1, nodeRight);
            treesLazy[historyIndex].erase(it);
        }
        return p->value = mergeOp(updateSub(historyIndex, node->left, index, newValue, nodeLeft, mid),
                                  updateSub(historyIndex, node->right, index, newValue, mid + 1, nodeRight));
    }

    T updateRangeSub(int historyIndex, Node* node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return node ? node->value : defaultValue;

        if (nodeLeft == nodeRight)
            return node->value = newValue;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(node->id);
        if (it != treesLazy[historyIndex].end()) {
            pushDown(historyIndex, node->left, it->second, nodeLeft, mid);
            pushDown(historyIndex, node->right, it->second, mid + 1, nodeRight);
            treesLazy[historyIndex].erase(it);
        }

        if (left <= nodeLeft && nodeRight <= right) {
            treesLazy[historyIndex][node->id] = newValue;
            return node->value = blockOp(newValue, nodeRight - nodeLeft + 1);
        }

        return node->value = mergeOp(updateRangeSub(historyIndex, node->left, left, right, newValue, nodeLeft, mid),
                                     updateRangeSub(historyIndex, node->right, left, right, newValue, mid + 1, nodeRight));
    }

    pair<T,Node*> upgradeSub(int historyIndex, Node* node, int index, T newValue, int nodeLeft, int nodeRight) {
        if (index < nodeLeft || nodeRight < index)
            return make_pair(node ? node->value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, createNode(node->id, newValue, node->left, node->right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(node->id);
        if (it != treesLazy[historyIndex].end()) {
            auto L = pushDownUpgrade(historyIndex, node->left, it->second, nodeLeft, mid);
            auto R = pushDownUpgrade(historyIndex, node->right, it->second, mid + 1, nodeRight);
            node = createNode(node->id, node->value, L.second, R.second);
            treesLazy[historyIndex].erase(it);
        }

        auto L = upgradeSub(historyIndex, node->left, index, newValue, nodeLeft, mid);
        auto R = upgradeSub(historyIndex, node->right, index, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(p->id, value, p->left, p->right));
    }

    pair<T, Node*> upgradeRangeSub(int historyIndex, Node* node, int left, int right, T newValue, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return make_pair(node ? node->value : defaultValue, node);

        if (nodeLeft == nodeRight)
            return make_pair(newValue, createNode(node->id, newValue, node->left, node->right));

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;

        auto it = treesLazy[historyIndex].find(node->id);
        if (it != treesLazy[historyIndex].end()) {
            auto L = pushDownUpgrade(historyIndex, node->left, it->second, nodeLeft, mid);
            auto R = pushDownUpgrade(historyIndex, node->right, it->second, mid + 1, nodeRight);
            node = createNode(node->id, node->value, L.second, R.second);
            treesLazy[historyIndex].erase(it);
        }

        if (left <= nodeLeft && nodeRight <= right) {
            treesLazy[historyIndex][node->id] = newValue;
            T value = blockOp(newValue, nodeRight - nodeLeft + 1);
            return make_pair(value, createNode(node->id, value, node->left, node->right));
        }

        auto L = upgradeRangeSub(historyIndex, node->left, left, right, newValue, nodeLeft, mid);
        auto R = upgradeRangeSub(historyIndex, node->right, left, right, newValue, mid + 1, nodeRight);
        T value = mergeOp(L.first, R.first);
        return make_pair(value, createNode(node->id, value, L.second, R.second));
    }

    T pushDown(int historyIndex, Node* p, T newValue, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return p->value = newValue;

        treesLazy[historyIndex][p->id] = newValue;
        return p->value = blockOp(newValue, nodeRight - nodeLeft + 1);
    }

    pair<T, Node*> pushDownUpgrade(int historyIndex, Node* p, T newValue, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight)
            return make_pair(newValue, createNode(p->id, newValue, p->left, p->right));

        treesLazy[historyIndex][p->id] = newValue;
        T value = blockOp(newValue, nodeRight - nodeLeft + 1);
        return make_pair(value, createNode(p->id, value, p->left, p->right));
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePersistentSegmentTreeLazy(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePersistentSegmentTreeLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(v, mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline PersistentSegmentTreeLazy<T, MergeOp, BlockOp> makePersistentSegmentTreeLazy(const T arr[], int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return PersistentSegmentTreeLazy<T, MergeOp, BlockOp>(arr, size, mop, bop, dfltValue);
}
