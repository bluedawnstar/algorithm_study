#pragma once

//--- Block Allocator ---------------------------------------------------------

template <typename T, size_t ChunkSizeN>
struct Allocator {
    struct ChunkT {
        int n;
        T values[ChunkSizeN];

        ChunkT() {
            n = 0;
        }
    };
    stack<shared_ptr<ChunkT>> mChunks;

    Allocator() {
        mChunks.push(shared_ptr<ChunkT>(new ChunkT()));
    }

    ~Allocator() {
        // no action
    }

    T* alloc() {
        if (mChunks.top()->n >= ChunkSizeN)
            mChunks.push(shared_ptr<ChunkT>(new ChunkT()));
        return &mChunks.top()->values[mChunks.top()->n++];
    }

    void clear() {
        stack<shared_ptr<ChunkT>>().swap(mChunks);
    }
};

//--- Trie --------------------------------------------------------------------

struct Trie {
    static const size_t MaxCharN = 26;
    static const size_t AllocBlockSize = 128;
    static int ch2i(char ch) {
        return ch - 'a';
    }

    struct Node {
        int     leafCount;
        Node*   parent;
        Node*   children[MaxCharN];

        void init(Node* parent = nullptr) {
            this->leafCount = 0;
            this->parent = parent;
            memset(this->children, 0, sizeof(this->children));
        }

        bool isLeaf() const {
            return leafCount > 0;
        }

        bool isEmpty() const {
            if (leafCount > 0)
                return false;
            for (int i = 0; i < MaxCharN; i++) {
                if (children[i])
                    return false;
            }
            return true;
        }
    };

    Node mRoot;

    Trie() {
        mRoot.init();
        mFreeNode = nullptr;
    }

    void clear() {
        for (int i = 0; i < MaxCharN; i++) {
            if (mRoot.children[i]) {
                deleteNode(mRoot.children[i]);
                mRoot.children[i] = nullptr;
            }
        }
    }


    // return true if it's a new string.
    template <typename T>
    bool insertWord(T s, int len) {
        if (len <= 0)
            return false;

        Node* p = &mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            if (!p->children[idx])
                p->children[idx] = allocNode(p);
            p = p->children[idx];
        }
        return ++p->leafCount == 1;
    }

    // return exactly matched word
    template <typename T>
    Node* findWord(T s, int len) {
        if (len <= 0)
            return nullptr;

        Node* p = &mRoot;
        for (int i = 0; i < len && p; i++) {
            int idx = ch2i(s[i]);
            p = p->children[idx];
        }
        return (p && p->leafCount > 0) ? p : nullptr;
    }

    // prefix matching
    // return (prefix_matching_length, word_matched?)
    template <typename T>
    pair<int, bool> searchWord(T s, int len) {
        if (len <= 0)
            return make_pair(0, false);

        Node* p = &mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            p = p->children[idx];
            if (!p)
                return make_pair(i, false);
        }
        return make_pair(len, p->isLeaf());
    }

    // remove a exactly matched word, but the real node is not deleted
    template <typename T>
    bool removeWord(T s, int len) {
        Node* p = findWord(s, len);
        if (!p)
            return false;

        p->leafCount--;

        return true;
    }

    // delete a exactly matched word
    template <typename T>
    bool deleteWord(T s, int len, bool all = false) {
        if (len <= 0)
            return false;

        Node* p = findWord(s, len);
        if (!p)
            return false;

        if (all)
            p->leafCount = 0;
        else
            p->leafCount--;

        for (int i = len - 1; i >= 0 && p != &mRoot && p->isEmpty(); i--) {
            Node* del = p;
            p = p->parent;
            p->children[ch2i(s[i])] = nullptr;
            freeNode(del);
        }

        return true;
    }

private:
    Allocator<Node, AllocBlockSize> mAlloc;
    Node* mFreeNode;

    Node* allocNode(Node* parent = nullptr) {
        Node* p = nullptr;
        if (mFreeNode) {
            p = mFreeNode;
            mFreeNode = mFreeNode->parent;
            p->init(parent);
        } else {
            p = mAlloc.alloc();
            p->init(parent);
        }

        return p;
    }

    void freeNode(Node* node) {
        node->parent = mFreeNode;
        mFreeNode = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (int i = 0; i < MaxCharN; i++) {
            if (p->children[i])
                deleteNode(p->children[i]);
        }

        freeNode(p);
    }
};

//--- Compressed Trie ---------------------------------------------------------

struct CompressedTrie {
    static const size_t MaxCharN = 26;
    static const size_t AllocBlockSize = 128;
    static int ch2i(char ch) {
        return ch - 'a';
    }

    struct Node {
        const char* text;
        int     textLen;
        int     leafCount;
        Node*   parent;
        Node*   children[MaxCharN];

        void init(Node* parent = nullptr) {
            this->text = nullptr;
            this->textLen = 0;
            this->leafCount = 0;
            this->parent = parent;
            memset(this->children, 0, sizeof(this->children));
        }

        bool isLeaf() const {
            return leafCount > 0;
        }

