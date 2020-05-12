#pragma once

//--- Aho-Corasick ------------------------------------------------------------

template <int MaxCharN = 26, int BaseChar = 'a'>
struct AhoCorasick {
    static constexpr int ch2i(char ch) {
        return ch - BaseChar;
    }

    struct Node {
        int             terminal;       // number of string to be ended at this node
        Node*           suffixLink;

        vector<int>     output;
        Node*           children[MaxCharN];

        void init() {
            this->terminal = -1;
            this->suffixLink = nullptr;
            output.clear();
            memset(children, 0, sizeof(children));
        }

        bool isTerminal() const {
            return terminal >= 0;
        }

        bool isEmpty() const {
            if (terminal >= 0)
                return false;
            for (int i = 0; i < MaxCharN; i++) {
                if (children[i])
                    return false;
            }
            return true;
        }
    };

    Node mRoot;

    AhoCorasick() {
        mRoot.init();
    }

    ~AhoCorasick() {
        clear();
    }

    void clear() {
        for (auto* p : nodes)
            delete p;
        nodes.clear();
        mRoot.init();
    }

    void insert(const char* s, int len, int id) {
        if (len <= 0)
            return;

        Node* p = &mRoot;
        for (int i = 0; i < len; i++) {
            int idx = ch2i(s[i]);
            if (!p->children[idx])
                p->children[idx] = allocNode();
            p = p->children[idx];
        }
        p->terminal = id;
    }

    // return true if it's a new string.
    void insert(const string& s, int id) {
        return insert(&s[0], int(s.length()), id);
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
            p = p->children[ch2i(s[i])];
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
            p = p->children[idx];
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

    //--- Aho-Corasick --------------------------------------------------------

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
                if (!here->children[c])
                    continue;

                Node* child = here->children[c];

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
        while (state != &mRoot && !state->children[chIdx])
            state = state->suffixLink;
        if (state->children[chIdx])
            state = state->children[chIdx];

        out.insert(out.end(), state->output.begin(), state->output.end());

        return state;
    }

    // out = { pattern_id, ... }
    const Node* nextAhoCorasick(const Node* currState, char ch, vector<int>& out) const {
        int chIdx = ch2i(ch);
        while (currState != &mRoot && !currState->children[chIdx])
            currState = currState->suffixLink;
        if (currState->children[chIdx])
            currState = currState->children[chIdx];

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
            while (state != &mRoot && !state->children[chIdx])
                state = state->suffixLink;
            if (state->children[chIdx])
                state = state->children[chIdx];

            for (int j = 0; j < int(state->output.size()); j++)
                ret.push_back(make_pair(i, state->output[j]));
        }
        return ret;
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
