#pragma once

#include "blockAllocator.h"

//--- Compressed Trie ---------------------------------------------------------

template <int R = 26, int BaseChar = 'a'>
struct CompressedTrie {
    static const int AllocBlockSizeBit = 10;
    static constexpr int toIndex(char ch) {
        return ch - BaseChar;
    }

    struct Node {
        string          text;
        bool            terminal;
        unsigned char   childrenCount;
        Node*           children[R];

        Node() {
            init();
        }

        void init() {
            terminal = false;
            childrenCount = 0;
            memset(children, 0, sizeof(children));
        }

        bool isLeaf() const {
            return childrenCount == 0;
        }

        int countChildren() const {
            return static_cast<int>(childrenCount);
        }

        void setChild(int index, Node* child) {
            if (!children[index])
                childrenCount++;
            children[index] = child;
        }

        void resetChild(int index) {
            if (children[index])
                childrenCount--;
            children[index] = nullptr;
        }

        Node* findFirstChild() const {
            for (int i = 0; i < R; i++) {
                if (children[i])
                    return children[i];
            }
            return nullptr;
        }

        void mergeFrom(const Node* p) {
            text += p->text;
            terminal = p->terminal;
            childrenCount = p->childrenCount;
            memcpy(children, p->children, sizeof(children));
        }
    };

    Node root;

    CompressedTrie() {
        freeNodeList = nullptr;
    }

    void clear() {
        for (int i = 0; i < R; i++) {
            if (root.children[i]) {
                deleteNode(root.children[i]);
                root.children[i] = nullptr;
            }
        }
    }

    // return true if it's a new string
    bool insert(const string& s) {
        if (s.empty())
            return false;

        Node* prev = nullptr;
        Node* curr = &root;
        for (int i = 0; i < int(s.length()); i += int(curr->text.length())) {
            int next = toIndex(s[i]);

            prev = curr;
            curr = curr->children[next];
            if (!curr) {
                curr = allocNode();
                curr->text = s.substr(i);
                prev->setChild(next, curr);
            } else {
                int j;
                for (j = 1; i + j < int(s.length()) && j < int(curr->text.length()); j++) {
                    if (s[i + j] != curr->text[j])
                        break;
                }
                if (j < int(curr->text.length())) {
                    curr = split(curr, j);
                    prev->setChild(next, curr);
                }
            }
        }

        if (curr->terminal)
            return false;

        curr->terminal = true;
        return true;
    }

    // delete a exactly matched word
    bool erase(const string& s) {
        if (s.empty())
            return false;
        return erase(&root, s, 0);
    }

    // return exactly matched word
    Node* find(const string& s) const {
        auto [totalLen, node, lastLen] = findLongestPrefix(s);
        if (totalLen != int(s.length()))
            return nullptr;
        return totalLen == s.length() && node->terminal ? node : nullptr;
    }

    // prefix matching
    // return (prefix_matching_length, last_node, matching_length_in_the_last_node)
    tuple<int, Node*, int> findLongestPrefix(const string& s) const {
        Node* curr = const_cast<Node*>(&root);
        if (s.empty())
            return make_tuple(0, curr, 0);

        int i, j;
        for (i = 0; i < int(s.length()); ) {
            int next = toIndex(s[i]);
            if (!curr->children[next])
                break;
            curr = curr->children[next];

            for (i++, j = 1; i < int(s.length()) && j < int(curr->text.length()); i++, j++) {
                if (s[i] != curr->text[j])
                    break;
            }
            if (j < int(curr->text.length()))
                break;
        }
        return make_tuple(i, curr, j);
    }

private:
    BlockAllocator<Node, AllocBlockSizeBit> allocator;
    Node* freeNodeList = nullptr;

    Node* allocNode(Node* parent = nullptr) {
        Node* p = nullptr;
        if (freeNodeList) {
            p = freeNodeList;
            freeNodeList = freeNodeList->children[0];
            p->init();
        } else {
            p = allocator.alloc();
            p->init();
        }

        return p;
    }

    void freeNode(Node* node) {
        node->children[0] = freeNodeList;
        freeNodeList = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (int i = 0; i < R; i++) {
            if (p->children[i])
                deleteNode(p->children[i]);
        }
        freeNode(p);
    }

    //---

    Node* split(Node* node, int position) {
        Node* newNode = allocNode();

        auto s = std::move(node->text);

        newNode->text = s.substr(0, position);
        newNode->setChild(toIndex(s[position]), node);
        node->text = s.substr(position);

        return newNode;
    }

    // return removed
    bool erase(Node* node, const string& s, int index) {
        if (index >= int(s.length())) {
            if (!node->terminal)
                return false;

            node->terminal = false;
            if (node->countChildren() == 1 && node != &root) {
                auto* childNode = node->findFirstChild();
                node->mergeFrom(childNode);
                freeNode(childNode);
            }
            return true;
        }

        int next = toIndex(s[index]);
        auto* nextNode = node->children[next];
        if (!nextNode)
            return false;

        int i, j;
        for (i = index + 1, j = 1; i < int(s.length()) && j < int(nextNode->text.length()); i++, j++) {
            if (s[i] != nextNode->text[j])
                break;
        }
        if (j < int(nextNode->text.length()))
            return false;

        auto res = erase(nextNode, s, i);
        if (res && !nextNode->terminal && nextNode->isLeaf()) {
            node->resetChild(next);
            freeNode(nextNode);
            if (!node->terminal && node->countChildren() == 1 && node != &root) {
                auto* childNode = node->findFirstChild();
                node->mergeFrom(childNode);
                freeNode(childNode);
            }
        }
        return res;
    }
};
