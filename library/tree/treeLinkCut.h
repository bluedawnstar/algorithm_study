#pragma once

typedef int T;

// ---
struct LinkCutTree {
    struct Node {
        Node*   pathParent;
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;    // sub-tree's size in a sub-tree
        T       value;

        void init() {
            pathParent = nullptr;
            parent = left = right = nullptr;
            cnt = 1;
        }
    };

    Node* root(Node* x) {
        access(x);
        while (x->left)
            x = x->left;
        splay(x);
        return x;
    }

    int depth(Node *x) {
        access(x);
        return x->cnt - 1;
    }


    Node* lca(Node* x, Node* y) {
        access(x);
        return access(y);
    }

    // link x to y
    void link(Node *x, Node *y) {
        assert(x->left == nullptr); // x is a root of a represented tree
        access(x);
        access(y);
        x->left = y;
        y->parent = x;
        update(x);
    }

    // a represented tree to two represented trees
    //   which are a x's left sub-tree and x's right sub-tree including x
    void cut(Node *x) {
        access(x);
        x->left->parent = nullptr;
        x->left = nullptr;
        update(x);
    }


    Node* access(Node* x) {
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
    void splay(Node* x) {
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

    void rotateRight(Node *x) {
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

    void rotateLeft(Node *x) {
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

    void updateCnt(Node *x) {
        x->cnt = 1;
        if (x->left)
            x->cnt += x->left->cnt;
        if (x->right)
            x->cnt += x->right->cnt;
    }

    void update(Node* x) {
        updateCnt(x);

        //TODO: customizing point
    }
};

/*
class LinkCut {
    Node *x;

public:
    LinkCut(int n) {
        x = new Node[n];
        for (int i = 0; i < n; i++) {
            x[i].label = i;
            update(&x[i]);
        }
    }

    virtual ~LinkCut() {
        delete[] x;
    }

    void link(int u, int v) {
        ::link(&x[u], &x[v]);
    }

    void cut(int u) {
        ::cut(&x[u]);
    }

    int root(int u) {
        return ::root(&x[u])->label;
    }

    int depth(int u) {
        return ::depth(&x[u]);
    }

    int lca(int u, int v) {
        return ::lca(&x[u], &x[v])->label;
    }
};
*/
