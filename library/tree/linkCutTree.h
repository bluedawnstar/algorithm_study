#pragma once

template <typename T>
struct LinkCutTree {
    struct Node {
        Node*   pathParent;
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;    // tree's size in a sub-tree
        T       value;

        Node() {
            init();
        }

        Node(const T& val) : value(val) {
            init();
        }

        void init() {
            pathParent = nullptr;
            parent = left = right = nullptr;
            cnt = 1;
        }
    };

    static Node* root(Node* x) {
        access(x);
        while (x->left)
            x = x->left;
        splay(x);
        return x;
    }

    static Node* lca(Node* x, Node* y) {
        access(x);
        return access(y);
    }

    // global depth of x
    static int depth(Node* x) {
        access(x);
        return x->cnt - 1;
    }

    static bool isConnected(Node* x, Node* y) {
        if (x == y)
            return true;
        access(x);
        access(y);
        return x->parent != nullptr || x->pathParent != nullptr;
    }


    // link x to y
    static bool link(Node* x, Node* y) {
        if (x == y)
            return false;   // connected
        access(x);
        access(y);
        if (x->parent != nullptr || x->pathParent != nullptr)
            return false;   // connected

        x->left = y;
        y->parent = x;
        update(x);

        return true;
    }

    // a represented tree to two represented trees
    //   which are a x's left sub-tree and x's right sub-tree including x
    static void cut(Node* x) {
        access(x);
        if (x->left) {
            x->left->parent = nullptr;
            x->left = nullptr;
            update(x);
        }
    }


    // make x as the global root
    static Node* access(Node* x) {
        splay(x);
        if (x->right) {
            x->right->pathParent = x;
            x->right->parent = nullptr;
            x->right = nullptr;
            update(x);
        }

        Node* last = x;
        while (x->pathParent) {
            Node* y = x->pathParent;
            last = y;
            splay(y);
            if (y->right) {
                y->right->pathParent = y;
                y->right->parent = nullptr;
            }
            y->right = x;
            x->parent = y;
            x->pathParent = nullptr;
            update(y);
            splay(x);
        }
        assert(x->right == nullptr);
        return last;
    }

protected:
    static void splay(Node* x) {
        Node *y, *z;

        while (x->parent) {
            y = x->parent;
            if (!y->parent) {
                if (x == y->left)
                    rotateRight(x);
                else
                    rotateLeft(x);
            } else {
                z = y->parent;
                if (y == z->left) {
                    if (x == y->left)
                        rotateRight(y), rotateRight(x);
                    else
                        rotateLeft(x), rotateRight(x);
                } else {
                    if (x == y->right)
                        rotateLeft(y), rotateLeft(x);
                    else
                        rotateRight(x), rotateLeft(x);
                }
            }
        }
        update(x);
    }

    static void rotateRight(Node* x) {
        Node* y = x->parent;
        Node* z = y->parent;
        y->left = x->right;
        if (x->right)
            x->right->parent = y;
        x->right = y;
        y->parent = x;
        x->parent = z;
        if (z) {
            if (y == z->left)
                z->left = x;
            else
                z->right = x;
        }
        x->pathParent = y->pathParent;
        y->pathParent = nullptr;
        update(y);
    }

    static void rotateLeft(Node* x) {
        Node* y = x->parent;
        Node* z = y->parent;
        y->right = x->left;
        if (x->left)
            x->left->parent = y;
        x->left = y;
        y->parent = x;
        x->parent = z;
        if (z) {
            if (y == z->left)
                z->left = x;
            else
                z->right = x;
        }
        x->pathParent = y->pathParent;
        y->pathParent = nullptr;
        update(y);
    }

    // in a aux tree
    static void updateCnt(Node* x) {
        x->cnt = 1;
        if (x->left)
            x->cnt += x->left->cnt;
        if (x->right)
            x->cnt += x->right->cnt;
    }

    static void update(Node* x) {
        updateCnt(x);

        //TODO: customizing point
    }
};

template <typename T>
struct LinkCutTreeArray {
    vector<typename LinkCutTree<T>::Node> nodes;

    LinkCutTreeArray(int N) : nodes(N) {
        // no action
    }

    void setValue(int u, const T& value) {
        nodes[u].value = value;
    }

    const T& getValue(int u) const {
        return nodes[u].value;
    }

    int root(int u) {
        return LinkCutTree<T>::root(&nodes[u]) - &nodes[0];
    }

    int lca(int u, int v) {
        return LinkCutTree<T>::lca(&nodes[u], &nodes[v]) - &nodes[0];
    }

    int depth(int u) {
        return LinkCutTree<T>::depth(&nodes[u]);
    }

    bool isConnected(int u, int v) {
        return LinkCutTree<T>::isConnected(&nodes[u], &nodes[v]);
    }


    // link x to y
    bool link(int x, int y) {
        return LinkCutTree<T>::link(&nodes[x], &nodes[y]);
    }

    void cut(int x) {
        LinkCutTree<T>::cut(&nodes[x]);
    }

    int access(int x) {
        return LinkCutTree<T>::access(&nodes[x]);
    }
};
