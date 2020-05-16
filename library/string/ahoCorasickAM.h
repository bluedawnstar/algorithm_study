#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include "trie.h"

//--- Aho-Corasick ------------------------------------------------------------

template <int MaxCharN = 26, int BaseChar = 'a'>
struct AhoCorasickAM {
    static constexpr int ch2i(char ch) {
        return ch - BaseChar;
    }

    static int popcnt(unsigned x) {
#ifndef __GNUC__
        return int(__popcnt(x));
        /*
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return int(x & 0x0000003F);
        */
#else
        return __builtin_popcount(x);
#endif
    }

    struct Node {
        int             terminal;       // id of string to be ended at this node
        Node*           suffixLink;

        vector<int>     output;         // matched id list

        unsigned        childSet;
        vector<Node*>   children;

        void init() {
            this->terminal = -1;
            this->suffixLink = nullptr;
            this->childSet = 0;
            this->children.clear();
        }

        bool isTerminal() const {
            return terminal >= 0;
        }

        bool isEmpty() const {
            if (terminal >= 0)
                return false;
            return childSet == 0;
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
        }

        void eraseChild(int chIdx) {
            int idx = popcnt(childSet & ((1 << chIdx) - 1));
            if ((childSet & (1 << chIdx)) != 0) {
                //children[idx] = nullptr;
                children.erase(children.begin() + idx);
                childSet &= ~(1 << chIdx);
            }
        }
    };

    Node mRoot;

    AhoCorasickAM() {
        mRoot.init();
    }

    ~AhoCorasickAM() {
        clear();
    }

    void clear() {
        for (auto* p : mRoot.children) {
            deleteNode(p);
        }
    }

    void insert(const char* s, int len, int id) {
        if (len <= 0)
            return;

        Node* p = &mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            if (!p->hasChild(idx))
                p->setChild(idx, allocNode());
            p = p->getChild(idx);
        }
        p->terminal = id;
    }

    // return true if it's a new string.
    void insert(const string& s, int id) {
        return insert(&s[0], int(s.length()), id);
    }

    //--- trie

    // return exactly matched word
    Node* find(const string& s) const {
        return find(s.c_str(), int(s.length()));
    }

    // return exactly matched word
    Node* find(const char* s, int len) const {
        if (len <= 0)
            return nullptr;

        Node* p = (Node*)&mRoot;
        for (int i = 0; i < len && p; i++) {
            int idx = ch2i(s[i]);
            p = p->getChild(idx);
        }
        return (p && p->isTerminal()) ? p : nullptr;
    }

    // prefix matching
    // return (prefix_matching_length, string_id_if_exactly_matched)
    pair<int, int> search(const char* s, int len) const {
        if (len <= 0)
            return make_pair(0, -1);

        Node* p = (Node*)&mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            p = p->getChild(idx);
            if (!p)
                return make_pair(i, -1);
        }
        return make_pair(len, p->terminal);
    }

    // prefix matching
    // return (prefix_matching_length, string_id_if_exactly_matched)
    pair<int, int> search(const string& s) const {
        return search(&s[0], int(s.length()));
    }

    //--- Aho-Corasick

    // compute failure function
    // O(M), M = the sum of all pattern lengths
    void build() {
        queue<Node*> Q;

        mRoot.suffixLink = &mRoot;
        Q.push(&mRoot);

        while (!Q.empty()) {
            Node* here = Q.front();
            Q.pop();

            for (int c = 0; c < MaxCharN; c++) {
                if (!here->hasChild(c))
                    continue;

                Node* child = here->getChild(c);

                if (here == &mRoot)
                    child->suffixLink = &mRoot;
                else {
                    Node* t = here->suffixLink;
                    while (t != &mRoot && !t->hasChild(c))
                        t = t->suffixLink;
                    if (t->hasChild(c))
                        t = t->getChild(c);
                    child->suffixLink = t;
                }

                child->output = child->suffixLink->output;
                if (child->terminal != -1)
                    child->output.push_back(child->terminal);
                Q.push(child);
            }
        }
    }

    //---

    // out = { pattern_id, ... }
    const Node* firstAhoCorasick(char ch, vector<int>& out) const {
        const Node* state = &mRoot;

        int chIdx = ch2i(ch);
        while (state != &mRoot && !state->hasChild(chIdx))
            state = state->suffixLink;
        if (state->hasChild(chIdx))
            state = state->getChild(chIdx);

        out.insert(out.end(), state->output.begin(), state->output.end());

        return state;
    }

    // out = { pattern_id, ... }
    const Node* nextAhoCorasick(const Node* currState, char ch, vector<int>& out) const {
        int chIdx = ch2i(ch);
        while (currState != &mRoot && !currState->hasChild(chIdx))
            currState = currState->suffixLink;
        if (currState->hasChild(chIdx))
            currState = currState->getChild(chIdx);

        out.insert(out.end(), currState->output.begin(), currState->output.end());

        return currState;
    }

    //---

    // search all patterns in s
    // return (last_character_in_s, pattern_id)s
    // O(M + N + K), M = the sum of all pattern lengths, N = text length, K = the number of patterns in the text
    vector<pair<int, int>> doAhoCorasick(const string& s) const {
        return doAhoCorasick(&s[0], int(s.length()));
    }

    vector<pair<int, int>> doAhoCorasick(const char* s, int len) const {
        vector<pair<int, int>> ret;

        const Node* state = &mRoot;

        for (int i = 0; i < len; i++) {
            int chIdx = ch2i(s[i]);
            while (state != &mRoot && !state->hasChild(chIdx))
                state = state->suffixLink;
            if (state->hasChild(chIdx))
                state = state->getChild(chIdx);

            for (int j = 0; j < int(state->output.size()); j++)
                ret.push_back(make_pair(i, state->output[j]));
        }
        return ret;
    }

private:
    Node* allocNode() {
        Node* p = new Node();
        p->init();
        return p;
    }

    void deleteNode(Node* p) {
        if (p) {
            for (auto* t : p->children)
                deleteNode(t);
            delete p;
        }
    }
};
