#pragma once

template <typename T, typename MergeOp, typename BlockOp>
struct LinkCutTreePathQuery {
    enum LazyT {
        lzNone,
        lzSet,
        lzAdd
    };

    struct Node {
        Node*   pathParent;
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;            // tree's size in a sub-tree

        bool    revert;         // 

        T       value;          // 
        T       rangeValue;     // 

        LazyT   lazy;           // 
        T       lazyValue;      // 
    };

    void initNode(Node* node, const T& dfltVal) {
        node->pathParent = nullptr;
        node->parent = node->left = node->right = nullptr;
        node->cnt = 1;
        node->revert = false;

        node->rangeValue = node->value = node->lazyValue = dfltVal;
        node->lazy = lzNone;
    }

    bool isRoot(Node* node) const {
        return !node->parent && !node->pathParent;
    }

    void pushDown(Node* node) {
        if (node->revert) {
            node->revert = false;

            swap(node->left, node->right);
            if (node->left)
                node->left->revert = !node->left->revert;
            if (node->right)
                node->right->revert = !node->right->revert;
        }

        if (node->lazy) {
            if (node->left) {
                node->left->lazy = node->lazy;
                if (node->lazy == lzSet) {
                    node->left->lazyValue = node->lazyValue;
                    node->left->value = node->lazyValue;
                    node->left->rangeValue = blockOp(node->lazyValue, node->left->cnt);
                } else {
                    node->left->lazyValue += node->lazyValue;
                    node->left->value += node->lazyValue;
                    node->left->rangeValue += blockOp(node->lazyValue, node->left->cnt);
                }
            }
            if (node->right) {
                node->right->lazy = node->lazy;
                if (node->lazy == lzSet) {
                    node->right->lazyValue = node->lazyValue;
                    node->right->value = node->lazyValue;
                    node->right->rangeValue = blockOp(node->lazyValue, node->right->cnt);
                } else {
                    node->right->lazyValue += node->lazyValue;
                    node->right->value += node->lazyValue;
                    node->right->rangeValue += blockOp(node->lazyValue, node->right->cnt);
                }
            }
            node->lazyValue = defaultValue;
            node->lazy = lzNone;
        }
    }

    void update(Node* node) {
        node->cnt = 1;
        if (node->left)
            node->cnt += node->left->cnt;
        if (node->right)
            node->cnt += node->right->cnt;

        node->rangeValue = node->value;
        if (node->left)
            node->rangeValue = mergeOp(node->rangeValue, node->left->rangeValue);
        if (node->right)
            node->rangeValue = mergeOp(node->rangeValue, node->right->rangeValue);
    }

    //------------------------------------------------------------

    T defaultValue;

    MergeOp mergeOp;
    BlockOp blockOp;

