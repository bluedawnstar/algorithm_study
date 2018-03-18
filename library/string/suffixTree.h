#pragma once

struct SuffixTree {
    static int popcnt(unsigned x) {
#ifndef __GNUC__
        return (int)__popcnt(x);
#else
        return __builtin_popcount(x);
#endif
    }

    struct Node {
        int     id;

        int     begin;
        int*    end;
        int     depth;                  // distance in characters from root

        Node*   parent;
        Node*   suffixLink;

        int     suffixIndex;

        unsigned int childSet;
        vector<Node*> children;

        void init(int id) {
            this->id = id;
            this->begin = 0;
            this->end = nullptr;
            this->depth = 0;
            this->parent = nullptr;
            this->suffixLink = nullptr;
            this->suffixIndex = -1;
            this->childSet = 0;
            this->children.clear();
        }

        int getLength() const {
            return *end - begin;
        }

        bool isLeaf() const {
            return !childSet;
        }

        bool isEnd() const {
            return suffixIndex >= 0;
        }

        int getChildCount() const {
            return popcnt(childSet);
        }

        bool hasChild(int chIdx) const {
            return (childSet & (1 << chIdx)) != 0;
        }

        Node* getChild(int chIdx) const {
            if ((childSet & (1 << chIdx)) == 0)
                return nullptr;
            int idx = popcnt(childSet & ((1 << chIdx) - 1));
            return children[idx];
        }

        void setChild(int chIdx, Node* node) {
            int idx = popcnt(childSet & ((1 << chIdx) - 1));
            if ((childSet & (1 << chIdx)) != 0) {
                children[idx] = node;
            } else {
                children.insert(children.begin() + idx, node);
                childSet |= (1 << chIdx);
            }
            node->parent = this;
        }
    };

    Node            mRoot;
    string          mText;

    int             mNodeN;     // 
    vector<Node>    mNodes;     // 

    Node*           mActiveNode;
    int             mActiveLen;
    int*            mLeafEnd;

    int             mCharMin;
    int             mCharMax;

    //SuffixTree() {
    //    init(0);
    //}

    explicit SuffixTree(int maxN, int charMin = 'a', int charMax = 'z') : mCharMin(charMin), mCharMax(charMax) {
        init(maxN);
    }

    void init(int maxN) {
        mNodes.resize(maxN * 2);
        mInts.resize(maxN * 2);

        mNodeN = 0;
        mIntN = 0;

        mRoot.init(0);
        mRoot.end = allocInt(0);

        mText.clear();

        mActiveNode = nullptr;
        mActiveLen = 0;
        mLeafEnd = allocInt(0);
    }

    // build a suffix trie from string 's'
    void build(const string& s, bool finalize = true) {
        build(&s[0], (int)s.length(), finalize);
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
            extend(s[i]);

        if (finalize)
            setSuffixIndex();
    }

    int ch2i(int ch) const {
        return ch - mCharMin;
    }

    int charSize() const {
        return mCharMax - mCharMin + 1;
    }

    void extend(char ch) {
        int currPos = (int)mText.length();
        mText += ch;

        *mLeafEnd = currPos + 1;

        int idx = ch2i(ch);
        if (!mActiveNode)
            mActiveNode = &mRoot;

        Node* last = nullptr;
        while (mActiveLen >= 0) {
            Node* node = mActiveNode->getChild(ch2i(mText[currPos - mActiveLen]));

            // walk down
            while (node != nullptr && mActiveLen >= node->getLength()) {
                mActiveLen -= node->getLength();
                mActiveNode = node;
                node = node->getChild(ch2i(mText[currPos - mActiveLen]));
            }

            if (node == nullptr) {
                Node* newNode = allocNode(currPos, -1, mActiveNode->depth + mActiveNode->getLength());
                mActiveNode->setChild(idx, newNode);
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
                    splitNode->setChild(idx, newNode);

                    node->begin += mActiveLen;
                    node->depth += mActiveLen;
                    splitNode->setChild(afterTailIdx, node);

                    mActiveNode->setChild(ch2i(mText[currPos - mActiveLen]), splitNode);
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

    // update suffix indexes.
    // It means there is no update(adding), because a node with suffix index is a leaf node.
    void setSuffixIndex() {
        if (mText.empty())
            return;
        setSuffixIndex(&mRoot);
    }

    void resetIndex() {
        if (mText.empty())
            return;
        resetSuffixIndex(&mRoot);
    }


    const Node* getNode(int id) const {
        return (id == 0) ? &mRoot : &mNodes[id - 1];
    }

    Node* getNode(int id) {
        return (id == 0) ? &mRoot : &mNodes[id - 1];
    }

    // return (prefix_matching_length, suffix_index)
    pair<int, int> search(const string& s) const {
        return search(&s[0], (int)s.length());
    }

    // return (prefix_matching_length, suffix_index)
    pair<int, int> search(const char* s, int len) const {
        if (len <= 0)
            return make_pair(0, -1);

        Node* p = mRoot.getChild(ch2i(s[0]));
        int i = 0;
        while (p) {
            for (int j = 0; j < p->getLength(); i++, j++) {
                if (i >= len || s[i] != mText[p->begin + j])
                    return make_pair(i, -1);
            }
            if (i >= len)
                break;

            p = p->getChild(ch2i(s[i]));
        }

        return make_pair(i, p ? p->suffixIndex : -1);
    }

    string getString(const Node* node) const {
        vector<const Node*> stk;
        for (const Node* p = node; p != &mRoot; p = p->parent)
            stk.push_back(p);
        
        string res;
        for (int i = (int)stk.size() - 1; i >= 0; i--)
            res += mText.substr(stk[i]->begin, stk[i]->getLength());

        return res;
    }

private:
    int             mIntN;      // 
    vector<int>     mInts;      // 

    Node* allocNode() {
        Node* p = &mNodes[mNodeN++];
        p->init(mNodeN);
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

    int* allocInt(int val) {
        int* v = &mInts[mIntN++];
        *v = val;
        return v;
    }

    void setSuffixIndex(Node* node) {
        if (!node)
            return;

        if (node->isLeaf())
            node->suffixIndex = (int)mText.length() - node->depth - node->getLength();
        else {
            node->suffixIndex = -1;
            for (int i = 0; i <= mCharMax - mCharMin; i++) {
                if (node->hasChild(i))
                    setSuffixIndex(node->getChild(i));
            }
        }
    }

    void resetSuffixIndex(Node* node) {
        if (!node)
            return;

        node->suffixIndex = -1;
        for (int i = 0; i <= mCharMax - mCharMin; i++) {
            if (node->hasChild(i))
                resetSuffixIndex(node->getChild(i));
        }
    }
};
