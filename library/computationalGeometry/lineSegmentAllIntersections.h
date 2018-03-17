#pragma once

#include "vec2D.h"

// Line Sweep Algorithm
namespace LineSegmentAllIntersections {
    bool lineIntersection(Vec2D<double> a, Vec2D<double> b, Vec2D<double> c, Vec2D<double> d, Vec2D<double>& out) {
        double det = (b - a).cross(d - c);
        if (isZero(det))
            return false;
        double det2 = (c - a).cross(d - c) / (double)det;
        out.x = a.x + (b.x - a.x) * det2;
        out.y = a.y + (b.y - a.y) * det2;
        return true;
    }

    bool parallelSegments(Vec2D<double> a, Vec2D<double> b, Vec2D<double> c, Vec2D<double> d, Vec2D<double>& out) {
        if (b < a)
            swap(a, b);
        if (d < c)
            swap(c, d);

        if (!(b - a).isCollinear(c - a) || (b < c) || (d < a))
            return false;

        if (a < c) {
            out.x = c.x;
            out.y = c.y;
        } else {
            out.x = a.x;
            out.y = a.y;
        }

        return true;
    }

    bool inBoundingRectangle(Vec2D<double> p, Vec2D<double> a, Vec2D<double> b) {
        if (b < a)
            swap(a, b);
        return (p == a) || (p == b) || (a < p && p < b);
    }

    bool segmentIntersection(Vec2D<double> a, Vec2D<double> b, Vec2D<double> c, Vec2D<double> d, Vec2D<double>& out) {
        if (!lineIntersection(a, b, c, d, out))
            return parallelSegments(a, b, c, d, out);
        return inBoundingRectangle(out, Vec2D<double>(a), Vec2D<double>(b)) &&
               inBoundingRectangle(out, Vec2D<double>(c), Vec2D<double>(d));
    }

    long long makeUID(int id1, int id2) {
        return ((long long)min(id1, id2) << 32) + max(id1, id2);
    }

    struct LineSegment {
        Vec2D<double> a, b;
        int id;

        LineSegment() {
        }

        LineSegment(double ax, double ay, double bx, double by, int _id = 0) : a{ ax, ay }, b{ bx, by }, id(_id) {
            if (a > b)
                swap(a, b);
        }

        LineSegment(Vec2D<double> _a, Vec2D<double> _b, int _id = 0) : a(_a), b(_b), id(_id) {
            if (a > b)
                swap(a, b);
        }

        bool operator ==(const LineSegment& rhs) const {
            return id == rhs.id;
        }

        bool isLess(const LineSegment& rhs, double x) const {
            double y0 = getY(x);
            double y1 = rhs.getY(x);
            if (fabs(y1 - y0) > EPSILON)
            //if (y0 != y1)
                return y0 < y1;
            else
                return (b.y - a.y) * (rhs.b.x - rhs.a.x) < (rhs.b.y - rhs.a.y) * (b.x - a.x);
        }

        double getY(double x) const {
            return (b.y - a.y) * (x - a.x) / (b.x - a.x) + a.y;
        }
    };

    // Treap
    struct SearchTree {
        struct Node {
            Node* parent;
            Node* left;
            Node* right;

            int   priority;

            LineSegment value;

            void init() {
                parent = left = right = nullptr;
                priority = (rand() & 0x7fff) * (rand() & 0x7fff);
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
        };

        int     count;
        Node*   tree;
        double  currX;

        SearchTree() {
            count = 0;
            tree = nullptr;
        }

        ~SearchTree() {
            deleteRecursive(tree);
        }

        int size() const {
            return count;
        }

        Node* insert(const LineSegment& value) {
            Node* p = createNode(value);
            tree = insert(tree, p);
            tree->parent = nullptr;
            return p;
        }

        bool erase(const LineSegment& key) {
            tree = erase(tree, key);
            if (tree)
                tree->parent = nullptr;
            return true;
        }