    explicit LinkCutTreePathQuery(const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    Node* parent(Node* x) {
        access(x);
        if (!x->left)
            return nullptr;

        pushDown(x);
        x = x->left;
        while (x->right) {
            pushDown(x);
            x = x->right;
        }
        access(x);
        return x;
    }

    Node* findRoot(Node* v) {
        access(v);
        while (v->left) {
            pushDown(v);
            v = v->left;
        }
        splay(v);
        return v;
    }

    // global depth of v (>= 0)
    int depth(Node* v) {
        access(v);
        return v->cnt - 1;
    }

    bool isConnected(Node* v, Node* u) {
        if (v == u)
            return true;
        access(v);
        access(u);
        return v->parent || v->pathParent;
    }

    //--- query

    //PRECONDITION: v is connected to u
    Node* lca(Node* v, Node* u) {
        access(v);
        return access(u);
    }

    // dist >= 0
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    Node* climb(Node* v, int dist) {
        access(v);
        while (v) {
            if (v->right) {
                if (v->right->cnt > dist) {
                    v = v->right;
                    continue;
                }
                dist -= v->right->cnt;
            }
            if (dist-- <= 0)
                break;
            v = v->left;
        }
        access(v);
        return v;
    }

    // kth >= 1
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    Node* climbKth(Node* v, int kth, const function<int(T)>& valueToCountF) {
        access(v);
        while (v) {
            int d;
            if (v->right) {
                d = valueToCountF(v->right->rangeValue);    // >= 0
                if (d >= kth) {
                    v = v->right;
                    continue;
                }
                kth -= d;
            }
            d = valueToCountF(v->value);                    // 0 or 1
            if (d >= kth)
                break;
            kth -= d;
            v = v->left;
        }
        access(v);
        return v;
    }

    T queryToRoot(Node* v) {
        access(v);
        return v->rangeValue;
    }

    T queryToAncestor(Node* v, Node* ancestor) {
        if (v == ancestor) {
            access(v);
            return v->value;
        }

        access(ancestor);
        access(v);
        //assert(ancestor->parent || ancestor->pathParent); // connected

        splay(ancestor);
        if (ancestor->right)
            ancestor->right->parent = nullptr;
        splay(v);
        v->parent = ancestor;

        T res = mergeOp(ancestor->value, v->value);
        if (v->left)
            res = mergeOp(v->left->rangeValue, res);

        return res;
    }

    T query(Node* v, Node* u) {
        if (v == u) {
            access(v);
            return v->value;
        }

        Node* lc = lca(v, u);
        if (lc == v)
            return queryToAncestor(u, lc);
        else if (lc == u)
            return queryToAncestor(v, lc);

        return mergeOp(queryToAncestor(v, lc), queryToAncestor(u, climb(u, depth(u) - depth(lc) - 1)));
    }

    //--- for accumulative operation
    // Use this functions when MergeOp is 'add' (supporting subtraction)

    T queryToAncestorAccumulative(Node* v, Node* ancestor) {
        return queryToRoot(v) - queryToRoot(ancestor) + ancestor->value;
    }

    T queryAccumulative(Node* v, Node* u) {
        Node* lc = lca(v, u);
        return queryToRoot(v) + queryToRoot(u) - queryToRoot(lc) * 2 + lc->value;
    }

    //--- update

    void update(Node* v, const T& value) {
        access(v);
        v->value = value;
        update(v);
    }

    void add(Node* v, const T& value) {
        access(v);
        v->value += value;
        update(v);
    }


    void updateRangeToAncestor(Node* v, Node* ancestor, const T& value) {
        if (v == ancestor) {
            update(v, value);
            return;
        }

        access(ancestor);
        access(v);
        //assert(ancestor->parent || ancestor->pathParent); // connected

        splay(ancestor);
        if (ancestor->right)
            ancestor->right->parent = nullptr;
        splay(v);
        v->parent = ancestor;

        if (v->left) {
            v->left->lazy = lzSet;
            v->left->lazyValue = value;
            v->left->value = value;
            v->left->rangeValue = blockOp(value, v->left->cnt);
        }
        v->value = value;
        update(v);

        ancestor->value = value;
        update(ancestor);
    }

    void updateRange(Node* v, Node* u, const T& value) {
        if (v == u) {
            update(v, value);
            return;
        }

        Node* lc = lca(v, u);
        if (lc == v)
            updateRangeToAncestor(u, lc, value);
        else if (lc == u)
            updateRangeToAncestor(v, lc, value);
        else {
            updateRangeToAncestor(v, lc, value);
            updateRangeToAncestor(u, climb(u, depth(u) - depth(lc) - 1), value);
        }
    }


    void addRangeToAncestor(Node* v, Node* ancestor, const T& value) {
        if (v == ancestor) {
            add(v, value);
            return;
        }

        access(ancestor);
        access(v);
        //assert(ancestor->parent || ancestor->pathParent); // connected

        splay(ancestor);
        if (ancestor->right)
            ancestor->right->parent = nullptr;
        splay(v);
        v->parent = ancestor;

        if (v->left) {
            v->left->lazy = lzAdd;
            v->left->lazyValue += value;
            v->left->value += value;
            v->left->rangeValue += blockOp(value, v->left->cnt);
        }

        v->value += value;
        update(v);

        ancestor->value += value;
        update(ancestor);
    }

    void addRange(Node* v, Node* u, const T& value) {
        if (v == u) {
            add(v, value);
            return;
        }

        Node* lc = lca(v, u);
        if (lc == v)
            addRangeToAncestor(u, lc, value);
        else if (lc == u)
            addRangeToAncestor(v, lc, value);
        else {
            addRangeToAncestor(v, lc, value);
            addRangeToAncestor(u, climb(u, depth(u) - depth(lc) - 1), value);
        }
    }

    //--- 

    // link v to u
    bool link(Node* v, Node* u) {
        if (v == u)
            return false;   // connected

        access(v);
        access(u);
        if (v->parent || v->pathParent)
            return false;   // connected

        v->left = u;
        u->parent = v;
        update(v);

        return true;
    }

    bool cut(Node* v, Node* u) {
        makeRoot(v);
        access(u);
        if (u->left != v || v->right)
            return false;   // v and u is not directly connected to each other

        if (u->left) {
            u->left->parent = nullptr;
            u->left = nullptr;
            update(u);
        }
        return true;
    }

    // a represented tree to two represented trees
    //   which are a x's left sub-tree and x's right sub-tree including x
    void cut(Node* x) {
        access(x);
        if (x->left) {
            x->left->parent = nullptr;
            x->left = nullptr;
            update(x);
        }
    }

    //--- internal functions

    // make x as the global root
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

    void makeRoot(Node* x) {
        access(x);
        x->revert = !x->revert;
    }

protected:
    void rotateRight(Node* x) {
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

    void rotateLeft(Node* x) {
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

    void splay(Node* x) {
        Node *y, *z;

        while (x->parent) {
            y = x->parent;
            if (!y->parent) {
                pushDown(y);
                pushDown(x);
                if (x == y->left)
                    rotateRight(x);
                else
                    rotateLeft(x);
            } else {
                z = y->parent;
                pushDown(z);
                pushDown(y);
                pushDown(x);
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
        pushDown(x);
        update(x);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
struct LinkCutTreePathQueryArray : public LinkCutTreePathQuery<T, MergeOp, BlockOp> {
    typedef LinkCutTreePathQuery<T, MergeOp, BlockOp> LinkCutTreeT;
    vector<typename LinkCutTreeT::Node> nodes;

    LinkCutTreePathQueryArray(int N, const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : LinkCutTreePathQuery<T, MergeOp, BlockOp>(mop, bop, dflt), nodes(N) {
        for (int i = 0; i < N; i++)
            LinkCutTreeT::initNode(&nodes[i], dflt);
    }

    int parent(int v) {
        return LinkCutTreeT::parent(&nodes[v]) - &nodes[0];
    }

    int findRoot(int v) {
        return LinkCutTreeT::findRoot(&nodes[v]) - &nodes[0];
    }

    int lca(int v, int u) {
        return LinkCutTreeT::lca(&nodes[v], &nodes[u]) - &nodes[0];
    }

    int depth(int v) {
        return LinkCutTreeT::depth(&nodes[v]);
    }

    bool isConnected(int v, int u) {
        return LinkCutTreeT::isConnected(&nodes[v], &nodes[u]);
    }

    //--- query

    // kth >= 1
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    int climbKth(int v, int kth, const function<int(T)>& valueToCountF) {
        return LinkCutTreeT::climbKth(&nodes[v], kth, valueToCountF) - &nodes[0];
    }

    T queryToRoot(int v) {
        return LinkCutTreeT::queryToRoot(&nodes[v]);
    }

    T queryToAncestor(int v, int ancestor) {
        return LinkCutTreeT::queryToAncestor(&nodes[v], &nodes[ancestor]);
    }

    T query(int v, int u) {
        return LinkCutTreeT::query(&nodes[v], &nodes[u]);
    }

    //--- for accumulative operation
    // Use this functions when MergeOp is 'add' (supporting subtraction)

    T queryToAncestorAccumulative(int v, int ancestor) {
        return LinkCutTreeT::queryToAncestorAccumulative(&nodes[v], &nodes[ancestor]);
    }

    T queryAccumulative(int v, int u) {
        return LinkCutTreeT::queryAccumulative(&nodes[v], &nodes[u]);
    }

    //--- update

    void update(int v, const T& value) {
        LinkCutTreeT::update(&nodes[v], value);
    }

    void add(int v, const T& value) {
        LinkCutTreeT::add(&nodes[v], value);
    }


    void updateRangeToAncestor(int v, int ancestor, const T& value) {
        LinkCutTreeT::updateRangeToAncestor(&nodes[v], &nodes[ancestor], value);
    }

    void updateRange(int v, int u, const T& value) {
        LinkCutTreeT::updateRange(&nodes[v], &nodes[u], value);
    }


    void addRangeToAncestor(int v, int ancestor, const T& value) {
        LinkCutTreeT::addRangeToAncestor(&nodes[v], &nodes[ancestor], value);
    }

    void addRange(int v, int u, const T& value) {
        LinkCutTreeT::addRange(&nodes[v], &nodes[u], value);
    }

    //---

    // link v to u
    bool link(int v, int u) {
        return LinkCutTreeT::link(&nodes[v], &nodes[u]);
    }

    void cut(int v, int u) {
        LinkCutTreeT::cut(&nodes[v], &nodes[u]);
    }

    void cut(int v) {
        LinkCutTreeT::cut(&nodes[v]);
    }

    //--- internal functions
    int access(int v) {
        return LinkCutTreeT::access(&nodes[v]) - &nodes[0];
    }

    void makeRoot(int v) {
        return LinkCutTreeT::makeRoot(&nodes[v]);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline LinkCutTreePathQuery<T, MergeOp, BlockOp> makeLinkCutTreePathQuery(MergeOp mop, BlockOp bop, T dfltValue) {
    return LinkCutTreePathQuery<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline LinkCutTreePathQueryArray<T, MergeOp, BlockOp> makeLinkCutTreePathQueryArray(int size, MergeOp mop, BlockOp bop, T dfltValue) {
    return LinkCutTreePathQueryArray<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
}
