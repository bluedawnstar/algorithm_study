#pragma once

template <typename T, int KD = 2>
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

    KDTree() {
        count = 0;
        root = nullptr;
    }

    void insert(Node* newNode) {
        if (root == nullptr) {
            root = newNode;
            return;
        }

        int currD = 0;
        Node* curr = root;

        while (curr) {
            if (newNode->point[currD] < curr->point[currD]) {
                if (curr->left == nullptr) {
                    curr->left = newNode;
                    break;
                }
                curr = curr->left;
            } else {
                if (curr->right == nullptr) {
                    curr->right = newNode;
                    break;
                }
                curr = curr->right;
            }
            currD = (currD + 1) % KD;
        }
    }

    template <typename U>
    Node* insert(const U& data) {
        Node* p = createNode(data);
        insert(p);
        return p;
    }


    template <typename U>
    Node* find(const U& data) const {
        if (!root) {
            return nullptr;
        }

        int currD = 0;
        Node* curr = root;

        while (curr) {
            if (isSame(curr, data))
                break;
            curr = (data[currD] < curr->point[currD]) ? curr->left : curr->right;
            currD = (currD + 1) % KD;
        }

        return curr;
    }

    Node* findMin(int dimen) {
        return _findMin(root, dimen, 0);
    }


    template <typename U>
    void deleteNode(const U& data) {
        root = _deleteNode(root, data, 0);
    }

protected:
    Node* createNode(const vector<T>& data) {
        Node* p = new Node;
        copy(data.begin(), data.end(), p->point);
        p->left = p->right = nullptr;
        return p;
    }

    Node* createNode(const T data[]) {
        Node* p = new Node;
        copy(data, data + KD, p->point);
        p->left = p->right = nullptr;
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
    Node* _deleteNode(Node* node, const U& data, int depth) {
        if (node == nullptr)
            return nullptr;

        int currD = depth % KD;

        if (isSame(node, data)) {
            if (node->right != nullptr) {
                Node* minNode = findMin(node->right, currD);
                copyData(node, minNode->point);
                node->right = _deleteNode(node->right, minNode->point, depth + 1);
            } else if (node->left != nullptr) {
                Node* minNode = findMin(node->left, currD);
                copyData(node, minNode->point);
                node->right = _deleteNode(node->left, minNode->point, depth + 1);
            } else {
                delete node;
                return nullptr;
            }
            return node;
        }

        if (data[currD] < node->point[currD])
            node->left = _deleteNode(node->left, data, depth + 1);
        else
            node->right = _deleteNode(node->right, data, depth + 1);
        return node;
    }
};