        void erase(Node* it) {
            if (!it)
                return;

            Node* parent = it->parent;
            Node* node = merge(it->left, it->right);
            destroyNode(it);

            if (parent != nullptr) {
                if (parent->left == it)
                    parent->left = node;
                else if (parent->right == it)
                    parent->right = node;
            } else {
                tree = node;
            }
            if (node)
                node->parent = parent;
        }

        Node* lowerBound(double px, double py) const {
            if (tree == nullptr)
                return nullptr;

            Node* y = nullptr;
            Node* x = tree;
            while (x != nullptr) {
                if (!(x->value.getY(px) < py)) {
                    y = x;
                    x = x->left;
                } else
                    x = x->right;
            }
            return y;
        }

        Node* upperBound(double px, double py) const {
            if (tree == nullptr)
                return nullptr;

            Node* y = nullptr;
            Node* x = tree;
            while (x != nullptr) {
                if (py < x->value.getY(px)) {
                    y = x;
                    x = x->left;
                } else
                    x = x->right;
            }
            return y;
        }

    protected:
        Node* createNode(const LineSegment& item) {
            Node* p = new Node();
            p->init();
            p->value = item;
            count++;
            return p;
        }

        void destroyNode(Node* node) {
            count--;
            delete node;
        }

        void deleteRecursive(Node* node) {
            if (node != nullptr) {
                deleteRecursive(node->left);
                deleteRecursive(node->right);
                destroyNode(node);
            }
        }

        pair<Node*, Node*> split(Node* root, const LineSegment& key) {
            if (root == nullptr)
                return make_pair(nullptr, nullptr);

            if (root->value.isLess(key, currX)) {
                auto rs = split(root->right, key);
                root->right = rs.first;
                if (rs.first)
                    rs.first->parent = root;
                return make_pair(root, rs.second);
            } else {
                auto ls = split(root->left, key);
                root->left = ls.second;
                if (ls.second)
                    ls.second->parent = root;
                return make_pair(ls.first, root);
            }
        }

        Node* insert(Node* root, Node* node) {
            if (root == nullptr)
                return node;

            if (root->priority < node->priority) {
                auto splitted = split(root, node->value);
                node->left = splitted.first;
                node->right = splitted.second;
                if (splitted.first)
                    splitted.first->parent = node;
                if (splitted.second)
                    splitted.second->parent = node;
                return node;
            }

            if (node->value.isLess(root->value, currX)) {
                root->left = insert(root->left, node);
                if (root->left)
                    root->left->parent = root;
            } else {
                root->right = insert(root->right, node);
                if (root->right)
                    root->right->parent = root;
            }
            return root;
        }


        Node* merge(Node* a, Node* b) {
            if (!a)
                return b;
            if (!b)
                return a;

            if (a->priority < b->priority) {
                b->left = merge(a, b->left);
                if (b->left)
                    b->left->parent = b;
                return b;
            } else {
                a->right = merge(a->right, b);
                if (a->right)
                    a->right->parent = a;
                return a;
            }
        }

        Node* erase(Node* root, const LineSegment& key) {
            if (!root)
                return root;

            if (root->value == key) {
                Node* res = merge(root->left, root->right);
                destroyNode(root);
                return res;
            }

            if (key.isLess(root->value, currX)) {
                root->left = erase(root->left, key);
                if (root->left)
                    root->left->parent = root;
            } else {
                root->right = erase(root->right, key);
                if (root->right)
                    root->right->parent = root;
            }
            return root;
        }
    };

    struct Event {
        Vec2D<double> p;
        int type;

        int id;
        int id2;

        Event(Vec2D<double> _p, int _id, int _id2, int _type) : p(_p), id(_id), id2(_id2), type(_type) {
        }

        bool operator ==(const Event& rhs) const {
            return p == rhs.p && type == rhs.type;
        }

        bool operator <(const Event& rhs) const {
            if (p.x != rhs.p.x)
                return p.x < rhs.p.x;
            else if (type != rhs.type)
                return type < rhs.type;
            else
                return p.y < rhs.p.y;
        }

