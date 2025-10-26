#pragma once

#if defined(_MSC_VER)
#include <intrin.h>
#endif
#include <immintrin.h>

#include "blockAllocator.h"

//--- Array Mapped Compressed Trie ---------------------------------------------------------

template <int BaseChar = 'a'>
struct CompressedTrieAM {
    static const int AllocBlockSizeBit = 10;
    static constexpr int toIndex(char ch) {
        return ch - BaseChar;
    }

    static int popcnt(unsigned x) {
#if defined(_MSC_VER)
        return int(__popcnt(x));
#elif defined(__GNUC__)
        return __builtin_popcount(x);
#else
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return int(x & 0x0000003F);
#endif
    }

    struct Node {
        string          text;
        bool            terminal;
        unsigned        childSet;
        vector<Node*>   children;

        Node() {
            init();
        }

        void init() {
            terminal = false;
            childSet = 0;
            children.clear();
        }

        bool isLeaf() const {
            return childSet == 0;
        }

        int countChildren() const {
            return popcnt(childSet);
        }

        bool hasChild(int index) const {
            return (childSet & (1u << index)) != 0;
        }

        Node* getChild(int index) const {
            if ((childSet & (1u << index)) == 0)
                return nullptr;
            int idx = popcnt(childSet & ((1u << index) - 1u));
            return children[idx];
        }

        void setChild(int index, Node* node) {
            int idx = popcnt(childSet & ((1u << index) - 1u));
            if ((childSet & (1u << index)) != 0) {
                children[idx] = node;
            } else {
                children.insert(children.begin() + idx, node);
                childSet |= (1u << index);
            }
        }

        void resetChild(int index) {
            int idx = popcnt(childSet & ((1 << index) - 1u));
            if ((childSet & (1u << index)) != 0) {
                children.erase(children.begin() + idx);
                childSet &= ~(1u << index);
            }
        }

        Node* findFirstChild() const {
            if (!childSet)
                return nullptr;
            /*
            * ex) x = 00110100
            *     ~x  11001011
            * (x - 1) 00110011
            * -----------------
            *       & 00000011
            */
            return children[0];
        }

        void mergeFrom(Node* p) {
            text += p->text;
            terminal = p->terminal;
            childSet = p->childSet;
            children = std::move(p->children);
        }
    };

    Node root;

    CompressedTrieAM() {
        freeNodeList = nullptr;
    }

    void clear() {
        for (auto* p : root.children)
            deleteNode(p);
        root.init();
    }

    // return true if it's a new string.
    bool insert(const string& s) {
        if (s.empty())
            return false;

        Node* prev = nullptr;
        Node* curr = &root;
        for (int i = 0; i < int(s.length()); i += int(curr->text.length())) {
            int next = toIndex(s[i]);

            prev = curr;
            curr = curr->getChild(next);
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
            if (!curr->hasChild(next))
                break;
            curr = curr->getChild(next);

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

    Node* allocNode() {
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
        node->children.clear();
        node->children.push_back(freeNodeList);
        freeNodeList = node;
    }

    void deleteNode(Node* p) {
        if (!p)
            return;

        for (auto* t : p->children)
            deleteNode(t);
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
        auto* nextNode = node->getChild(next);
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
