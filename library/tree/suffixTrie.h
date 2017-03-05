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

struct SuffixTrie {
    static const size_t MaxCharN = 26;
    static const size_t AllocBlockSize = 128;
    static int ch2i(char ch) {
        return ch - 'a';
    }

    struct Node {
        Node*   parent;                 // for delete
        Node*   suffixLink;

        int     suffixIndex;
        int     childCount;
        Node*   children[MaxCharN];

        void init(Node* parent = nullptr) {
            this->parent = parent;
            this->suffixLink = nullptr;
            this->suffixIndex = -1;
            this->childCount = 0;
            memset(this->children, 0, sizeof(this->children));
        }

        bool isLeaf() const {
            return childCount <= 0;
        }

        bool isEnd() const {
            return suffixIndex >= 0;
        }
    };

    Node mRoot;
    Allocator<Node, AllocBlockSize> mAlloc;
    Node* mFreeNode;

    string mText;
    Node* mSuffixLink[MaxCharN];

    SuffixTrie() {
        mRoot.init();
        mFreeNode = nullptr;
        memset(mSuffixLink, 0, sizeof(mSuffixLink));
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
        memset(mSuffixLink, 0, sizeof(mSuffixLink));
    }


    int build(const char* s, int len, bool finalize = true) {
        if (len <= 0)
            return 0;

        mText.clear();
        memset(mSuffixLink, 0, sizeof(mSuffixLink));

        int added = 0;
        for (int i = 0; i < len; i++)
            added += extendSuffix(s[i]);

        if (finalize)
            updateSuffixIndex();

        return added;
    }

    void updateSuffixIndex() {
        if (mText.empty())
            return;

        Node* slink = mSuffixLink[ch2i(mText.back())];
        for (int i = 0; slink != &mRoot; i++) {
            slink->suffixIndex = i;
            slink = slink->suffixLink;
        }
    }

    void resetSuffixIndex() {
        if (mText.empty())
            return;

        Node* slink = mSuffixLink[ch2i(mText.back())];
        for (int i = 0; slink != &mRoot; i++) {
            slink->suffixIndex = -1;
            slink = slink->suffixLink;
        }
    }


    int extendSuffix(char ch) {
        if (mText.empty()) {
            Node* p = allocNode(&mRoot);
            mRoot.children[ch2i(ch)] = p;
            mRoot.childCount++;
            mSuffixLink[ch2i(ch)] = p;

            mText += ch;
            return 1;
        }

        int idx = ch2i(ch);
        Node* slink = mSuffixLink[ch2i(mText.back())];

        Node* p = allocNode(slink);
        mSuffixLink[idx] = p;
        slink->children[idx] = p;
        slink->childCount++;

        int cnt = 1;
        for (slink = slink->suffixLink; slink; slink = slink->suffixLink) {
            if (slink->children[idx]) {
                p->suffixLink = slink->children[idx];
                break;
            }

            p = p->suffixLink = allocNode(slink);
            slink->children[idx] = p;
            slink->childCount++;
            cnt++;
        }
        mText += ch;

        return cnt;
    }

    int shrinkSuffix() {
        if (mText.empty())
            return 0;

        int cnt = 0;

        int idx = ch2i(mText.back());
        Node* p = mSuffixLink[idx];
        while (p && p != &mRoot && p->childCount <= 0) {
            Node* del = p;
            p->parent->children[idx] = nullptr;
            p->parent->childCount--;
            p = p->suffixLink;

            freeNode(del);

            cnt++;
        }

        mSuffixLink[idx] = (p && p != &mRoot) ? p : nullptr;
        mText.pop_back();

        return cnt;
    }


    // return (prefix_matching_length, suffix_index)
    template <typename T>
    pair<int, int> search(T s, int len) {
        if (len <= 0)
            return make_pair(0, -1);

        Node* p = &mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            if (!p->children[idx])
                return make_pair(i, -1);
            p = p->children[idx];
        }
        return make_pair(len, p->suffixIndex);
    }

private:
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
        p->suffixLink = &mRoot;

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
