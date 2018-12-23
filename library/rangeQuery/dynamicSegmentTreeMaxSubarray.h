#pragma once

// https://www.codechef.com/problems/CBFEAST
template <typename T = int>
struct DynamicSegmentTreeMaxSubarray {
    enum FieldT {
        fInner,
        fPrefix,
        fSuffix,
    };

    struct Node {
        T totalSum;         // total sum of a range
        T maxPrefixSum;     // max prefix sum of a range
        T maxSuffixSum;     // max suffix sum of a range
        T maxSum;           // max sum of a range

        bool hasData;
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
            hasData = false;
            totalSum = 0;
            maxPrefixSum = 0;
            maxSuffixSum = 0;
            maxSum = 0;
        }
    };

    Node* rootL;
    Node* rootR;
    vector<Node*> nodes;

    int rangeMin;
    int rangeMax;

    DynamicSegmentTreeMaxSubarray(int rangeMin, int rangeMax)
        : rangeMin(rangeMin), rangeMax(rangeMax) {
        rootL = createNode();
        rootR = createNode();
    }

    ~DynamicSegmentTreeMaxSubarray() {
        for (auto it : nodes)
            delete it;
    }


    void addLeft(int left, int right, T value) {
        add(left, right, value, rootL, rangeMin, rangeMax);
    }

    void addRight(int left, int right, T value) {
        add(left, right, value, rootR, rangeMin, rangeMax);
    }

    T query(int index, T minResult = 0) {
        T ans1 = max(query(index, fInner, rootL, rangeMin, rangeMax), query(index, fInner, rootR, rangeMin, rangeMax));
        T ans2 = max(minResult, query(index, fSuffix, rootL, rangeMin, rangeMax) + query(index, fSuffix, rootR, rangeMin, rangeMax));
        return max(ans1, ans2);
    }

private:
    Node* createNode() {
        nodes.push_back(new Node());
        return nodes.back();
    }

    void add(int left, int right, T value, Node* node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return;

        if (left <= nodeLeft && nodeRight <= right) {
            modify(node, value);
        } else {
            push(node);

            T mid = nodeLeft + (nodeRight - nodeLeft) / 2;
            if (!node->left)
                node->left = createNode();
            add(left, right, value, node->left, nodeLeft, mid);

            if (!node->right)
                node->right = createNode();
            add(left, right, value, node->right, mid + 1, nodeRight);
        }
    }

    T query(int index, FieldT type, Node* node, int nodeLeft, int nodeRight) {
        if (!node)
            return 0;

        if (nodeLeft == nodeRight)
            return node->get(type);

        push(node);

        T mid = (nodeLeft + nodeRight) / 2;
        if (index <= mid)
            return query(index, type, node->left, nodeLeft, mid);
        else
            return query(index, type, node->right, mid + 1, nodeRight);
    }

    // aux is a prefix of the target
    void modify(Node* target, Node* aux) {
        target->hasData = true;

        target->maxSum = max(max(target->maxSum, aux->maxSum), aux->maxSuffixSum + target->maxPrefixSum);
        target->maxPrefixSum = max(aux->maxPrefixSum, aux->totalSum + target->maxPrefixSum);
        target->maxSuffixSum = max(target->maxSuffixSum, target->totalSum + aux->maxSuffixSum);
        target->totalSum = target->totalSum + aux->totalSum;
    }

    // value is a prefix of the target
    void modify(Node* target, T value) {
        target->hasData = true;

        target->maxSum = max(max(target->maxSum, value), value + target->maxPrefixSum);
        target->maxPrefixSum = max(value, value + target->maxPrefixSum);
        target->maxSuffixSum = max(target->maxSuffixSum, target->totalSum + value);
        target->totalSum = target->totalSum + value;
    }

    void push(Node *node) {
        if (!node->hasData)
            return;

        if (!node->left)
            node->left = createNode();
        modify(node->left, node);

        if (!node->right)
            node->right = createNode();
        modify(node->right, node);

        node->clear();
    }
};
