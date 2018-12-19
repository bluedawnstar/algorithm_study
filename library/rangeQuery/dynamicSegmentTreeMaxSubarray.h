#pragma once

// https://www.codechef.com/problems/CBFEAST
struct DynamicSegmentTreeMaxSubarray {
    enum FieldT {
        fInner,
        fPrefix,
        fSuffix,
    };

    struct Node {
        int totalSum;       // total sum of a range
        int maxPrefixSum;   // max prefix sum of a range
        int maxSuffixSum;   // max suffix sum of a range
        int maxSum;         // max sum of a range

        bool marked;
        Node* left;
        Node* right;

        Node() {
            memset(this, 0, sizeof(*this));
        };

        int get(FieldT type) const {
            switch (type) {
            case fInner: return maxSum;
            case fPrefix: return maxPrefixSum;
            case fSuffix: return maxSuffixSum;
            }
            return 0;
        }

        void clear() {
            marked = false;
            totalSum = maxPrefixSum = maxSuffixSum = maxSum = 0;
        }
    };

    Node* rootL;
    Node* rootR;

    int rangeMin;
    int rangeMax;

    DynamicSegmentTreeMaxSubarray(int rangeMin, int rangeMax)
        : rangeMin(rangeMin), rangeMax(rangeMax) {
        rootL = new Node();
        rootR = new Node();
    }

    void updateLeft(int left, int right, int value) {
        update(rootL, rangeMin, rangeMax, left, right, value);
    }

    void updateRight(int left, int right, int value) {
        update(rootR, rangeMin, rangeMax, left, right, value);
    }

    int query(int index) {
        int ans1 = max(query(rootL, rangeMin, rangeMax, index, fInner), query(rootR, rangeMin, rangeMax, index, fInner));
        int ans2 = max(0, query(rootL, rangeMin, rangeMax, index, fSuffix) + query(rootR, rangeMin, rangeMax, index, fSuffix));
        return max(ans1, ans2);
    }

private:
    void update(Node* node, int nodeLeft, int nodeRight, int left, int right, int value) {
        if (right < nodeLeft || nodeRight < left)
            return;

        if (left <= nodeLeft && nodeRight <= right) {
            modify(node, value);
        } else {
            push(node);

            int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
            if (!node->left)
                node->left = new Node();
            update(node->left, nodeLeft, mid, left, right, value);

            if (!node->right)
                node->right = new Node();
            update(node->right, mid + 1, nodeRight, left, right, value);
        }
    }

    int query(Node* node, int nodeLeft, int nodeRight, int index, FieldT type) {
        if (!node)
            return 0;

        if (nodeLeft == nodeRight)
            return node->get(type);

        push(node);

        int mid = (nodeLeft + nodeRight) / 2;
        if (index <= mid)
            return query(node->left, nodeLeft, mid, index, type);
        else
            return query(node->right, mid + 1, nodeRight, index, type);
    }

    // aux is a prefix of the target
    void modify(Node* target, Node* aux) {
        target->marked = true;

        target->maxSum = max(max(target->maxSum, aux->maxSum), aux->maxSuffixSum + target->maxPrefixSum);
        target->maxPrefixSum = max(aux->maxPrefixSum, aux->totalSum + target->maxPrefixSum);
        target->maxSuffixSum = max(target->maxSuffixSum, target->totalSum + aux->maxSuffixSum);
        target->totalSum = target->totalSum + aux->totalSum;
    }

    // value is a prefix of the target
    void modify(Node* target, int value) {
        target->marked = true;

        target->maxSum = max(max(target->maxSum, value), value + target->maxPrefixSum);
        target->maxPrefixSum = max(value, value + target->maxPrefixSum);
        target->maxSuffixSum = max(target->maxSuffixSum, target->totalSum + value);
        target->totalSum = target->totalSum + value;
    }

    void push(Node *node) {
        if (!node->marked)
            return;

        if (!node->left)
            node->left = new Node();
        modify(node->left, node);

        if (!node->right)
            node->right = new Node();
        modify(node->right, node);

        node->clear();
    }
};
