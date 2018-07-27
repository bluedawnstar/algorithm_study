#pragma once

template <typename T = int, int KD = 2>
struct KDTree {
    struct Node {
        T point[KD];
        Node *left;
        Node *right;

        template <typename U>
        bool operator ==(const U& rhs) const {
            for (int i = 0; i < KD; i++) {
                if (point[i] != rhs[i])
                    return false;
            }
            return true;
        }
    };
    int count;
    Node* root;
    vector<pair<T, T>> bound; // [min, max)

    KDTree() : bound(KD, make_pair(numeric_limits<T>::max(), numeric_limits<T>::min())) {
        count = 0;
        root = nullptr;
    }


    void insert(Node* newNode) {
        if (root == nullptr) {
            root = newNode;
            count++;
            return;
        }

        int currD = 0;
        Node* curr = root;

        while (curr) {
            if (newNode->point[currD] < curr->point[currD]) {
                if (curr->left == nullptr) {
                    curr->left = newNode;
                    count++;
                    break;
                }
                curr = curr->left;
            } else {
                if (curr->right == nullptr) {
                    curr->right = newNode;
                    count++;
                    break;
                }
                curr = curr->right;
            }
            currD = (currD + 1) % KD;
        }
    }

    template <typename U>
    Node* insert(const U& point) {
        Node* p = createNode(point);
        insert(p);
        return p;
    }

    template <typename U>
    Node* find(const U& point) const {
        if (!root) {
            return nullptr;
        }

        int currD = 0;
        Node* curr = root;

        while (curr) {
            if (isSame(curr, point))
                break;
            curr = (point[currD] < curr->point[currD]) ? curr->left : curr->right;
            currD = (currD + 1) % KD;
        }

        return curr;
    }

    Node* findMin(int dimen) {
        return _findMin(root, dimen, 0);
    }

    template <typename U>
    void deleteNode(const U& point) {
        root = _deleteNode(root, point, 0);
    }

    template <typename U>
    Node* searchNearest(const U& point) {
        Node* res = nullptr;
        auto bnd = bound;
        double minDist = numeric_limits<double>::max();
        _searchNearest(root, point, 0, res, minDist, bnd);
        return res;
    }

    // inclusive
    template <typename U>
    vector<Node*> searchRegion(const U& minPoint, const U& maxPoint) {
        vector<Node*> res;
        auto bnd = bound;
        _searchRegion(root, 0, res, bnd, minPoint, maxPoint);
        return res;
    }

    // inclusive
    template <typename U>
    vector<Node*> searchRegionFast(const U& minPoint, const U& maxPoint) {
        vector<Node*> res;
        // [min, max]
        vector<pair<T, T>> bound(KD);
        for (int i = 0; i < KD; i++) {
            bound[i].first = minPoint[i];
            bound[i].second = maxPoint[i] + 1;
        }
        _searchRegionFast(root, 0, res, bound);
        return res;
    }

    template <typename U>
    vector<Node*> searchKNN(const U& point, int K) {
        priority_queue<pair<double,Node*>> pq;
        auto bnd = bound;
        _searchKNN(root, point, 0, pq, K, bnd);

        vector<Node*> res(pq.size());
        for (int i = int(pq.size()) - 1; i >= 0; i--) {
            res[i] = pq.top().second;
            pq.pop();
        }
        return res;
    }

protected:
    Node* createNode(const vector<T>& point) {
        Node* p = new Node;
        copy(point.begin(), point.end(), p->point);
        p->left = p->right = nullptr;

        for (int i = 0; i < KD; i++) {
            bound[i].first = min(bound[i].first, point[i]);
            bound[i].second = max(bound[i].second, point[i] + 1);
        }

        return p;
    }

    Node* createNode(const T point[]) {
        Node* p = new Node;
        copy(point, point + KD, p->point);
        p->left = p->right = nullptr;

        for (int i = 0; i < KD; i++) {
            bound[i].first = min(bound[i].first, point[i]);
            bound[i].second = max(bound[i].second, point[i] + 1);
        }

        return p;
    }

    void copyData(Node* dst, const vector<T>& src) {
        copy(src.begin(), src.end(), dst->point);
    }

    void copyData(Node* dst, const T src[]) {
        copy(src, src + KD, dst->point);
    }

