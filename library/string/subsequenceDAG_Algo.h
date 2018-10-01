#pragma once

struct SubsequenceDAGAlgo {
    int maxN;
    SubsequenceDAG& dag;

    SubsequenceDAGAlgo(SubsequenceDAG& dat, int maxN) : dag(dat), maxN(maxN),
        D(maxN * 2, 0), totLen(maxN * 2, 0) {
    }


    // 1. Number of different subsequences of a given string .
    vector<long long> D;
    long long countSubsequences(int u) {
        if (D[u])
            return D[u];

        long long res = 1;

        for (int i = 0; i < SubsequenceDAG::MaxCharN; i++) {
            if (dag.nodes[u].next[i])
                res += countSubsequences(dag.nodes[u].next[i]);
        }

        return D[u] = res;
    }

    long long countSubsequences() {
        fill(D.begin(), D.end(), 0);
        return countSubsequences(0); // include empty string
    }


    // 2. Total length of all distinct subsequences.
    vector<long long> totLen;
    long long totalLengthOfAllDistinctSubsequences(int u) {
        long long res = 0;

        if (totLen[u])
            return totLen[u];

        for (int i = 0; i < SubsequenceDAG::MaxCharN; i++)
            if (dag.nodes[u].next[i])
                //res += totalLengthOfAllDistinctSubsequences(dag.nodes[u].next[i]) + D[dag.nodes[u].next[i]];
                res += totalLengthOfAllDistinctSubsequences(dag.nodes[u].next[i]) + countSubsequences(dag.nodes[u].next[i]);

        return totLen[u] = res;
    }

    long long totalLengthOfAllDistinctSubsequences() {
        fill(D.begin(), D.end(), 0);
        countSubsequences();

        fill(totLen.begin(), totLen.end(), 0);
        return totalLengthOfAllDistinctSubsequences(0);
    }


    // 3. Lexicographically kth subsequence.
    // kth >= 1
    bool kthSubsequence(string& res, int kth, int u, int& path) {
        for (int i = 0; i < SubsequenceDAG::MaxCharN; i++) {
            if (dag.nodes[u].next[i]) {
                if (++path == kth || kthSubsequence(res, kth, dag.nodes[u].next[i], path)) {
                    res.push_back(SubsequenceDAG::i2ch(i));
                    return true;
                }
            }
        }
        return false;
    }

    // kth >= 0, excluding empty string
    string kthSubsequence(int kth) {
        string res;
        int path = 0;

        kthSubsequence(res, kth + 1, 0, path);

        reverse(res.begin(), res.end());
        return res;
    }


    // 4. Fast lexicographically kth subsequence.
    long long makeCountTable() {
        fill(D.begin(), D.end(), 0);
        return countSubsequences(0); // include empty string
    }

    // kth >= 1
    bool kthSubsequenceFast(string& res, long long kth, int u, long long& path) {
        for (int i = 0; i < SubsequenceDAG::MaxCharN; i++) {
            int v = dag.nodes[u].next[i];
            if (v) {
                if (path + D[v] < kth) {
                    path += D[v];
                    continue;
                }

                if (++path == kth || kthSubsequenceFast(res, kth, dag.nodes[u].next[i], path)) {
                    res.push_back(SubsequenceDAG::i2ch(i));
                    return true;
                }
            }
        }

        return false;
    }

    // kth >= 0, excluding empty string
    string kthSubsequenceFast(long long kth) {
        string res;
        long long path = 0;

        kthSubsequenceFast(res, kth + 1, 0, path);

        reverse(res.begin(), res.end());
        return res;
    }


    // 5. find all subsequences, including empty string
    vector<string> findAllSubsequences() {
        vector<string> res;
        dag.iterateAll([&res](int node, const string& s) {
            res.push_back(s);
        });
        return res;
    }
};
