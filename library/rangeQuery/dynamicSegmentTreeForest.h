#pragma once

template <typename T, typename MergeOp = function<T(T, T)>>
struct DynamicSegmentTreeForest {
    struct Node {
        int L;
        int R;
        T   value;
    };

    int N;
    vector<Node> nodes;

    MergeOp mergeOp;
    T defaultValue;

    explicit DynamicSegmentTreeForest(MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
    }

    explicit DynamicSegmentTreeForest(int n, MergeOp op, T dflt = T())
        : N(n), mergeOp(op), defaultValue(dflt) {
        nodes.reserve(N * 4);
    }


    void init(int n) {
        N = n;
        nodes.clear();
        nodes.reserve(N * 4);
    }

    //---

    // if root is -1 then empty tree
    int update(int root, int index, T value) {
        return update(root, 0, N - 1, index, value);
    }

    // if root is -1 then empty tree
    T query(int root, int left, int right) const {
        return query(root, 0, N - 1, left, right);
    }

    T get(int node) const {
        if (node < 0)
            return defaultValue;
        return nodes[node].value;
    }

    //---

    int merge(int node1, int node2) {
        if (node1 < 0)
            return node2;
        else if (node2 < 0)
            return node1;

        nodes[node1].value = mergeOp(nodes[node1].value, nodes[node2].value);
        nodes[node1].L = merge(nodes[node1].L, nodes[node2].L);
        nodes[node1].R = merge(nodes[node1].R, nodes[node2].R);

        return node1;
    }

private:
    int update(int node, int left, int right, int index, T value) {
        if (node < 0) {
            node = int(nodes.size());
            nodes.push_back({ -1, -1, defaultValue });
        }
        
        if (left == right) {
            nodes[node].value = value;
            return node;
        }

        int L = nodes[node].L;
        int R = nodes[node].R;

        int mid = (left + right) >> 1;
        if (index <= mid) {
            L = update(L, left, mid, index, value);
            nodes[node].L = L;
        } else {
            R = update(nodes[node].R, mid + 1, right, index, value);
            nodes[node].R = R;
        }

        nodes[node].value = mergeOp(L < 0 ? defaultValue : nodes[L].value,
                                    R < 0 ? defaultValue : nodes[R].value);

        return node;
    }

    T query(int node, int L, int R, int left, int right) const {
        if (L > right || R < left || node < 0)
            return defaultValue;
        else if (left <= L && R <= right)
            return nodes[node].value;

        int mid = (L + R) >> 1;
        return mergeOp(query(nodes[node].L, L, mid, left, right),
                       query(nodes[node].R, mid + 1, R, left, right));
    }
};