    template <typename U>
    bool isSame(const Node* lhs, const U& rhs) const {
        for (int i = 0; i < KD; i++) {
            if (lhs->point[i] != rhs[i])
                return false;
        }
        return true;
    }

    Node* minNode(Node* x, Node* y, Node* z, int dimen) {
        Node *res = x;
        if (y != nullptr && y->point[dimen] < res->point[dimen])
            res = y;
        if (z != nullptr && z->point[dimen] < res->point[dimen])
            res = z;
        return res;
    }

    Node* _findMin(Node* node, int dimen, int depth) {
        if (node == nullptr)
            return nullptr;

        int currD = depth % KD;

        if (currD == dimen) {
            if (node->left == nullptr)
                return node;
            else
                return _findMin(node->left, dimen, depth + 1);
        }

        return minNode(node,
            _findMin(node->left, dimen, depth + 1),
            _findMin(node->right, dimen, depth + 1),
            dimen);
    }

    template <typename U>
    Node* _deleteNode(Node* node, const U& point, int depth) {
        if (node == nullptr)
            return nullptr;

        int currD = depth % KD;

        if (isSame(node, point)) {
            if (node->right != nullptr) {
                Node* minNode = _findMin(node->right, currD, depth + 1);
                copyData(node, minNode->point);
                node->right = _deleteNode(node->right, minNode->point, depth + 1);
            } else if (node->left != nullptr) {
                Node* minNode = _findMin(node->left, currD, depth + 1);
                copyData(node, minNode->point);
                node->right = _deleteNode(node->left, minNode->point, depth + 1);
                node->left = nullptr;
            } else {
                delete node;
                count--;
                node = nullptr;
            }
        } else if (point[currD] < node->point[currD])
            node->left = _deleteNode(node->left, point, depth + 1);
        else
            node->right = _deleteNode(node->right, point, depth + 1);

        return node;
    }

    template <typename U>
    double calcDistance(Node* node, const U& point) {
        double res = 0.0;
        for (int i = 0; i < KD; i++) {
            res += double(node->point[i] - point[i]) * double(node->point[i] - point[i]);
        }
        return sqrt(res);
    }

    template <typename U>
    void _searchNearest(Node* node, const U& point, int depth, Node*& res, double& minDist, vector<pair<T, T>>& bound) {
        if (node == nullptr)
            return;

        if (res != nullptr) {
            for (int i = 0; i < KD; i++) {
                if (point[i] + minDist < bound[i].first || bound[i].second <= point[i] - minDist)
                    return;
            }
        }

        int currD = depth % KD;

        auto dist = calcDistance(node, point);
        if (dist < minDist) {
            minDist = dist;
            res = node;
        }

        if (point[currD] < node->point[currD]) {
            auto temp = bound[currD];

            bound[currD].second = min(bound[currD].second, node->point[currD]);
            _searchNearest(node->left, point, depth + 1, res, minDist, bound);
            bound[currD].second = temp.second;

            if (node->point[currD] <= point[currD] + minDist) {
                bound[currD].first = max(bound[currD].first, node->point[currD]);
                _searchNearest(node->right, point, depth + 1, res, minDist, bound);
                bound[currD].first = temp.first;
            }
        } else {
            auto temp = bound[currD];

            bound[currD].first = max(bound[currD].first, node->point[currD]);
            _searchNearest(node->right, point, depth + 1, res, minDist, bound);
            bound[currD].first = temp.first;

            if (point[currD] - minDist < node->point[currD]) {
                bound[currD].second = min(bound[currD].second, node->point[currD]);
                _searchNearest(node->left, point, depth + 1, res, minDist, bound);
                bound[currD].second = temp.second;
            }
        }
    }

    void dfsIncludeAll(Node* node, vector<Node*>& res) {
        if (node->left)
            dfsIncludeAll(node->left, res);
        res.push_back(node);
        if (node->right)
            dfsIncludeAll(node->right, res);
    }

