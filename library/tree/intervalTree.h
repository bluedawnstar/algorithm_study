#pragma once

// Sorted by low
// 
// [!!] Is it really needed? I think set, map is enough.
// 
template <typename T = int>
struct IntervalTree {
    struct Interval {
        T       low, high;  // inclusive
        //DataT   data;
    };

    struct IntervalNode {
        Interval        interval;
        T               max;
        IntervalNode*   left;
        IntervalNode*   right;
    };

    int             count;
    IntervalNode*   tree;

    IntervalTree() : count(0), tree(nullptr) {
    }

    ~IntervalTree() {
        destroyAll(tree);
    }


    // inclusive
    IntervalNode* insert(T low, T high) {
        if (tree == nullptr)
            return tree = createNode(low, high);
        return insert(tree, low, high);
    }

    // inclusive
    void erase(T low, T high) {
        tree = eraseSub(tree, low, high);
    }

    // inclusive
    IntervalNode* searchOverlap(T low, T high) const {
        if (!tree)
            return nullptr;

        IntervalNode* curr = tree;
        
        while (curr) {
            if (isOverlap(curr->interval, low, high))
                return curr;

            if (curr->left != nullptr && low <= curr->left->max)
                curr = curr->left;
            else
                curr = curr->right;
        }

        return curr;
    }

    // inclusive
    vector<IntervalNode*> searchContained(T low, T high) const {
        vector<IntervalNode*> res;
        searchContained(tree, low, high, res);
        return res;
    }

    IntervalNode* getLeftmost() const {
        IntervalNode* curr = tree;
        while (curr->left)
            curr = curr->left;
        return curr;
    }

    IntervalNode* getLeftmost(IntervalNode* node) const {
        IntervalNode* curr = node;
        while (curr->left)
            curr = curr->left;
        return curr;
    }

    // inclusive
    static bool isOverlap(const Interval& lhs, const Interval& rhs) {
        return lhs.low <= rhs.high && rhs.low <= lhs.high;
    }

    // inclusive
    static bool isOverlap(const Interval& lhs, T low, T high) {
        return lhs.low <= high && low <= lhs.high;
    }

public:
    // inclusive
    IntervalNode* createNode(T low, T high) {
        auto* p = new IntervalNode;

        p->interval.low = low;
        p->interval.high = high;
        p->max = high;
        p->left = p->right = nullptr;
        count++;

        return p;
    }

    void destroyNode(IntervalNode* node) {
        delete node;
        count--;
    }

    void destroyAll(IntervalNode* node) {
        if (!node)
            return;
        if (node->left)
            destroyAll(node->left);
        if (node->right)
            destroyAll(node->right);
        destroyNode(node);
    }

    // inclusive
    void searchContained(IntervalNode* node, T low, T high, vector<IntervalNode*>& res) const {
        if (!node)
            return;

        if (node->left && low < node->interval.low)
            searchContained(node->left, low, high, res);

        if (low <= node->interval.low && node->interval.high <= high)
            res.push_back(node);

        if (node->right && low <= node->right->max)
            searchContained(node->right, low, high, res);
    }

    // inclusive
    IntervalNode* insert(IntervalNode* node, T low, T high) {
        IntervalNode* newNode = nullptr;
        
        if (low < node->interval.low) {
            if (!node->left)
                newNode = node->left = createNode(low, high);
            else
                newNode = insert(node->left, low, high);
        } else {
            if (!node->right)
                newNode = node->right = createNode(low, high);
            else
                newNode = insert(node->right, low, high);
        }

        if (node->max < high)
            node->max = high;

        return newNode;
    }

    // inclusive
    IntervalNode* eraseSub(IntervalNode* node, T low, T high) {
        if (!node)
            return nullptr;

        if (low < node->interval.low)
            node->left = eraseSub(node->left, low, high);
        else if (low > node->interval.low)
            node->right = eraseSub(node->right, low, high);
        else {
            if (high == node->interval.high) {
                if (!node->left) {
                    auto p = node->right;
                    destroyNode(node);
                    return p;
                } else if (!node->right) {
                    auto p = node->left;
                    destroyNode(node);
                    return p;
                }

                auto p = getLeftmost(node->right);
                node->interval = p->interval;
                node->right = eraseSub(node->right, p->interval.low, p->interval.high);
            } else {
                node->right = eraseSub(node->right, low, high);
            }
        }
        node->max = max(node->interval.high,
                    max(node->left ? node->left->max : numeric_limits<T>::min(),
                        node->right ? node->right->max : numeric_limits<T>::min()));
        return node;
    }
};