        bool operator >(const Event& rhs) const {
            return !operator <(rhs) && !operator ==(rhs);
        }
    };

    struct DuplicateCheck {
        unordered_set<long long> S;

        bool operator()(int id1, int id2) {
            long long key = ((long long)min(id1, id2) << 32) + max(id1, id2);
            if (S.find(key) != S.end())
                return true;
            S.insert(key);
            return false;
        }
    };

    // Bentley-Ottman : O((N + k)logN)
    // <Precondition>
    //   1. No two line segment endpoints or crossings have the same x-coordinate
    //   2. No line segment endpoint lies upon another line segment
    //   3. No three line segments intersect at a single point
    //
    // returns (line 1 id, line 2 id, intersection point)
    vector<tuple<int, int, Vec2D<double>>> findAllIntersections(vector<LineSegment>& v, double epsilon) {
        priority_queue<Event, vector<Event>, greater<Event>> Q;
        for (int i = 0; i < (int)v.size(); i++) {
            Q.emplace(v[i].a, i, -1, 0);
            Q.emplace(v[i].b, i, -1, 2);
            v[i].id = i;
        }

        vector<tuple<int, int, Vec2D<double>>> res;

        DuplicateCheck check;
        vector<SearchTree::Node*> nodes(v.size());
        SearchTree tree;
        while (!Q.empty()) {
            auto e = Q.top();
            Q.pop();

            tree.currX = e.p.x;
            if (e.type == 0) {
                auto it = tree.insert(v[e.id]);
                nodes[e.id] = it;

                Vec2D<double> pt;

                auto* below = it->prev();
                if (below) {
                    if (!check(below->value.id, e.id)
                        && segmentIntersection(below->value.a, below->value.b, v[e.id].a, v[e.id].b, pt)) {
                        Q.emplace(pt, below->value.id, e.id, 1);
                        res.emplace_back(below->value.id, e.id, pt);
                    }
                }
                auto* above = it->next();
                if (above) {
                    if (!check(above->value.id, e.id)
                        && segmentIntersection(above->value.a, above->value.b, v[e.id].a, v[e.id].b, pt)) {
                        Q.emplace(pt, e.id, above->value.id, 1);
                        res.emplace_back(e.id, above->value.id, pt);
                    }
                }
            } else if (e.type == 2) {
                auto it = nodes[e.id];
                auto* below = it->prev();
                auto* above = it->next();
                if (below && above) {
                    Vec2D<double> pt;
                    if (!check(above->value.id, below->value.id)
                        && segmentIntersection(above->value.a, above->value.b, below->value.a, below->value.b, pt)) {
                        Q.emplace(pt, below->value.id, above->value.id, 1);
                        res.emplace_back(below->value.id, above->value.id, pt);
                    }
                }
                tree.erase(it);
                //nodes[e.id] = nullptr;
            } else {
                int belowId = e.id;
                int aboveId = e.id2;
                auto below = nodes[belowId];
                auto above = nodes[aboveId];

                swap(nodes[belowId], nodes[aboveId]);
                swap(below->value, above->value);
                swap(belowId, aboveId);

                Vec2D<double> pt;
                auto* belowbelow = below->prev();
                if (belowbelow) {
                    if (!check(belowbelow->value.id, below->value.id)
                        && segmentIntersection(belowbelow->value.a, belowbelow->value.b, below->value.a, below->value.b, pt)) {
                        Q.emplace(pt, belowbelow->value.id, below->value.id, 1);
                        res.emplace_back(belowbelow->value.id, below->value.id, pt);
                    }
                }
                auto* aboveabove = above->next();
                if (aboveabove) {
                    if (!check(aboveabove->value.id, above->value.id)
                        && segmentIntersection(aboveabove->value.a, aboveabove->value.b, above->value.a, above->value.b, pt)) {
                        Q.emplace(pt, above->value.id, aboveabove->value.id, 1);
                        res.emplace_back(above->value.id, aboveabove->value.id, pt);
                    }
                }
            }
        }

        return res;
    }
}
