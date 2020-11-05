#pragma once

template <typename T, typename MergeOp = function<T(T,T)>>
struct RBTreeRangeQuery {
    enum RBColor {
        rbcBlack,
        rbcRed
    };

    struct Node {
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;

        RBColor color;
        T       value;
        T       rangeValue;

        explicit Node(T x) {
            init(x);
        }

        void init(T x) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            cnt = 1;

            color = rbcBlack;
            value = x;
            rangeValue = x;
        }

        Node* next() const {
            const Node* node = this;
            if (node->right != nullptr)
                return node->right->minimum();
            else {
                Node *cur, *parent;

                cur = const_cast<Node*>(node);
                parent = node->parent;
                while ((parent != nullptr) && (cur == parent->right)) {
                    cur = parent;
                    parent = parent->parent;
                }
                return parent;
            }
        }

        Node* prev() const {
            const Node* node = this;
            if (node->left != nullptr)
                return node->left->maximum();
            else {
                Node *cur, *parent;

                cur = const_cast<Node*>(node);
                parent = node->parent;
                while ((parent != nullptr) && (cur == parent->left)) {
                    cur = parent;
                    parent = parent->parent;
                }
                return parent;
            }
        }

        Node* minimum() const {
            Node* p = const_cast<Node*>(this);
            while (p->left != nullptr)
                p = p->left;
            return p;
        }

        Node* maximum() const {
            Node* p = const_cast<Node*>(this);
            while (p->right != nullptr)
                p = p->right;
            return p;
        }


        bool operator ==(const Node& rhs) const {
            return value == rhs.value;
        }

