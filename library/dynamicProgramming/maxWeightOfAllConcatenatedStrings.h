#pragma once

// https://www.codechef.com/MAY20A/problems/TWOSTRS

/*
   - pattern[i] = { string, ... }, all patterns are pairwise distinct
   - weight[i] = weight of pattern[i]
   - 1 <= pattern[i] <= maxPatternLength

   find max weight-sum of found patterns on all concatenated strings A[...i] + B[j...]
      -1 <= i < length(A),  -1 means A not to be used
      -1 <= j < length(B),  -1 means B not to be used
*/
struct MaxWeightOfAllConcatenatedStrings {
    // O(sum(|patterns|) + |A|*|B|*max(|patterns|))
    static long long solve(const string& A, const string& B,
            const vector<string>& patterns, const vector<int>& weights,
            int maxPatternLength) {
        int nA = int(A.length());
        int nB = int(B.length());

        vector<long long> dpA(nA + 1);
        vector<long long> dpB(nB + 1);

        Trie trie;

        int repeatA = 0;
        int repeatB = 0;
        // process string A forward
        {
            for (int i = 0; i < int(patterns.size()); i++) {
                kmp(A, patterns[i], dpA, weights[i]);
                trie.insert(patterns[i], weights[i]);
            }
            vector<int> pi = prefixFunc(A);
            repeatA = nA - pi.back();
        }
        // process string B backward
        {
            auto revB = B;
            reverse(revB.begin(), revB.end());
            for (int i = 0; i < int(patterns.size()); i++) {
                string revPat = patterns[i];
                reverse(revPat.begin(), revPat.end());
                kmp(revB, revPat, dpB, weights[i]);
            }
            vector<int> pi = prefixFunc(revB);
            repeatB = nB - pi.back();
        }

        for (int i = 1; i <= nA; i++)
            dpA[i] += dpA[i - 1];
        for (int i = 1; i <= nB; i++)
            dpB[i] += dpB[i - 1];

        //--- create a weight table from substrings newly created on catenated strings

        int wH = min(nA, repeatA + maxPatternLength - 2);
        int wW = min(nB, repeatB + maxPatternLength - 2);
        vector<vector<int>> dpW(wH, vector<int>(wW));

        vector<Trie::Node*> nodes;
        for (int i = 0; i < wH; i++) {
            // extend to A[i]
            {
                vector<Trie::Node*> next;

                for (auto* it : nodes) {
                    auto p = trie.next(it, A[i]);
                    if (p)
                        next.push_back(p);
                }
                auto p = trie.mRoot.children[Trie::ch2i(A[i])];
                if (p)
                    next.push_back(p);

                swap(nodes, next);
            }

            // dpW[i][j] = calculate weight from substrings newly created on a catenated string (A[:i] + B[j:])
            for (int j = wW - 1; j >= 0; j--) {
                int w = 0;
                for (auto* p : nodes) {
                    for (int b = nB - j - 1; b < nB; b++) {
                        p = trie.next(p, B[b]);
                        if (!p)
                            break;
                        w += p->value;
                    }
                }
                dpW[i][j] = w;
            }
        }

        //--- find maximum weight

        long long res = 0;
        for (int i = 0; i <= nA; i++) {
            for (int j = 0; j <= nB; j++) {
                long long w = dpA[i] + dpB[nB - j];

                if (i > 0 && j < nB) {
                    int cI = i - 1;
                    int cJ = nB - j - 1;
                    if (cI >= wH)
                        cI = (cI - maxPatternLength + 2) % repeatA + maxPatternLength - 2;
                    if (cJ >= wW)
                        cJ = (cJ - maxPatternLength + 2) % repeatB + maxPatternLength - 2;
                    w += dpW[cI][cJ];
                }

                res = max(res, w);
            }
        }

        return res;
    }

private:
    static vector<int> prefixFunc(const string& s) {
        int n = int(s.length());
        vector<int> pi;
        pi.reserve(n);

        pi.push_back(0);

        int j = 0;
        for (int i = 1; i < n; i++) {
            while (j > 0 && s[j] != s[i])
                j = pi[j - 1];

            if (s[j] == s[i])
                j++;

            pi.push_back(j);
        }

        return pi;
    }

    static void kmp(const string& s, const string& pat, vector<long long>& dp, int value) {
        auto pi = prefixFunc(pat);

        int n = int(s.length());
        int m = int(pat.size());
        int j = 0;
        for (int i = 0; i < n; i++) {
            while (j > 0 && s[i] != pat[j])
                j = pi[j - 1];

            if (s[i] == pat[j]) {
                if (j == m - 1) {
                    dp[i + 1] += value;
                    j = pi[j];
                } else {
                    j++;
                }
            }
        }
    }

    struct Trie {
        static const size_t MaxCharN = 26;
        static const size_t AllocBlockSize = 1024;
        static constexpr int ch2i(char ch) {
            return ch - 'a';
        }

        struct Node {
            int     leafCount;
            Node*   parent;
            Node*   children[MaxCharN];

            int     value;

            void init(Node* parent = nullptr) {
                this->leafCount = 0;
                this->parent = parent;
                memset(this->children, 0, sizeof(this->children));
                this->value = 0;
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
        bool insert(const string& s, int value) {
            return insert(&s[0], int(s.length()), value);
        }

        // return true if it's a new string.
        bool insert(const char* s, int len, int value) {
            if (len <= 0)
                return false;

            Node* p = &mRoot;
            for (int i = 0; i < len; i++) {
                int idx = ch2i(s[i]);
                if (!p->children[idx])
                    p->children[idx] = allocNode(p);
                p = p->children[idx];
            }
            p->value = value;
            return ++p->leafCount == 1;
        }

        // return exactly matched word
        Node* find(const string& s) const {
            return find(&s[0], int(s.length()));
        }

        // return exactly matched word
        Node* find(const char* s, int len) const {
            if (len <= 0)
                return nullptr;

            Node* p = (Node*)&mRoot;
            for (int i = 0; i < len && p; i++) {
                int idx = ch2i(s[i]);
                p = p->children[idx];
            }
            return p;
        }

        Node* next(Node* p, char c) const {
            return p->children[ch2i(c)];
        }

    private:
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
};
