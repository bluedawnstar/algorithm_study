#pragma once

/*
    - builds a cliqueNode-tree from chordal graph
    - weight-product sum of independent subsets in an chordal graph, including empty set
    - the number of independent subsets in an chordal graph, including empty set

    http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.49.9635&rep=rep1&type=pdf
    https://dspace.jaist.ac.jp/dspace/bitstream/10119/3276/1/S-49.pdf
    https://discuss.codechef.com/t/palindeq-editorial/82148
*/
template <int mod = 998'244'353>
struct CliqueTree {
    vector<vector<int>> cliqueTree;     // a tree to have clique groups as nodes
                                        // .. cliqueTree[cliqueNode] = { cliqueNode_0, cliqueNode_1, ... }

    vector<vector<int>> cliqueGroup;    // cliqueGroup[cliqueNode] = { vertext IDs of original graph }
                                        // .. the set of maximal cliques
    vector<int> cliqueNode;             // cliqueNode[u] = clique group id to include u

    vector<int> perfectEliminationOrder;// perfect elimination order of original chordal graph

    CliqueTree() {
    }

    CliqueTree(const vector<vector<int>>& chordalGraph) {
        buildFromChordalGraph(chordalGraph);
    }

    // O((N + E)*logN)
    void buildFromChordalGraph(const vector<vector<int>>& chordalGraph) {
        int N = int(chordalGraph.size());

        cliqueGroup = vector<vector<int>>(1);
        cliqueNode = vector<int>(N, -1);
        perfectEliminationOrder = vector<int>(N);

        vector<pair<int, int>> cliqueTreeEdges;

        vector<int> mark(N);
        set<pair<int, int>> unused;             // (-mark[u], u)
        for (int i = 0; i < N; i++)
            unused.emplace(-mark[i], i);

        vector<vector<int>> who(N);             // 
        vector<int> last(N + 1, N);             // 

        cliqueNode.push_back(0);
        int prev = N + 1;
        for (int i = N - 1; i >= 0; i--) {
            int x = unused.begin()->second;
            unused.erase(unused.begin());

            if (mark[x] <= prev) {
                vector<int> cur = who[x];
                cur.push_back(x);
                cliqueGroup.push_back(cur);
                cliqueTreeEdges.emplace_back(cliqueNode[last[x]], int(cliqueGroup.size()) - 1);
            } else {
                cliqueGroup.back().push_back(x);
            }
            for (int y : chordalGraph[x]) {
                if (cliqueNode[y] != -1)
                    continue;
                who[y].push_back(x);

                // update
                unused.erase({ -mark[y], y });
                mark[y]++;
                unused.emplace(-mark[y], y);

                last[y] = x;
            }
            prev = mark[x];
            perfectEliminationOrder[i] = x;
            cliqueNode[x] = int(cliqueGroup.size()) - 1;
        }

        cliqueTree = vector<vector<int>>(cliqueGroup.size());
        for (auto it : cliqueTreeEdges) {
            cliqueTree[it.first].push_back(it.second);
            cliqueTree[it.second].push_back(it.first);
        }
    }

    /*
        weight-product sum of independent subsets in an chordal graph, including empty set

        -   SUM {    PROD F[x]    }
            all     x in a independent subset
         independent
           subsets

        O(NlogN)
    */
    int sumWeightProductOfIndependentSubsetsInChordalGraph(const vector<int>& W) {
        int N = int(cliqueGroup.size());

        vector<long long> noneDP(N);            // the number of independent subsets where each clique is not selected
        vector<vector<long long>> chosenDP(N);  // the number of independent subsets where each clique is selected
        vector<map<int, int>> rev(N);
        for (int i = 0; i < N; i++) {
            chosenDP[i].resize(cliqueGroup[i].size());
            for (int j = 0; j < int(cliqueGroup[i].size()); j++)
                rev[i][cliqueGroup[i][j]] = j;
        }

        function<void(int, int)> solve;
        solve = [&](int u, int parent) {
            set<int> setU, onlyU;
            for (int x : cliqueGroup[u]) {
                setU.insert(x);
                onlyU.insert(x);
            }

            noneDP[u] = 1ll;

            map<int, long long> dpV;
            vector<int> children;
            for (int v : cliqueTree[u]) {
                if (v == parent)
                    continue;
                children.push_back(v);
                solve(v, u);

                long long sum = noneDP[v];
                for (int i = 0; i < int(cliqueGroup[v].size()); i++) {
                    int x = cliqueGroup[v][i];
                    if (!setU.count(x)) {   // splitted cliques
                        sum += chosenDP[v][i];
                        if (sum >= mod)
                            sum -= mod;
                    } else {
                        onlyU.erase(x);     // overlapped cliques
                    }
                }
                // none[v] + SUM { chosen[v][x] } ,  Ci : the set of chordal nodes present in i-th clique
                //          x in {Cv - Cu}
                dpV[v] = sum;

                // none[u] = PROD { none[v] + SUM{ chosen[v][x] }
                //         v in children(u)   x in {Cv - Cu}
                noneDP[u] = noneDP[u] * sum % mod;
            }

            for (int x : cliqueGroup[u]) {
                if (onlyU.count(x)) {           // x in {Cu - Cv}
                    // chosen[u][x] = none[u] * W(x)
                    chosenDP[u][rev[u][x]] = noneDP[u] * W[x] % mod;
                } else {
                    long long invW = modInv(W[x]);
                    long long value = 1;
                    for (int v : children) {
                        if (rev[v].count(x))    // x in {Cu and Cv}
                            //  PROD { chosen[v][x] / W(x) }
                            // v in children[u]
                            // x not in Cv
                            value = value * chosenDP[v][rev[v][x]] % mod * invW % mod;
                        else                    // x in {Cv - Cu}
                            value = value * dpV[v] % mod;
                    }
                    // chosen[u][x] = W(x) * { PROD { chosen[v][x] / W(x) } } * { PROD { none[v] + SUM chosen[v][y] } }
                    //                        v in children[u]                  v in children[u]   y in {Cv - Cu}
                    //                        x not in Cv                       x not in Cv
                    chosenDP[u][rev[u][x]] = value * W[x] % mod;
                }
            }
        };

        solve(0, -1);
        long long res = noneDP[0];
        for (auto c : chosenDP[0]) {
            res += c;
            if (res >= mod)
                res -= mod;
        }
        return int(res);
    }

