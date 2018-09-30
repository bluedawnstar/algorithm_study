#pragma once

/*
 https://github.com/zimpha/algorithmic-library/blob/master/StringUtility/eerTree.cc
 https://arxiv.org/abs/1506.04862

 EERTREE: An Efficient Data Structure for Processing Palindromes in Strings
 
 Properties:
   1) Nodes and edges of an eerTree form two weakly connected components:
      the tree of odd (resp., of even) nodes rooted at rte (resp., at rto).

   2) The tree of even (resp., odd) nodes is precisely the trie of right
      halves of even-length palindromes (resp., the trie of right halves,
      ncluding the central symbol, of odd-length palindromes).

   3) Nodes and inverted suffix links of an eerTree form a tree with a
      loop at its root rto.

   4) After each add(c), the last NodeT corresponds to the longest suffix of the current string.

 Applications:
   1. for a given string S find a subpalindrome P maximizing the value
      |P|*occ(S, P), where occ(S, P) is the number of occurrences of P in S.
      After building the eertree, use nodes[p].link->occ += nodes[p].occ to enumerate each NodeT from the back to the front and calculate
      occ(S, P), then the normal traversal tree calculation is fine.

   2. for a string S, find the number of triples i, j, k such that
      1 ≤ i ≤ j < k ≤ |S| and the strings S[i..j], S[j+1..k] are palindromes.
      Construct two eertrees in reverse, and save the number of palindromes starting/ending at j, which is nodes[last].count.

   3. Multiple strings of eertree: Each time you build a string, clear the last NodeT and start adding a new string.
      3.1 Find the number of subpalindromes, common to all k given strings.
      3.2 Find the longest subpalindrome contained in all k given strings.
      3.3 For strings S and T find the number of palindromes P having more
          occurrences in S than in T .
      3.4 For strings S and T find the number of equal palindromes, i.e., of
          triples (i, j, k) such that S[i..i+k] = T [j..j+k] is a palindrome.

   4. Minimum palindrome division: Add a serial link, detailed reference paper
*/

struct eerTree {
    static const int INF = 1000000000;
    static const int MaxCharN = 26;

    static int ch2i(int c) { return c - 'a'; }

    struct NodeT {
        int start, end;     // 
        int len;            // length of substring
        int suffixLink;     // suffix link
        int next[MaxCharN];
        int count;          // the number of palindromic strings to be ended at the current node

        int occ;            // the occurrences of this NodeT in the whole string
        int serialLink;     // serial link: used in palindromic factorization
        int diff, dp;       // used in palindromic factorization

        NodeT() {
            memset(this, 0, sizeof(NodeT));
        }
    };
    
    string s;
    vector<NodeT> nodes;

    int nodeN;                  // node 1 - root with len -1, node 2 - root with len 0
    int lastSuffix;
    vector<int> suffixNodes;    // suffix nodes (index of string --> suffix node index)

    int oddRoot, evenRoot;      // two initial root nodes (odd root, even root)

    //--- result

    vector<int> minFactorization;           // 
    vector<pair<int, bool>> palCount;       // (the number of added palindrom strings, if new palindromic substring)

    explicit eerTree(int maxN) {
        init(maxN);
    }

    void init(int maxN) {
        s.clear();
        s.reserve(maxN);
        nodes = vector<NodeT>(maxN + 3);

        oddRoot = 1;
        evenRoot = 2;
        nodeN = 2;

        nodes[oddRoot].suffixLink = oddRoot;
        nodes[oddRoot].len = -1;
        nodes[evenRoot].suffixLink = oddRoot;
        nodes[evenRoot].len = 0;

        lastSuffix = evenRoot;
        suffixNodes.clear();
        suffixNodes.reserve(maxN);

        minFactorization.assign(maxN + 1, INF);
        minFactorization[0] = 0;
        palCount.assign(maxN, make_pair(0, false));
    }


