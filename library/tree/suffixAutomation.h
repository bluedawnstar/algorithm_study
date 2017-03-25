#pragma once

struct SuffixAutomation {
    static const size_t MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }

    struct StateT {
        int len;                    // the length of the longest string in a class
        int suffixLink;             // 
        int edges[MaxCharN];        // labeled edges
    };
    int N;
    vector<StateT> state;           // t0 is zeroth state, other states get numbers 1,2,3,...
    int last;

    SuffixAutomation(int maxN) : state(maxN * 2, StateT{ 0, 0, { 0, } }) {
        init();
    }

    void init() {
        last = 0;
        state[0].len = 0;
        state[0].suffixLink = -1;
        N = 1;
    }

    void extend(char c) {
        int i = ch2i(c);

        int cur = N++;
        state[cur].len = state[last].len + 1;

        // add edge from last nodes into new node
        int p;
        for (p = last; p >= 0 && !state[p].edges[i]; p = state[p].suffixLink)
            state[p].edges[i] = cur;

        if (p == -1)
            state[cur].suffixLink = 0;                            // set new node's suffix link to t0
        else {
            int q = state[p].edges[i];
            if (state[p].len + 1 == state[q].len)
                state[cur].suffixLink = q;                        // we do not have to split q, just set the correct suffix link
            else {
                // we have to split, add q'
                int clone = N++;
                state[clone].len = state[p].len + 1;
                memcpy(state[clone].edges, state[q].edges, sizeof(state[q].edges)); // copy edges of q
                state[clone].suffixLink = state[q].suffixLink;          // copy suffix link of q

                for (; p >= 0 && state[p].edges[i] == q; p = state[p].suffixLink)
                    state[p].edges[i] = clone;                          // fix last edges of all nodes to have the same class to p

                state[q].suffixLink = state[cur].suffixLink = clone;    // set q and cur's suffix link to clone
            }
        }
        last = cur;
    }

    template <typename T>
    void extend(T s, int n) {
        for (int i = 0; i < n; i++)
            extend(s[i]);
    }
};
