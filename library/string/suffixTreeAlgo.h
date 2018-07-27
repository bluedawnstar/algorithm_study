#pragma once

#include "suffixTree.h"
#include "../tree/lcaSparseTable.h"

struct SuffixTreeAlgo {
    typedef SuffixTree::Node Node;

    SuffixTree&     tree;
    vector<int>     levels;     // of all nodes
    LcaSparseTable  sptLCA;     // of all nodes

    vector<Node*>   suffixes;   // of leaf nodes

    SuffixTreeAlgo(SuffixTree& tr) : tree(tr) {
        buildLcaTable();
        suffixes = getSuffixNodes();
    }

    //--- LCA ----------------------------------------------------

    Node* findLCA(const Node* left, const Node* right) const {
        int id = sptLCA.lca(left->id, right->id);
        return tree.getNode(id);
    }

    Node* findLCA(int leftSuffixId, int rightSuffixId) const {
        return findLCA(suffixes[leftSuffixId], suffixes[rightSuffixId]);
    }

    Node* findLCAWithNodeID(int leftNodeId, int rightNodeId) const {
        int id = sptLCA.lca(leftNodeId, rightNodeId);
        return tree.getNode(id);
    }

    //--- Counting substrings ------------------------------------

    string lcp(int leftSuffixId, int rightSuffixId) const {
        const Node* lca = findLCA(leftSuffixId, rightSuffixId);
        if (lca == &tree.mRoot)
            return string();
        return tree.getString(lca);
    }

    //--- Check Substring ----------------------------------------

    int countSubstrings(Node* p) const {
        if (!p)
            return 0;

        int res = (p != &tree.mRoot) ? p->getLength() : 0;
        for (auto* c : p->children)
            res += countSubstrings(c);
        return res;
    }

    long long countSubstrings() const {
        return countSubstrings(&tree.mRoot);
    }

    bool isSubstring(const string& s) const {
        return isSubstring(&s[0], int(s.length()));
    }

    bool isSubstring(const char* s, int N) const {
        return dfsCheckSubstring(&tree.mRoot, s, N, 0);
    }

    pair<Node*, int> search(const string& s) const {
        return search(&s[0], int(s.length()));
    }

    pair<Node*, int> search(const char* s, int N) const {
        pair<Node*, int> res(nullptr, 0);
        dfsSearch(res, &tree.mRoot, s, N, 0);
        return res;
    }

    //--- Repeated Substring -------------------------------------

    string findLongestRepeatedSubstring() const {
        int maxHeight = 0;
        int substrStartIndex = 0;

        dfsFindRepeatedString(&tree.mRoot, 0, maxHeight, substrStartIndex);

        if (maxHeight == 0)
            return string();
        return tree.mText.substr(substrStartIndex, maxHeight);
    }

    //--- Building Suffix Array ----------------------------------

    vector<int> buildSuffixArray() const {
        vector<int> res;
        res.reserve(tree.mText.length());
        dfsBuildSuffixArray(&tree.mRoot, res);
        return res;
    }

private:
    void dfsGetLevels(vector<int>& levels, Node* p, int currLevel) {
        levels[p->id] = currLevel;
        if (!p->isLeaf()) {
            for (auto* c : p->children)
                dfsGetLevels(levels, c, currLevel + 1);
        }
    }

    void buildLcaTable() {
        if (!tree.mText.empty() && tree.mText.back() != '$')
            tree.extend('$');   // make real suffix tree
        tree.setSuffixIndex();

        vector<int> parent(tree.mNodeN + 1);
        parent[0] = -1;
        for (int i = 0; i < tree.mNodeN; i++)
            parent[i + 1] = tree.mNodes[i].parent->id;

        levels.resize(tree.mNodeN + 1);
        dfsGetLevels(levels, &tree.mRoot, 0);

        sptLCA.build(parent, levels);
    }

    // 0: longest suffix, ..., n - 1: shortest suffix, n: '$'
    vector<Node*> getSuffixNodes() const {
        vector<Node*> res(tree.mText.length());

        for (int i = 0; i < tree.mNodeN; i++) {
            Node* p = (Node*)&tree.mNodes[i];
            if (p->isLeaf()) {
                int idx = int(tree.mText.length()) - p->depth - p->getLength();
                res[idx] = p;
            }
        }

        return res;
    }

    //--- Check Substring ----------------------------------------

    int dfsCountLeaf(const Node* node) const {
        if (!node)
            return 0;

        if (node->isLeaf())
            return 1;

        int res = 0;
        for (auto* c : node->children)
            res += dfsCountLeaf(c);
        return res;
    }

    // -1: not matched, 0: matched (not completed), 1: matched (completed)
    int traverseEdge(const char* s, int N, int idx, int start, int end) const {
        for (int k = start; k < end && idx < N; k++, idx++) {
            if (tree.mText[k] != s[idx])
                return -1;
        }
        return (idx >= N) ? 1 : 0;
    }

    bool dfsCheckSubstring(const Node* node, const char* s, int N, int idx) const {
        if (!node)
            return false;

        if (node != &tree.mRoot) {
            int res = traverseEdge(s, N, idx, node->begin, *(node->end));
            if (res != 0)
                return (res == 1);
        }

        idx += node->getLength();
        if (node->hasChild(tree.ch2i(s[idx])))
            return dfsCheckSubstring(node->getChild(tree.ch2i(s[idx])), s, N, idx);
        else
            return false;
    }


    int countLeaf(const Node* node) const {
        if (!node)
            return 0;
        return dfsCountLeaf(node);
    }

    // result: (matched node, the number of substring of the matched node)
    bool dfsSearch(pair<Node*, int>& result, const Node *node, const char* s, int N, int idx) const {
        if (!node)
            return false;

        if (node != &tree.mRoot) {
            int res = traverseEdge(s, N, idx, node->begin, *(node->end));
            if (res == -1)
                return false;
            else if (res == 1) {
                if (node->isLeaf()) {
                    result.first = (Node*)node;
                    result.second = 1;
                } else {
                    result.first = (Node*)node;
                    result.second = countLeaf(node);
                }
                return true;
            }
        }

        idx += node->getLength();
        if (node->hasChild(tree.ch2i(s[idx])))
            return dfsSearch(result, node->getChild(tree.ch2i(s[idx])), s, N, idx);
        else
            return false;
    }

    //--- Repeated Substring -------------------------------------

    void dfsFindRepeatedString(Node* node, int labelHeight, int& maxHeight, int& substrStartIndex) const {
        if (!node)
            return;

        if (!node->isLeaf()) {
            for (auto* c : node->children) {
                dfsFindRepeatedString(c, labelHeight + c->getLength(), maxHeight, substrStartIndex);
            }
        } else if (maxHeight < labelHeight - node->getLength()) {
            maxHeight = labelHeight - node->getLength();
            substrStartIndex = node->suffixIndex;
        }
    }

    //--- Building Suffix Array ----------------------------------

    void dfsBuildSuffixArray(const Node* node, vector<int>& suffixArray) const {
        if (!node)
            return;

        if (node->isLeaf())
            suffixArray.push_back(node->suffixIndex);
        else {
            for (auto* c : node->children)
                dfsBuildSuffixArray(c, suffixArray);
        }
    }
};
