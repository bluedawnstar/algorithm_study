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

struct SuffixTree {
    static const size_t MaxCharN = 26;
    static const size_t AllocBlockSize = 128;

    static int ch2i(char ch) {
        return ch - 'a';
    }

    struct Node {
        int     begin;
        int*    end;
        int     depth;                  // distance in characters from root

        Node*   parent;
        Node*   suffixLink;

        int     suffixIndex;
        int     childCount;
        Node*   children[MaxCharN];     //TODO: make more efficient

        void init() {
            this->begin = 0;
            this->end = nullptr;
            this->depth = 0;
            this->parent = nullptr;
            this->suffixLink = nullptr;
            this->suffixIndex = -1;
            this->childCount = 0;
            memset(this->children, 0, sizeof(this->children));
        }

        int getLength() {
            return *end - begin;
        }

        bool isLeaf() const {
            return childCount <= 0;
        }

        bool isEnd() const {
            return suffixIndex >= 0;
        }

        void addChild(int index, Node* node) {
            if (!children[index])
                childCount++;
            children[index] = node;
            if (node)
                node->parent = this;
        }
    };

    Node mRoot;
    string mText;

    Node* mActiveNode;
    int   mActiveLen;
    int*  mLeafEnd;

    SuffixTree() {
        mFreeNode = nullptr;

        mRoot.init();
        mRoot.end = allocInt(0);

        mActiveNode = nullptr;
        mActiveLen = 0;
        mLeafEnd = allocInt(0);
    }

    void clear() {
        for (int i = 0; i < MaxCharN; i++) {
            if (mRoot.children[i]) {
                deleteNode(mRoot.children[i]);
                mRoot.children[i] = nullptr;
                mRoot.childCount = 0;
            }
        }
        mText.clear();

        mActiveNode = nullptr;
        mActiveLen = 0;
        *mLeafEnd = 0;
    }

    // build a suffix trie from string 's'
    void build(const char* s, int len, bool finalize = true) {
        if (len <= 0)
            return;

        mText.clear();

        mActiveNode = nullptr;
        mActiveLen = 0;
        *mLeafEnd = 0;

        for (int i = 0; i < len; i++)
            extendSuffix(s[i]);

        if (finalize)
            setSuffixIndex();
    }

    // update suffix indexes.
    // It means there is no update(adding), because a node with suffix index is a leaf node.
    void setSuffixIndex() {
        if (mText.empty())
            return;
        setSuffixIndex(&mRoot);
    }

    void resetSuffixIndex() {
        if (mText.empty())
            return;
        resetSuffixIndex(&mRoot);
    }


    void extendSuffix(char ch) {
        int currPos = (int)mText.length();
        mText += ch;

        *mLeafEnd = currPos + 1;

        int idx = ch2i(ch);
        if (!mActiveNode)
            mActiveNode = &mRoot;

        Node* last = nullptr;
        while (mActiveLen >= 0) {
            Node* node = mActiveNode->children[ch2i(mText[currPos - mActiveLen])];

            // walk down
            while (node != nullptr && mActiveLen >= node->getLength()) {
                mActiveLen -= node->getLength();
                mActiveNode = node;
                node = node->children[ch2i(mText[currPos - mActiveLen])];
            }

            if (node == nullptr) {
                Node* newNode = allocNode(currPos, -1, mActiveNode->depth + mActiveNode->getLength());
                mActiveNode->addChild(idx, newNode);
                if (last != nullptr)
                    last->suffixLink = mActiveNode;
                last = nullptr;
            } else {
                int afterTailIdx = ch2i(mText[node->begin + mActiveLen]);

                if (afterTailIdx == idx) {
                    if (last != nullptr)
                        last->suffixLink = mActiveNode;
                    break;
                } else {
                    Node* splitNode = allocNode(node->begin, node->begin + mActiveLen, mActiveNode->depth + mActiveNode->getLength());
                    Node* newNode = allocNode(currPos, -1, node->depth + mActiveLen);
                    splitNode->addChild(idx, newNode);

                    node->begin += mActiveLen;
                    node->depth += mActiveLen;
                    splitNode->addChild(afterTailIdx, node);

                    mActiveNode->addChild(ch2i(mText[currPos - mActiveLen]), splitNode);
                    if (last != nullptr)
                        last->suffixLink = splitNode;
                    last = splitNode;
                }
            }

            if (mActiveNode == &mRoot)
                mActiveLen--;
            else
                mActiveNode = mActiveNode->suffixLink;
        }

        mActiveLen++;
    }

    // return (prefix_matching_length, suffix_index)
    template <typename T>
    pair<int, int> search(T s, int len) {
        if (len <= 0)
            return make_pair(0, -1);

        Node* p = mRoot.children[ch2i(s[0])];
        int i = 0;
        while (p) {
            for (int j = 0; j < p->getLength(); i++, j++) {
                if (i >= len || s[i] != mText[p->begin + j])
                    return make_pair(i, -1);
            }
            if (i >= len)
                break;

            p = p->children[ch2i(s[i])];
        }

        return make_pair(i, p ? p->suffixIndex : -1);
    }

private:
    Allocator<Node, AllocBlockSize> mAlloc;
    Allocator<int, AllocBlockSize> mAllocInt;
    Node* mFreeNode;

    Node* allocNode() {
        Node* p = nullptr;
        if (mFreeNode) {
            p = mFreeNode;
            mFreeNode = mFreeNode->parent;
            p->init();
        } else {
            p = mAlloc.alloc();
            p->init();
        }
        p->suffixLink = &mRoot;

        return p;
    }

    Node* allocNode(int begin, int end, int depth) {
        Node* node = allocNode();
        node->begin = begin;
        node->end = end < 0 ? mLeafEnd : allocInt(end);
        node->depth = depth;
        return node;
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

    int* allocInt(int val) {
        int* v = mAllocInt.alloc();
        *v = val;
        return v;
    }

    void setSuffixIndex(Node* node) {
        if (!node)
            return;

        if (node->isLeaf())
            node->suffixIndex = (int)mText.length() - node->depth - node->getLength();
        else {
            for (int i = 0; i < MaxCharN; i++) {
                if (node->children[i])
                    setSuffixIndex(node->children[i]);
            }
        }
    }

    void resetSuffixIndex(Node* node) {
        if (!node)
            return;

        node->suffixIndex = -1;
        for (int i = 0; i < MaxCharN; i++) {
            if (node->children[i])
                resetSuffixIndex(node->children[i]);
        }
    }
};
