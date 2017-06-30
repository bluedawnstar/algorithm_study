#pragma once

template <typename T>
struct PathQuerySetAndSumOpT {
    static const T defaultValue = 0;

    static void updateValue(T& value, const T& newValue) {
        value = newValue;
    }

    static T mergeValue(const T& a, const T& b) {
        return a + b;
    }

    static void updateRange(T& value, const T& newValue, int newN) {
        value = newValue * newN;
    }
};

template <typename T>
struct PathQueryAddAndSumOpT {
    static const T defaultValue = 0;

    static void updateValue(T& value, const T& newValue) {
        value += newValue;
    }

    static T mergeValue(const T& a, const T& b) {
        return a + b;
    }

    static void updateRange(T& value, const T& newValue, int newN) {
        value += newValue * newN;
    }
};


template <typename T, typename Op>
struct LinkCutTreePathQuery {
    struct Node {
        Node*   pathParent;
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;            // tree's size in a sub-tree

        T       value;          // 
        T       rangeValue;     // 

        bool    lazyExist;      // 
        T       lazy;           // 

        bool    revert;

        Node() {
            init();
        }

        Node(const T& val) : value(val), rangeValue(val) {
            init();
        }

        void init() {
            pathParent = nullptr;
            parent = left = right = nullptr;
            cnt = 1;

            rangeValue = value = lazy = typename Op::defaultValue;

            revert = false;
            lazy = false;
        }

        bool isRoot() const {
            return !parent || !pathParent;
        }

        void pushDown() {
            if (revert) {
                revert = false;

                swap(left, right);
                if (left)
                    left->revert = !left->revert;
                if (right)
                    right->revert = !right->revert;
            }

            if (!lazyExist)
                return;

            if (left) {
                left->lazyExist = true;
                typename Op::updateValue(left->lazy, lazy);
                typename Op::updateValue(left->value, lazy);
                typename Op::updateRange(left->rangeValue, lazy, left->cnt);
            }
            if (right) {
                right->lazyExist = true;
                typename Op::updateValue(right->lazy, lazy);
                typename Op::updateValue(right->value, lazy);
                typename Op::updateRange(right->rangeValue, lazy, right->cnt);
            }
            lazy = typename Op::defaultValue;
            lazyExist = false;
        }

        void update() {
            cnt = 1;
            if (left)
                cnt += left->cnt;
            if (right)
                cnt += right->cnt;

            rangeValue = value;
            if (left)
                rangeValue = typename Op::mergeValue(rangeValue, left->rangeValue);
            if (right)
                rangeValue = typename Op::mergeValue(rangeValue, right->rangeValue);
        }
    };

    static T query(Node* u, Node* v) {
        makeRoot(u);
        access(v);
        return v->rangeValue;
    }

    static void update(Node* u, Node* v, const T& value) {
        makeRoot(u);
        access(v);

        v->lazyExist = true;
        typename Op::updateValue(v->lazy, value);
        typename Op::updateValue(v->value, value);
        typename Op::updateRange(v->rangeValue, value, v->cnt);
    }


    static Node* root(Node* x) {
        access(x);
        while (x->left)
            x = x->left;
        splay(x);
        return x;
    }

    //PRECONDITION: x is connected to y
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
        return x->parent || x->pathParent;
    }

    static void makeRoot(Node* x) {
        access(x);
        x->revert = !x->revert;
    }

    // link x to y
    static bool link(Node* x, Node* y) {
        if (x == y)
            return false;   // connected
        access(x);
        access(y);
        if (x->parent || x->pathParent)
            return false;   // connected

        x->left = y;
        y->parent = x;
        update(x);

        return true;
    }

    static bool cut(Node* x, Node* y) {
        makeRoot(x);
        access(y);
        if (y->left != x || x->right)
            return false;   // x and y is not directly connected to each other

        if (y->left) {
            y->left->parent = nullptr;
            y->left = nullptr;
            update(y);
        }
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
                y->pushDown();
                x->pushDown();
                if (x == y->left)
                    rotateRight(x);
                else
                    rotateLeft(x);
            } else {
                z = y->parent;
                z->pushDown();
                y->pushDown();
                x->pushDown();
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
        x->pushDown();
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

    static void update(Node* x) {
        if (x)
            x->update();
    }
};

template <typename T, typename Op>
struct LinkCutTreeArrayPathQuery {
    typedef LinkCutTreePathQuery<T, Op> LinkCutTreeT;
    vector<typename LinkCutTreeT::Node> nodes;

    LinkCutTreeArrayPathQuery(int N) : nodes(N) {
        // no action
    }

    void setValue(int u, const T& value) {
        nodes[u].value = value;
    }

    const T& getValue(int u) const {
        return nodes[u].value;
    }


    T query(int u, int v) {
        return LinkCutTreeT::query(&nodes[u], &nodes[v]);
    }

    void update(int u, int v, const T& value) {
        LinkCutTreeT::update(&nodes[u], &nodes[v], value);
    }


    int root(int u) {
        return LinkCutTreeT::root(&nodes[u]) - &nodes[0];
    }

    int lca(int u, int v) {
        return LinkCutTreeT::lca(&nodes[u], &nodes[v]) - &nodes[0];
    }

    int depth(int u) {
        return LinkCutTreeT::depth(&nodes[u]);
    }

    bool isConnected(int u, int v) {
        return LinkCutTreeT::isConnected(&nodes[u], &nodes[v]);
    }


    void makeRoot(int u) {
        LinkCutTreeT::makeRoot(&nodes[u]);
    }

    // link x to y
    bool link(int u, int v) {
        return LinkCutTreeT::link(&nodes[u], &nodes[v]);
    }

    void cut(int u, int v) {
        LinkCutTreeT::cut(&nodes[u], &nodes[v]);
    }

    void cut(int u) {
        LinkCutTreeT::cut(&nodes[u]);
    }

    int access(int u) {
        return LinkCutTreeT::access(&nodes[u]);
    }
};