        bool isEmpty() const {
            if (leafCount > 0)
                return false;
            for (int i = 0; i < MaxCharN; i++) {
                if (children[i])
                    return false;
            }
            return true;
        }
    };

    Node mRoot;

    CompressedTrie() {
        mRoot.init();
        mFreeNode = nullptr;
    }

    void clear() {
        for (int i = 0; i < MaxCharN; i++) {
            if (mRoot.children[i]) {
                deleteNode(mRoot.children[i]);
                mRoot.children[i] = nullptr;
            }
        }
    }


    // return true if it's a new string.
    bool insertWord(const char* s, int len) {
        if (len <= 0)
            return false;

        Node* p = &mRoot;

        int idx = ch2i(s[0]);
        if (!p->children[idx]) {
            p = p->children[idx] = allocNode(p);
            p->text = s;
            p->textLen = len;
        } else {
            p = p->children[idx];
            for (int i = 0; i < len; ) {
                int j;
                for (j = 0; j < p->textLen && i < len; i++, j++) {
                    if (s[i] != p->text[j])
                        break;
                }
                //assert(j > 0);

                if (j < p->textLen)
                    p = split(p, j);

                if (i < len) {
                    idx = ch2i(s[i]);
                    if (!p->children[idx]) {
                        // add new node
                        p = p->children[idx] = allocNode(p);
                        p->text = s + i;
                        p->textLen = len - i;
                        break;
                    } else {
                        p = p->children[idx];
                    }
                }
            }
        }
        return ++p->leafCount == 1;
    }

    // return exactly matched word
    template <typename T>
    Node* findWord(T s, int len) {
        if (len <= 0)
            return nullptr;

        Node* p = mRoot.children[ch2i(s[0])];
        for (int i = 0; p; ) {
            for (int j = 0; j < p->textLen; i++, j++) {
                if (i >= len || s[i] != p->text[j])
                    return nullptr;
            }
            if (i >= len)
                break;

            p = p->children[ch2i(s[i])];
        }

        return (p && p->leafCount > 0) ? p : nullptr;
    }

    // prefix matching
    // return (prefix_matching_length, word_matched?)
    template <typename T>
    pair<int, bool> searchWord(T s, int len) {
        if (len <= 0)
            return make_pair(0, false);

        Node* p = mRoot.children[ch2i(s[0])];
        int i = 0;
        while (p) {
            for (int j = 0; j < p->textLen; i++, j++) {
                if (i >= len || s[i] != p->text[j])
                    return make_pair(i, false);
            }
            if (i >= len)
                break;

            p = p->children[ch2i(s[i])];
        }

        return make_pair(i, p && p->leafCount > 0);
    }

    // remove a exactly matched word, but the real node is not deleted
    template <typename T>
    bool removeWord(T s, int len) {
        Node* p = findWord(s, len);
        if (!p)
            return false;

        p->leafCount--;

        return true;
    }

    // delete a exactly matched word
    template <typename T>
    bool deleteWord(T s, int len, bool all = false, bool merge = true) {
        if (len <= 0)
            return false;

        Node* p = findWord(s, len);
        if (!p)
            return false;

        if (all)
            p->leafCount = 0;
        else
            p->leafCount--;

        if (p->isEmpty()) {
            Node* del = p;
            p = p->parent;
            p->children[ch2i(del->text[0])] = nullptr;
            freeNode(del);

            if (merge && !p->isLeaf() && p != &mRoot) {
                // If there is only one child of parent, the parent is merged to the child.
                int idx = -1, cnt = 0;
                for (int i = 0; i < MaxCharN; i++) {
                    if (p->children[i]) {
                        idx = i;
                        cnt++;
                    }
                }
                if (cnt == 1) {
                    Node* parent = p->parent;
                    Node* child = p->children[idx];

                    child->text -= p->textLen;
                    child->textLen += p->textLen;

                    idx = ch2i(p->text[0]);
                    parent->children[idx] = child;
                    child->parent = parent;

                    freeNode(p);
                }
            }
        }

        return true;
    }

private:
    Allocator<Node, AllocBlockSize> mAlloc;
    Node* mFreeNode;

    Node* allocNode(Node* parent = nullptr) {
        Node* p = nullptr;
        if (mFreeNode) {
            p = mFreeNode;
            mFreeNode = mFreeNode->parent;
            p->init(parent);
        } else {
            p = mAlloc.alloc();
            p->init(parent);
        }

        return p;
    }

    void freeNode(Node* node) {
        node->parent = mFreeNode;
        mFreeNode = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (int i = 0; i < MaxCharN; i++) {
            if (p->children[i])
                deleteNode(p->children[i]);
        }

        freeNode(p);
    }

    Node* split(Node* node, int offset) {
        Node* newNode = allocNode(node->parent);
        newNode->text = node->text;
        newNode->textLen = offset;
        newNode->children[ch2i(node->text[offset])] = node;
        newNode->parent->children[ch2i(newNode->text[0])] = newNode;
        //assert(newNode->textLen > 0);

        node->text += offset;
        node->textLen -= offset;
        node->parent = newNode;
        //assert(node->textLen > 0);

        return newNode;
    }
};