    // the number of independent subsets in an chordal graph, including empty set
    int countIndependentSubsetsInChordalGraph() {
        int N = int(cliqueGroup.size());

        vector<long long> noneDP(N);            // the number of independent subsets where each clique is not selected
        vector<vector<long long>> chosenDP(N);  // the number of independent subsets where each clique is selected
        vector<map<int, int>> rev(N);
        for (int i = 0; i < N; i++) {
            chosenDP[i].resize(cliqueGroup[i].size());
            for (int j = 0; j < int(cliqueGroup[i].size()); j++)
                rev[i][cliqueGroup[i][j]] = j;
        }

        function<void(int, int)> solve;
        solve = [&](int u, int parent) {
            set<int> setU, onlyU;
            for (int x : cliqueGroup[u]) {
                setU.insert(x);
                onlyU.insert(x);
            }

            noneDP[u] = 1ll;

            map<int, long long> dpV;
            vector<int> children;
            for (int v : cliqueTree[u]) {
                if (v == parent)
                    continue;
                children.push_back(v);
                solve(v, u);

                long long sum = noneDP[v];
                for (int i = 0; i < int(cliqueGroup[v].size()); i++) {
                    int x = cliqueGroup[v][i];
                    if (!setU.count(x)) {   // splitted cliques
                        sum += chosenDP[v][i];
                        if (sum >= mod)
                            sum -= mod;
                    } else {
                        onlyU.erase(x);     // overlapped cliques
                    }
                }
                // none[v] + SUM { chosen[v][x] } ,  Ci : the set of chordal nodes present in i-th clique
                //          x in {Cv - Cu}
                dpV[v] = sum;

                // none[u] = PROD { none[v] + SUM{ chosen[v][x] }
                //         v in children(u)   x in {Cv - Cu}
                noneDP[u] = noneDP[u] * sum % mod;
            }

            for (int x : cliqueGroup[u]) {
                if (onlyU.count(x)) {           // x in {Cu - Cv}
                    chosenDP[u][rev[u][x]] = noneDP[u];
                } else {
                    long long value = 1;
                    for (int v : children) {
                        if (rev[v].count(x))    // x in {Cu and Cv}
                            //  PROD { chosen[v][x] }
                            // v in children[u]
                            // x not in Cv
                            value = value * chosenDP[v][rev[v][x]] % mod;
                        else                    // x in {Cv - Cu}
                            value = value * dpV[v] % mod;
                    }
                    // chosen[u][x] = F(x) * { PROD { chosen[v][x] / F(x) } } * { PROD { none[v] + SUM chosen[v][y] } }
                    //                        v in children[u]                  v in children[u]   y in {Cv - Cu}
                    //                        x not in Cv                       x not in Cv
                    chosenDP[u][rev[u][x]] = value;
                }
            }
        };

        solve(0, -1);
        long long res = noneDP[0];
        for (auto c : chosenDP[0]) {
            res += c;
            if (res >= mod)
                res -= mod;
        }
        return int(res);
    }

private:
    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }

    static int modInv(int x) {
        return modPow(x, mod - 2);
    }
};
