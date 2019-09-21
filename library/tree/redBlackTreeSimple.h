#pragma once

// T has to have below functions
//    1) operator ==()
//    2) operator <()

template <typename T>
struct RBTreeSimple {
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

        Node(const T& item) {
            init(item);
        }

        void init(const T& item) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            cnt = 1;

            color = rbcBlack;
            value = item;
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

    Node*       root;     // root node
    bool        duplicatable;
    int         count;

    //-----------------------------------------------------------

    RBTreeSimple() {
        root = nullptr;
        duplicatable = false;
        count = 0;
    }

    RBTreeSimple(const RBTreeSimple<T>& tree) {
        root = nullptr;
        duplicatable = false;
        count = 0;
        copyRecursive(tree.root);
    }

    virtual ~RBTreeSimple() {
        deleteRecursive(root);
    }

    RBTreeSimple<T>& operator =(const RBTreeSimple<T>& tree) {
        if (this != &tree) {
            deleteRecursive(root);
            duplicatable = tree.duplicatable;
            root = nullptr;
            copyRecursive(tree.root);
        }
        return *this;
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    bool isDuplicatable() const {
        return duplicatable;
    }

    void setDuplicatable(bool enable) {
        duplicatable = enable;
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

    bool exist(const T& key) const {
        return find(key) != nullptr;
    }

    Node* operator [](int index) const {
        assert((root != nullptr ? root->cnt : 0) == count);

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
        assert((root != nullptr ? root->cnt : 0) == count);

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

    Node* find(const T& key) const {
        Node *p = root;

        while (p != nullptr && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    Node* lowerBound(const T& key) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (!(x->value < key)) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    Node* upperBound(const T& key) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (key < x->value) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }


    pair<Node*, bool> insert(const T& item) {
        pair<Node*, bool> ins = insertBST(item);
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

        Node *y, *x;

        if ((z->left == nullptr) || (z->right == nullptr))
            y = z;
        else
            y = z->next();

        // find child with which to replace y
        if (y->left != nullptr)
            x = y->left;
        else
            x = y->right;

        // splice child onto parent
        if (x != nullptr)
            x->parent = y->parent;

        if (y->parent == nullptr)
            // if deleting node is root, then replace root with x
            root = x;
        else {
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

    bool erase(const T& item) {
        return erase(find(item));
    }

    void clear() {
        deleteRecursive(root);
        root = nullptr;
    }

protected:
    Node* createNode(const T& item) {
        Node* p = new Node(item);
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        assert(node != nullptr);
        delete node;
        count--;
    }

    void copyRecursive(const Node* node) {
        if (node != nullptr) {
            insert(node->value);
            copyRecursive(node->left);
            copyRecursive(node->right);
        }
    }

    void deleteRecursive(Node* node) {
        if (node != nullptr) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
    }

    pair<Node*, bool> insertBST(const T& item) {
        Node* parent = nullptr;
        Node* x = root;

        while (x != nullptr) {
            parent = x;

            if (item == x->value && !duplicatable) {
                return make_pair(x, false);
            } else {
                if (item < x->value)
                    x = x->left;
                else
                    x = x->right;
            }
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

    void updateNodeCnt(Node* node) {
        if (node != nullptr)
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
    }

    void updateNode(Node* node) {
        updateNodeCnt(node);

        //TODO: add custom actions
    }
};
