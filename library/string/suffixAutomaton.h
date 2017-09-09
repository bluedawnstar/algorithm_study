#pragma once

struct SuffixAutomaton {
    static const size_t MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }

    struct StateT {
        int len;                    // the length of the longest string in a class
        int suffixLink;             //  
        int next[MaxCharN];         // labeled next

        bool cloned;                //[extended field] this node was cloned
        int rightEnd;               //[extended field] right-end position in original string (>= 0)
    };
    int N;                          // it is not a string length
    vector<StateT> state;           // t0 is zeroth state, other states get numbers 1,2,3,...
    int last;

    SuffixAutomaton(int maxN) : state(maxN * 2, StateT{ 0, 0, { 0, } }) {
        init();
    }

    void init() {
        last = 0;
        state[0].len = 0;
        state[0].suffixLink = -1;
        state[0].rightEnd = 0;          //+
        state[0].cloned = false;        //+
        N = 1;
    }

    void extend(char c) {
        int i = ch2i(c);

        int cur = N++;
        state[cur].len = state[last].len + 1;

        // add edge from last nodes into new node
        int p;
        for (p = last; p >= 0 && !state[p].next[i]; p = state[p].suffixLink)
            state[p].next[i] = cur;

        if (p == -1)
            state[cur].suffixLink = 0;                            // set new node's suffix link to t0
        else {
            int q = state[p].next[i];
            if (state[p].len + 1 == state[q].len)
                state[cur].suffixLink = q;                        // we do not have to split q, just set the correct suffix link
            else {
                // we have to split, add q'
                int clone = N++;
                state[clone].len = state[p].len + 1;
                memcpy(state[clone].next, state[q].next, sizeof(state[q].next)); // copy next of q
                state[clone].suffixLink = state[q].suffixLink;          // copy suffix link of q

                state[clone].cloned = true;                             //+
                state[clone].rightEnd = state[q].rightEnd;              //+

                for (; p >= 0 && state[p].next[i] == q; p = state[p].suffixLink)
                    state[p].next[i] = clone;                           // fix last next of all nodes to have the same class to p

                state[q].suffixLink = state[cur].suffixLink = clone;    // set q and cur's suffix link to clone
            }
        }

        state[cur].cloned = false;                                      //+
        state[cur].rightEnd = state[cur].len - 1;                       //+

        last = cur;
    }

    void extend(const string& s) {
        for (int i = 0; i < (int)s.length(); i++)
            extend(s[i]);
    }

    void extend(const char s[], int len) {
        for (int i = 0; i < len; i++)
            extend(s[i]);
    }

    vector<int> getTerminals() const {
        vector<int> res;
        for (int u = last; u > 0; u = state[u].suffixLink)
            res.push_back(u);
        return move(res);
    }
};
