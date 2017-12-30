#pragma once

// <PRECONDITION>
//  - T must support 'operator <'.
// 
// Min heap
template <typename T>
struct MergeableHeap {
    struct Heap {
        T value;
        Heap* left;
        Heap* right;

        Heap(const T& _value) : value(_value), left(nullptr), right(nullptr) {
        }
    };

    Heap* root;

    MergeableHeap() {
        root = nullptr;
    }

    ~MergeableHeap() {
        destroy(root);
    }

    bool empty() const {
        return root == nullptr;
    }

    T top() {
        return root->value;
    }

    T pop() {
        T res = root->value;
        root = merge(root->left, root->right);
        return res;
    }

    void push(const T& value) {
        root = add(root, value);
    }

    void mergeFrom(MergeableHeap& rhs) {
        root = merge(root, rhs.root);
        rhs.root = nullptr;
    }

    //-------------------------------------------------------------------------

    static Heap* create(const T& value) {
        return new Heap(value);
    }

    static void destroy(Heap* h) {
        if (!h)
            return;

        destroy(h->left);
        destroy(h->right);
        delete h;
    }

    static Heap* merge(Heap* a, Heap* b) {
        if (!a)
            return b;

        if (!b)
            return a;

        if (b->value < a->value)
            swap(a, b);

        if (rand() & 1)
            swap(a->left, a->right);

        a->left = merge(a->left, b);

        return a;
    }

    static Heap* add(Heap* h, const T& value) {
        return merge(h, new Heap(value));
    }
};