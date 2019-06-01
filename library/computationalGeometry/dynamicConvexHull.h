#pragma once

#if 0
//template <typename T>
typedef int T;
struct DynamicConvexHull {
    // T has to have below functions
    //    1) operator ==()
    //    2) operator <()
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
            pair<T, T> value;

            Node() {
                init();
            }

            Node(const pair<T,T>& pt) {
                init(pt);
            }

            void init() {
                parent = this;
                left = this;
                right = this;
                cnt = 0;

                color = rbcBlack;
            }

            void init(const pair<T,T>& pt) {
                parent = nullptr;
                left = nullptr;
                right = nullptr;
                cnt = 1;

                color = rbcBlack;
                value = pt;
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

        Node*   root;   // root node
        int     N;

        using CompareT = function<bool(const pair<T, T>&, const pair<T, T>&, const pair<T, T>&)>;
        CompareT less;

        //-----------------------------------------------------------

        explicit RBTreeSimple(const CompareT& less) : less(less) {
            root = nullptr;
            N = 0;
        }

        ~RBTreeSimple() {
            deleteRecursive(root);
        }

        int size() const {
            return N;
        }

        bool empty() const {
            return N == 0;
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

        Node* operator [](int index) const {
            //assert((root != nullptr ? root->cnt : 0) == N);

            if (index < 0 || index >= N)
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
            //assert((root != nullptr ? root->cnt : 0) == N);

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

        Node* find(const pair<T,T>& key) const {
            Node *p = root;

            while (p != nullptr && p->value != key) {
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


        pair<Node*, bool> insert(const pair<T,T>& pt) {
            pair<Node*, bool> ins = insertBST(pt);
            if (!ins.second)
                return ins;

            Node* x = ins.first;
            Node* y;

            x->color = rbcRed;
            while ((x != root) && (x->parent && x->parent->color == rbcRed)) {
                bool left = true;
                if (x->parent == x->parent->parent->left)
                    y = x->parent->parent->right;
                else {
                    y = x->parent->parent->left;
                    left = false;
                }

                if (y && y->color == rbcRed) {
                    x->parent->color = rbcBlack;
                    y->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    } else if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }
                    x->parent->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    if (left)
                        rotateRight(x->parent->parent);
                    else
                        rotateLeft(x->parent->parent);
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

        bool erase(const pair<T,T>& pt) {
            return erase(find(pt));
        }

        void clear() {
            deleteRecursive(root);
            root = nullptr;
        }

    protected:
        Node* createNode(const pair<T,T>& pt) {
            Node* p = new Node(pt);
            N++;
            return p;
        }

        void destroyNode(Node* node) {
            assert(node != nullptr);
            delete node;
            N--;
        }

        void deleteRecursive(Node* node) {
            if (node != nullptr) {
                deleteRecursive(node->left);
                deleteRecursive(node->right);
                destroyNode(node);
            }
        }

        pair<Node*, bool> insertBST(const pair<T,T>& pt) {
            Node* parent = nullptr;
            Node* x = root;

            while (x != nullptr) {
                parent = x;

                if (pt == x->value) {
                    return make_pair(x, false);
                } else {
                    if (pt < x->value)
                        x = x->left;
                    else
                        x = x->right;
                }
            }

            Node* newNode = createNode(pt);
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
                bool left = (x == x->parent->left);
                w = left ? x->parent->right : x->parent->left;
                if (!w)
                    break;

                if (w->color == rbcRed) {
                    w->color = rbcBlack;
                    x->parent->color = rbcRed;
                    if (left) {
                        rotateLeft(x->parent);
                        w = x->parent->right;
                    } else {
                        rotateRight(x->parent);
                        w = x->parent->left;
                    }
                    if (!w)
                        break;
                }

                if ((!w->left || w->left->color == rbcBlack) && (!w->right || w->right->color == rbcBlack)) {
                    w->color = rbcRed;
                    x = x->parent;
                    continue;
                }

                if (left) {
                    if (!w->right || w->right->color == rbcBlack) { //xxx
                        w->left->color = rbcBlack; //xxx
                        w->color = rbcRed;
                        rotateRight(w); //xxx
                        w = x->parent->right; //xxx
                    }

                    w->color = x->parent->color;
                    x->parent->color = rbcBlack;
                    w->right->color = rbcBlack; //xxx
                    rotateLeft(x->parent); //xxx
                    x = root;
                } else {
                    if (!w->left || w->left->color == rbcBlack) { //xxx
                        w->right->color = rbcBlack; //xxx
                        w->color = rbcRed;
                        rotateLeft(w); //xxx
                        w = x->parent->left; //xxx
                    }

                    w->color = x->parent->color;
                    x->parent->color = rbcBlack;
                    w->left->color = rbcBlack; //xxx
                    rotateRight(x->parent); //xxx
                }
                x = root;
            }

            x->color = rbcBlack;
        }

        void updateNodeToRoot(Node* node) {
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


    vector<pair<int, int>> upper;   // ordered by x coordinate (clockwise)
    vector<pair<int, int>> lower;   // ordered by x coordinate (counter-clockwise)

    void buildWithMutablePoints(vector<pair<int, int>>& points) {
        int j = 0, k = 0, n = int(points.size());
        sort(points.begin(), points.end());
        upper.resize(n);
        lower.resize(n);
        for (int i = 0; i < n; i++) {
            while (j >= 2 && cross(lower[j - 2], lower[j - 1], points[i]) <= 0) // cw?
                j--;
            while (k >= 2 && cross(upper[k - 2], upper[k - 1], points[i]) >= 0) // ccw?
                k--;
            lower[j++] = points[i];
            upper[k++] = points[i];
        }
        upper.resize(k);
        lower.resize(j);
    }

    void build(vector<pair<int, int>> points) {
        buildWithMutablePoints(points);
    }

    //---

    void addLeft(int x, int y) {
        if (lower.empty()) {
            upper.emplace_back(x, y);
            lower.emplace_back(x, y);
            return;
        } else if (lower.size() == 1) {
            if (lower.front().first == x && lower.front().second == y) {
                // boundary
            } else if (lower.front().first == x && y > lower.front().second) {
                lower.emplace_back(x, y);
                upper.emplace_back(x, y);
            } else {
                lower.emplace(lower.begin(), x, y);
                upper.emplace(upper.begin(), x, y);
            }
            return;
        } else if (lower.front().first == x) {
            // ...
            if (lower.back().first == x && lower.front().second == y) {
                // boundary
            } else if (lower.front().first == x && y > lower.front().second) {
                lower.emplace_back(x, y);
                upper.emplace_back(x, y);
            } else {
                lower.emplace(lower.begin(), x, y);
                upper.emplace(upper.begin(), x, y);
            }
        }

        //TODO: ...
    }

    void addRight(int x, int y) {
        if (lower.empty()) {
            upper.emplace_back(x, y);
            lower.emplace_back(x, y);
            return;
        } else if (lower.size() == 1) {
            lower.emplace_back(x, y);
            upper.emplace_back(x, y);
            return;
        } else if (lower.back().first == x) {
            // ...
        }

        //TODO: ...
    }

    void addUpper(int pos, int x, int y) {
        // + left boundary
    }

    void addLower(int pos, int x, int y) {
        // + right boundary
    }

    void add(int x, int y) {
        if (lower.empty()) {
            addRight(x, y);
            return;
        }

        if (x <= lower.front().first) {
            addLeft(x, y);
            return;
        } else if (x >= lower.back().first) {
            addRight(x, y);
            return;
        }

        if (1ll * (y - lower.front().second) * (lower.back().first - lower.front().first) >=
            1ll * (x - lower.front().first) * (lower.back().second - lower.front().second)) {
            // upper envelope
            int i = upper_bound(upper.begin(), upper.end(), make_pair(x, y), [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.first < b.first;
            }) - upper.begin();

            auto d = 1ll * (y - upper[i - 1].second) * (upper[i].first - upper[i - 1].first)
                   - 1ll * (x - upper[i - 1].first) * (upper[i].second - upper[i - 1].second);
            if (d > 0)
                addUpper(i, x, y);
            else if (d == 0) {
                // boundary;
            } else {
                // inner;
            }
        } else {
            // lower envelope
            int i = upper_bound(lower.begin(), lower.end(), make_pair(x, y), [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.first < b.first;
            }) - lower.begin();

            auto d = 1ll * (y - lower[i - 1].second) * (lower[i].first - lower[i - 1].first)
                   - 1ll * (x - lower[i - 1].first) * (lower[i].second - lower[i - 1].second);
            if (d < 0)
                addLower(i, x, y);
            else if (d == 0) {
                // boundary;
            } else {
                // inner;
            }
        }
    }

    //--- 

    // return square distance
    long long calcMaxDiameter() const {
        int j = 0, k, m;
        k = int(lower.size()) - 1;
        m = int(upper.size()) - 1;
        long long dist = 0;
        while (j < m || k > 0) {
            dist = max(dist, squareDist(upper[j], lower[k]));
            if (j == m)
                k--;
            else if (k == 0)
                j++;
            else {
                if (1ll * (upper[j + 1].second - upper[j].second) * (lower[k].first - lower[k - 1].first) >
                    1ll * (lower[k].second - lower[k - 1].second) * (upper[j + 1].first - upper[j].first))
                    j++;
                else
                    k--;
            }
        }
        return dist;
    }


    //--- hit test

    /*
       :   D    :
       +  +--+  +
       : /    \ :
       :/      \:
     B |   A....+
       +....    | C
       :\      /:
       : \    / :
       +  +--+  +
       :   E    :
    */
    enum AreaT {
        aNone = -1, // empty
        aLeft,      // B
        aRight,     // C
        aUpper,     // D
        aLower,     // E
        aInner,     // A
        aBoundary,  // A
    };
    // O(logN)
    AreaT hitTest(int x, int y) const {
        if (lower.empty())
            return aNone;

        if (x < lower.front().first)
            return aLeft;                       // B
        else if (x > lower.back().first)
            return aRight;                      // C
        else if (lower.back().first == x) {
            if (y < lower.front().second)
                return aLower;
            else if (y > lower.front().second)
                return aUpper;
            else
                return aBoundary;
        }

        if (1ll * (y - lower.front().second) * (lower.back().first - lower.front().first) >=
            1ll * (x - lower.front().first) * (lower.back().second - lower.front().second)) {
            // upper envelope
            int i = upper_bound(upper.begin(), upper.end(), make_pair(x, y), [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.first < b.first;
            }) - upper.begin();

            auto d = 1ll * (y - upper[i - 1].second) * (upper[i].first - upper[i - 1].first)
                   - 1ll * (x - upper[i - 1].first) * (upper[i].second - upper[i - 1].second);
            if (d > 0)
                return aUpper;
            else if (d == 0)
                return aBoundary;
            else
                return aInner;
        } else {
            // lower envelope
            int i = upper_bound(lower.begin(), lower.end(), make_pair(x, y), [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.first < b.first;
            }) - lower.begin();

            auto d = 1ll * (y - lower[i - 1].second) * (lower[i].first - lower[i - 1].first)
                   - 1ll * (x - lower[i - 1].first) * (lower[i].second - lower[i - 1].second);
            if (d < 0)
                return aLower;
            else if (d == 0)
                return aBoundary;
            else
                return aInner;
        }
    }

private:
    static long long cross(const pair<int, int>& P, const pair<int, int>& Q, const pair<int, int>& R) {
        return 1ll * (Q.first - P.first) * (R.second - P.second) - 1ll * (R.first - P.first) * (Q.second - P.second);
    }

    static long long squareDist(const pair<int, int>& L, const pair<int, int>& R) {
        long long x = L.first - R.first;
        long long y = L.second - R.second;
        return x * x + y * y;
    }
};

#else

template <typename T>
struct DynamicConvexHull {
    // <PRECONDITION>
    //  - Convex hull points are stored in counter-clockwise order
    // 
    // O(H)
    // return true if convex hull is changed
    static bool addPoint(vector<Vec2D<T>>& hull, Vec2D<T> p) {
        if (inside(hull, p))
            return false;

        // point having minimum distance from the point p
        int idx = 0;
        int N = int(hull.size());
        for (int i = 1; i < N; i++) {
            if (squareDistL2(p, hull[i]) < squareDistL2(p, hull[idx]))
                idx = i;
        }

        // find the upper tangent
        int up = idx;
        while (orientation(p, hull[up], hull[(up + 1) % N]) >= 0)
            up = (up + 1) % N;

        // find the lower tangent
        int low = idx;
        while (orientation(p, hull[low], hull[(N + low - 1) % N]) <= 0)
            low = (N + low - 1) % N;

        vector<Vec2D<T>> res;

        int curr = up;
        res.push_back(hull[curr]);
        while (curr != low) {
            curr = (curr + 1) % N;
            res.push_back(hull[curr]);
        }
        res.push_back(p);

        swap(hull, res);

        return true;
    }

private:
    // checks whether the point crosses the convex hull or not
    static T orientation(const Vec2D<T>& a, const Vec2D<T>& b, const Vec2D<T>& c) {
        return (b.second - a.second) * (c.first - b.first) - (c.second - b.second) * (b.first - a.first);
    }

    static T squareDistL2(const Vec2D<T>& p1, const Vec2D<T>& p2) {
        return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
    }

#if 0
    static bool inside(vector<Vec2D<T>> poly, Vec2D<T> p) {
        Vec2D<T> mid{ 0, 0 };

        int N = int(poly.size());

        p.first *= N;
        p.second *= N;
        for (int i = 0; i < N; i++) {
            mid.first += poly[i].first;
            mid.second += poly[i].second;
            poly[i].first *= N;
            poly[i].second *= N;
        }

        for (int i = 0, j; i < N; i++) {
            j = (i + 1) % N;
            T x1 = poly[i].first, x2 = poly[j].first;
            T y1 = poly[i].second, y2 = poly[j].second;

            T dy = y1 - y2;
            T dx = x2 - x1;
            T d = x1 * y2 - y1 * x2;

            if ((dy * mid.first + dx * mid.second + d) * (dy * p.first + dx * p.second + d) < 0)
                return false;
        }

        return true;
    }
#else
    static bool inside(const vector<Vec2D<T>>& polygon, Vec2D<T> pt) {
        int crossCnt = 0;
        for (int i = 0; i < int(polygon.size()); i++) {
            int j = (i + 1) % int(polygon.size());
            if ((polygon[i].second > pt.second) != (polygon[j].second > pt.second)) {
                double atX = double(polygon[j].first - polygon[i].first) * (pt.second - polygon[i].second) /
                    double(polygon[j].second - polygon[i].second) + polygon[i].first;
                if (pt.first < atX)
                    crossCnt++;
                //else if (pt.first == atX) // at boundary
                //    return true;
            }
        }

        return (crossCnt & 1) != 0;
    }
#endif
};
#endif