        bool operator <(const Node& rhs) const {
            return value < rhs.value;
        }
    };

    Node*       root;           // root node
    int         count;

    MergeOp     mergeOp;
    T           defaultValue;

    //-----------------------------------------------------------

    explicit RBTreeRangeQuery(MergeOp op, T dflt = T())
        : root(nullptr), count(0), mergeOp(op), defaultValue(dflt) {
    }

    virtual ~RBTreeRangeQuery() {
        deleteRecursive(root);
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    const Node* begin() const {
        return root ? root->minimum() : nullptr;
    }

    Node* begin() {
        return root ? root->minimum() : nullptr;
    }

    // inclusive
    const Node* end() const {
        return root ? root->maximum() : nullptr;
    }

    // inclusive
    Node* end() {
        return root ? root->maximum() : nullptr;
    }

    bool exist(T value) const {
        return find(value) != nullptr;
    }

    // index >= 0
    Node* operator [](int index) const {
        if (index < 0 || index >= count)
            return nullptr;

        int n = index;
        Node* p = root;
        while (p != nullptr) {
            while (p->left != nullptr && p->left->cnt > n)
                p = p->left;
            if (p->left != nullptr)
                n -= p->left->cnt;
            if (!n--)
                break;
            p = p->right;
        }

        return p;
    }

    int indexOf(Node* p) const {
        if (p == nullptr)
            return -1;

        int res = (p->left != nullptr) ? p->left->cnt : 0;
        while (p->parent != nullptr) {
            while (p->parent != nullptr && p->parent->left == p)
                p = p->parent;
            if (p->parent != nullptr) {
                p = p->parent;
                res++;
                if (p->left != nullptr)
                    res += p->left->cnt;
            }
        }

        return res;
    }

    Node* find(T value) const {
        Node *p = root;

        while (p != nullptr && !(p->value == value)) {
            if (value < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    Node* lowerBound(T value) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (!(x->value < value)) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    Node* upperBound(T value) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (value < x->value) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }


    pair<Node*, bool> insert(T value) {
        pair<Node*, bool> ins = insertBST(value);
        if (!ins.second)
            return ins;

        Node* x = ins.first;
        Node* y;

        x->color = rbcRed;
        while ((x != root) && (x->parent && x->parent->color == rbcRed)) {
            if (x->parent == x->parent->parent->left) {
                y = x->parent->parent->right;

                if (y && y->color == rbcRed) {
                    x->parent->color = rbcBlack;
                    y->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    }

                    x->parent->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    rotateRight(x->parent->parent);
                }
            } else {
                y = x->parent->parent->left;

                if (y && y->color == rbcRed) {
                    x->parent->color = rbcBlack;
                    y->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }

                    x->parent->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    rotateLeft(x->parent->parent);
                }
            }
        }
        root->color = rbcBlack;

        return ins;
    }

    bool erase(Node* node) {
        // "y" is a deleting node
        // "x" is child with which to replace y
        Node* z = node;
        if (z == nullptr)
            return false;

        Node* y = ((z->left == nullptr) || (z->right == nullptr)) ? z : z->next();
        Node* x = (y->left != nullptr) ? y->left : y->right;    // find child with which to replace y

        // splice child onto parent
        if (x != nullptr)
            x->parent = y->parent;

        if (y->parent == nullptr) {
            // if deleting node is root, then replace root with x
            root = x;
        } else {
            // splice in child node
            if (y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;
        }

        // if needed, save y data
        if (y != z)
            swap(z->value, y->value);

        updateNodeToRoot(y->parent);

        // adjust tree under red-black rules
        if (y != nullptr && y->color == rbcBlack)
            deleteFixup(x);
        destroyNode(y);

        return true;
    }

    bool erase(T value) {
        return erase(find(value));
    }

    void clear() {
        deleteRecursive(root);
        root = nullptr;
    }

    //--- query

    // query in range [0, index]
    T queryPrefix(int index) const {
        return queryPrefix(root, index + 1);
    }

    // query in range [index, N - 1]
    T querySuffix(int index) const {
        return querySuffix(root, count - index);
    }

    // query in range [lefIndex, rightIndex], 0 <= leftIndex, rightIndex < N
    T query(int leftIndex, int rightIndex) const {
        if (leftIndex <= 0 && count <= rightIndex + 1 && root != nullptr)
            return root->rangeValue;

        int nL = leftIndex + 1, nR = rightIndex + 1;

        Node* lca = root;
        for (Node* p = root; p != nullptr && nL > 0 && nR > 0; p = p->right) {
            while (p->left != nullptr && p->left->cnt >= nR)
                p = p->left;
            lca = p;

            int leftSize = (p->left ? p->left->cnt : 0) + 1;
            if (nL <= leftSize)
                break;
            nL -= leftSize;
            nR -= leftSize;
        }

        T res = lca->value;
        if (left == right)
            return res;

        if (lca->left != nullptr)
            res = mergeOp(res, querySuffix(lca->left, (lca->cnt - nL) - (lca->right ? lca->right->cnt : 0)));

        if (lca->right != nullptr)
            res = mergeOp(res, queryPrefix(lca->right, nR - (lca->left ? lca->left->cnt : 0) - 1));

        return res;
    }


    T queryPrefixWithValue(T value) const {
        return queryLessThanEqual(root, value);
    }

    T querySuffixWithValue(T value) const {
        return queryGreaterThanEqual(root, value);
    }

    // query in range [low value, high value]
    T queryWithValue(int leftValue, int rightValue) const {
        Node* lca = root;
        while (lca != nullptr) {
            if (rightValue < lca->value)
                lca = lca->left;
            else if (leftValue > lca->value)
                lca = lca->right;
            else
                break;
        }
        if (lca == nullptr)
            return defaultValue;

        T res = lca->value;
        if (lca->left != nullptr)
            res = mergeOp(res, queryGreaterThanEqual(lca->left, leftValue));

        if (lca->right != nullptr)
            res = mergeOp(res, queryLessThanEqual(lca->right, rightValue));

        return res;
    }

protected:
    Node* createNode(T x) {
        Node* p = new Node(x);
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        delete node;
        count--;
    }

    void deleteRecursive(Node* node) {
        if (node != nullptr) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
    }

    pair<Node*, bool> insertBST(T item) {
        Node* parent = nullptr;
        Node* x = root;

        while (x != nullptr) {
            parent = x;

            if (item < x->value)
                x = x->left;
            else
                x = x->right;
        }

        Node* newNode = createNode(item);
        newNode->parent = parent;

        if (parent == nullptr)
            root = newNode;
        else {
            if (newNode->value < parent->value)
                parent->left = newNode;
            else
                parent->right = newNode;
            updateNodeToRoot(parent);
        }

        return make_pair(newNode, true);
    }

    void rotateLeft(Node* node) {
        if (node == nullptr || node->right == nullptr)
            return;

        Node* y = node->right;

        // turn y's left subtree into node's right subtree
        node->right = y->left;
        if (y->left != nullptr)
            y->left->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == nullptr)
            root = y;
        else {
            if (node == node->parent->left)
                node->parent->left = y;
            else
                node->parent->right = y;
        }

        // put node on y's left
        y->left = node;
        node->parent = y;

        updateNode(node);
        updateNode(y);
    }

    void rotateRight(Node* node) {
        if (node == nullptr || node->left == nullptr)
            return;

        Node* y = node->left;

        // turn y's right subtree into node's left subtree
        node->left = y->right;
        if (y->right != nullptr)
            y->right->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == nullptr)
            root = y;
        else {
            if (node == node->parent->right)
                node->parent->right = y;
            else
                node->parent->left = y;
        }

        // put node on y's right
        y->right = node;
        node->parent = y;

        updateNode(node);
        updateNode(y);
    }

    void deleteFixup(Node* node) {
        if (node == nullptr)
            return;

        Node *w, *x = node;
        while ((x != root) && (x->color == rbcBlack)) {
            if (x == x->parent->left) {
                w = x->parent->right;
                if (!w)
                    break;

                if (w->color == rbcRed) {
                    w->color = rbcBlack;
                    x->parent->color = rbcRed;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (!w)
                    break;

                if ((!w->left || w->left->color == rbcBlack) && (!w->right || w->right->color == rbcBlack)) {
                    w->color = rbcRed;
                    x = x->parent;
                } else {
                    if (!w->right || w->right->color == rbcBlack) {
                        w->left->color = rbcBlack;
                        w->color = rbcRed;
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = rbcBlack;
                    w->right->color = rbcBlack;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                w = x->parent->left;
                if (!w)
                    break;

                if (w->color == rbcRed) {
                    w->color = rbcBlack;
                    x->parent->color = rbcRed;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (!w)
                    break;

                if ((!w->right || w->right->color == rbcBlack) && (!w->left || w->left->color == rbcBlack)) {
                    w->color = rbcRed;
                    x = x->parent;
                } else {
                    if (!w->left || w->left->color == rbcBlack) {
                        w->right->color = rbcBlack;
                        w->color = rbcRed;
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = rbcBlack;
                    w->left->color = rbcBlack;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }

        x->color = rbcBlack;
    }

    void updateNodeToRoot(Node* node) {
        // update count
        while (node != nullptr) {
            updateNode(node);
            node = node->parent;
        }
    }

    void updateNode(Node* node) {
        if (node != nullptr) {
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
            node->rangeValue = mergeOp(node->value,
                               mergeOp(node->left ? node->left->rangeValue : defaultValue,
                                       node->right ? node->right->rangeValue : defaultValue));
        }
    }

    //---

    T queryPrefix(Node* node, int cnt) const {
        if (node == nullptr || cnt <= 0)
            return defaultValue;
        else if (cnt >= node->cnt)
            return node->rangeValue;

        T res = defaultValue;

        Node* p = node;
        while (p != nullptr && cnt > 0) {
            while (p->left != nullptr && p->left->cnt >= cnt)
                p = p->left;
            if (p->cnt <= cnt) {
                res = mergeOp(res, p->rangeValue);
                break;
            }

            if (p->left != nullptr) {
                cnt -= p->left->cnt;
                res = mergeOp(res, p->left->rangeValue);
            }
            if (cnt > 0) {
                res = mergeOp(res, p->value);
                cnt--;
            }
            p = p->right;
        }

        return res;
    }

    T querySuffix(Node* node, int cnt) const {
        if (node == nullptr || cnt <= 0)
            return defaultValue;
        else if (cnt >= node->cnt)
            return node->rangeValue;

        T res = defaultValue;

        Node* p = node;
        while (p != nullptr && cnt > 0) {
            while (p->right != nullptr && p->right->cnt >= cnt)
                p = p->right;
            if (p->cnt <= cnt) {
                res = mergeOp(res, p->rangeValue);
                break;
            }

            if (p->right != nullptr) {
                cnt -= p->right->cnt;
                res = mergeOp(res, p->right->rangeValue);
            }
            if (cnt > 0) {
                res = mergeOp(res, p->value);
                cnt--;
            }
            p = p->left;
        }

        return res;
    }


    T queryLessThanEqual(Node* node, T x) const {
        if (node == nullptr)
            return defaultValue;

        T res = defaultValue;

        Node* p = node;
        while (p != nullptr) {
            if (p->value <= x) {
                res = mergeOp(res, p->value);
                if (p->left)
                    res = mergeOp(res, p->left->rangeValue);
                p = p->right;
            } else {
                p = p->left;
            }
        }

        return res;
    }

    T queryGreaterThanEqual(Node* node, T x) const {
        if (node == nullptr)
            return defaultValue;

        T res = defaultValue;

        Node* p = node;
        while (p != nullptr) {
            if (x <= p->value) {
                res = mergeOp(res, p->value);
                if (p->right)
                    res = mergeOp(res, p->right->rangeValue);
                p = p->left;
            } else {
                p = p->right;
            }
        }

        return res;
    }
};
