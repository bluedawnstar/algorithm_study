#pragma once

template <int MaxCharN = 26, int BaseChar = 'a'>
struct AhoCorasickPatternWeightSum {
    static constexpr int ch2i(char ch) {
        return ch - BaseChar;
    }

    struct Node {
        Node*       suffixLink;

        long long   value;              // weight-sum of matched patterns in suffixes at this node
                                        // 0 means no matched string
        Node*       children[MaxCharN];

        void init() {
            this->suffixLink = nullptr;
            value = 0;
            memset(children, 0, sizeof(children));
        }
    };

    Node mRoot;

    AhoCorasickPatternWeightSum() {
        mRoot.init();
    }

    ~AhoCorasickPatternWeightSum() {
        clear();
    }

    void clear() {
        for (auto* p : nodes)
            delete p;
        nodes.clear();
        mRoot.init();
    }

    void insert(const char* s, int len, int weight) {
        if (len <= 0)
            return;

        Node* p = &mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            if (!p->children[idx])
                p->children[idx] = allocNode();
            p = p->children[idx];
        }
        p->value += weight;
    }

    // return true if it's a new string.
    void insert(const string& s, int weight) {
        return insert(s.c_str(), int(s.length()), weight);
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
        for (int i = 0; i < len && p; i++)
            p = p->children[ch2i(s[i])];

        return (p && p->value) ? p : nullptr;
    }

    // prefix matching
    // return (prefix_matching_length, weight_sum_of_matched_patterns)
    pair<int, long long> search(const char* s, int len) const {
        if (len <= 0)
            return make_pair(0, 0ll);

        Node* p = (Node*)&mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            p = p->children[idx];
            if (!p)
                return make_pair(i, 0ll);
        }
        return make_pair(len, p->value);
    }

    // prefix matching
    // return (prefix_matching_length, weight_sum_of_matched_patterns)
    pair<int, long long> search(const string& s) const {
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
                Node* child = here->children[c];
                if (!child)
                    continue;

                if (here == &mRoot)
                    child->suffixLink = &mRoot;
                else {
                    Node* t = here->suffixLink;
                    while (t != &mRoot && !t->children[c])
                        t = t->suffixLink;
                    if (t->children[c])
                        t = t->children[c];
                    child->suffixLink = t;
                }

                child->value += child->suffixLink->value;
                Q.push(child);
            }
        }
    }

    //---

    // out = { pattern_id, ... }
    const Node* firstAhoCorasick(char ch, long long& out) const {
        return nextAhoCorasick(&mRoot, ch, out);
    }

    // out = { pattern_id, ... }
    const Node* nextAhoCorasick(const Node* currState, char ch, long long& out) const {
        int chIdx = ch2i(ch);
        while (currState != &mRoot && !currState->children[chIdx])
            currState = currState->suffixLink;
        if (currState->children[chIdx])
            currState = currState->children[chIdx];

        out += currState->value;

        return currState;
    }

    //---

    // return weight-sum of all matched-patterns in s
    // O(M + N + K), M = the sum of all pattern lengths, N = text length, K = the number of patterns in the text
    long long doAhoCorasick(const string& s) const {
        return doAhoCorasick(&s[0], int(s.length()));
    }

    long long doAhoCorasick(const char* s, int len) const {
        long long res = 0;

        const Node* state = &mRoot;

        for (int i = 0; i < len; i++) {
            int chIdx = ch2i(s[i]);
            while (state != &mRoot && !state->children[chIdx])
                state = state->suffixLink;
            if (state->children[chIdx])
                state = state->children[chIdx];

            res += state->value;
        }
        return res;
    }

private:
    vector<Node*> nodes;

    Node* allocNode() {
        Node* p = new Node();
        nodes.push_back(p);

        p->init();
        return p;
    }
};