    // return (the number of added palindrom strings, if new palindromic substring)
    pair<int, bool> extend(char ch) {
        auto res = add(ch);

        palCount[s.length() - 1] = res;
        factorize();

        suffixNodes.push_back(lastSuffix);

        return res;
    }

    void build(const char s[], int n) {
        for (int i = 0; i < n; i++)
            extend(s[i]);
    }

    void build(const string& s) {
        for (auto c : s)
            extend(c);
    }


    long long countPalindromes() const {
        long long res = 0;
        for (int i = 0; i < int(s.length()); i++)
            res += get<0>(palCount[i]);
        return res;
    }

    // result[i] = count(s, i + 1) - count(s, i)
    vector<int> getSuffixPalindromes() const {
        vector<int> res;
        res.reserve(s.length());
        for (int i = 0; i < int(s.length()); i++)
            res.push_back(get<0>(palCount[i]));
        return res;
    }


    int countMinFactorization() const {
        return minFactorization[s.length()];
    }

    // the number of minimal factorization in S[0, prefixLength-1]
    int countMinFactorization(int prefixLength) const {
        return minFactorization[prefixLength];
    }


    vector<string> getAnyMinFactorization() const {
        vector<string> res;

        int n = int(s.length());
        for (int p = lastSuffix; n > 0; ) {
            res.push_back(s.substr(nodes[p].start, nodes[p].len));
            n -= nodes[p].len;
            p = suffixNodes[n - 1];
        }
        reverse(res.begin(), res.end());
        return res;
    }

private:
    // return (the number of added palindrom strings, if new palindromic substring)
    pair<int, bool> add(char ch) {
        int pos = int(s.length());
        int cur = lastSuffix, curlen = 0;
        int let = ch2i(ch);

        s += ch;

        while (true) {
            curlen = nodes[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == ch)
                break;
            cur = nodes[cur].suffixLink;
        }
        if (nodes[cur].next[let]) {
            lastSuffix = nodes[cur].next[let];
            nodes[lastSuffix].occ++;
            return make_pair(nodes[lastSuffix].count, false);
        }

        lastSuffix = ++nodeN;
        nodes[cur].next[let] = nodeN;

        nodes[nodeN].len = nodes[cur].len + 2;
        nodes[nodeN].start = pos - nodes[nodeN].len + 1;
        nodes[nodeN].end = pos;

        if (nodes[nodeN].len == 1) {
            nodes[nodeN].suffixLink = evenRoot;
            nodes[nodeN].count = 1;
        } else {
            while (true) {
                cur = nodes[cur].suffixLink;
                curlen = nodes[cur].len;
                if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                    nodes[nodeN].suffixLink = nodes[cur].next[let];
                    break;
                }
            }
            nodes[nodeN].count = 1 + nodes[nodes[nodeN].suffixLink].count;
        }

        // used for palindromic factorization
        nodes[lastSuffix].diff = nodes[lastSuffix].len - nodes[nodes[lastSuffix].suffixLink].len;
        if (nodes[lastSuffix].diff == nodes[nodes[lastSuffix].suffixLink].diff)
            nodes[lastSuffix].serialLink = nodes[nodes[lastSuffix].suffixLink].serialLink;
        else
            nodes[lastSuffix].serialLink = nodes[lastSuffix].suffixLink;

        nodes[lastSuffix].occ++;
        return make_pair(nodes[lastSuffix].count, true);
    }

    void factorize() {
        int n = int(s.length());
        for (int p = lastSuffix; nodes[p].len; p = nodes[p].serialLink) {
            nodes[p].dp = minFactorization[n - nodes[nodes[p].serialLink].len - nodes[p].diff];
            if (nodes[p].diff == nodes[nodes[p].suffixLink].diff) {
                nodes[p].dp = min(nodes[p].dp, nodes[nodes[p].suffixLink].dp);
            }
            minFactorization[n] = min(minFactorization[n], nodes[p].dp + 1);
        }
    }
};