    template <typename U>
    void _searchRegion(Node* node, int depth, vector<Node*>& res, vector<pair<T, T>>& bound, const U& minPoint, const U& maxPoint) {
        if (node == nullptr)
            return;

        for (int i = 0; i < KD; i++) {
            if (maxPoint[i] < bound[i].first || bound[i].second <= minPoint[i] || bound[i].second <= bound[i].first)
                return;
        }

        bool include = true;
        for (int i = 0; i < KD; i++) {
            if (bound[i].first < minPoint[i] || maxPoint[i] + 1 < bound[i].second) {
                include = false;
                break;
            }
        }
        if (include) {
            dfsIncludeAll(node, res);
            return;
        }

        bool isIn = true;
        for (int i = 0; i < KD; i++) {
            if (node->point[i] < minPoint[i] || maxPoint[i] < node->point[i]) {
                isIn = false;
                break;
            }
        }
        if (isIn)
            res.push_back(node);

        int currD = depth % KD;

        if (minPoint[currD] < node->point[currD]) {
            auto temp = bound[currD].second;

            bound[currD].second = min(bound[currD].second, node->point[currD]);
            _searchRegion(node->left, depth + 1, res, bound, minPoint, maxPoint);
            bound[currD].second = temp;
        }
        if (node->point[currD] <= maxPoint[currD]) {
            auto temp = bound[currD].first;

            bound[currD].first = max(bound[currD].first, node->point[currD]);
            _searchRegion(node->right, depth + 1, res, bound, minPoint, maxPoint);
            bound[currD].first = temp;
        }
    }

    void _searchRegionFast(Node* node, int depth, vector<Node*>& res, vector<pair<T, T>>& bound) {
        if (node == nullptr)
            return;

        int currD = depth % KD;

        if (bound[currD].first >= bound[currD].second)
            return;

        if (bound[currD].first <= node->point[currD] && node->point[currD] < bound[currD].second) {
            bool isIn = true;
            for (int i = 0; i < KD; i++) {
                if (node->point[i] < bound[i].first || bound[i].second <= node->point[i]) {
                    isIn = false;
                    break;
                }
            }
            if (isIn)
                res.push_back(node);
        }

        if (bound[currD].first < node->point[currD]) {
            auto temp = bound[currD].second;

            bound[currD].second = min(bound[currD].second, node->point[currD]);
            _searchRegionFast(node->left, depth + 1, res, bound);
            bound[currD].second = temp;
        }
        if (node->point[currD] < bound[currD].second) {
            auto temp = bound[currD].first;

            bound[currD].first = max(bound[currD].first, node->point[currD]);
            _searchRegionFast(node->right, depth + 1, res, bound);
            bound[currD].first = temp;
        }
    }

    template <typename U>
    void _searchKNN(Node* node, const U& point, int depth, priority_queue<pair<double, Node*>>& pq, int K, vector<pair<T, T>>& bound) {
        if (node == nullptr)
            return;

        if (int(pq.size()) >= K) {
            for (int i = 0; i < KD; i++) {
                if (point[i] + pq.top().first < bound[i].first || bound[i].second <= point[i] - pq.top().first)
                    return;
            }
        }

        int currD = depth % KD;

        auto dist = calcDistance(node, point);
        if (int(pq.size()) < K || dist < pq.top().first) {
            pq.emplace(dist, node);
            if (int(pq.size()) > K)
                pq.pop();
        }

        if (point[currD] < node->point[currD]) {
            auto temp = bound[currD];

            bound[currD].second = min(bound[currD].second, node->point[currD]);
            _searchKNN(node->left, point, depth + 1, pq, K, bound);
            bound[currD].second = temp.second;

            if (int(pq.size()) < K || node->point[currD] <= point[currD] + pq.top().first) {
                bound[currD].first = max(bound[currD].first, node->point[currD]);
                _searchKNN(node->right, point, depth + 1, pq, K, bound);
                bound[currD].first = temp.first;
            }
        } else {
            auto temp = bound[currD];

            bound[currD].first = max(bound[currD].first, node->point[currD]);
            _searchKNN(node->right, point, depth + 1, pq, K, bound);
            bound[currD].first = temp.first;

            if (int(pq.size()) < K || point[currD] - pq.top().first < node->point[currD]) {
                bound[currD].second = min(bound[currD].second, node->point[currD]);
                _searchKNN(node->left, point, depth + 1, pq, K, bound);
                bound[currD].second = temp.second;
            }
        }
    }
};
