#pragma once

/*
   https://www.codechef.com/problems/MAXTOPO

   DAG like reversed tree

          a
        ^ ^ ^
       /  |  \
      b   c   d
    ... ... ^ ^ ^
           /  |  \
          e   f   g
         ... ... ...

  1. the number of topological order
    1.1) formula

                 (subtree_size[u] - 1)!
        F(u) = --------------------------- * PROD F(v)
                PROD { subtree_size[v]! }   v in children-of-u
                v in children-of-u

             = (subtree_size[u] - 1)! * PROD { F(v) / subtree_size[v]! }
                                       v in children-of-u

                subtree_size[u]!
             = ------------------ * PROD { F(v) / subtree_size[v]! }
                subtree_size[u]    v in children-of-u

       ==>

                            N!
        F(root) = --------------------------
                   PROD { subtree_size[u] }
                  u in V

        ex)
              a
            / | \
           b  c  d
         ... ... | \
                 e  f
                ... ...

                        (N - 1)!
       F(a) = ------------------------------- * F(b) * F(c) * F(d)
              size(b)! * size(c)! * size(d)!

                        (N - 1)!                                  (size(d) - 1)!
            = -------------------------------- * F(b) * F(c) * --------------------- * F(e) * F(f)
               size(b)! * size(c)! * size(d)!                   size(e)! * size(f)!

                        (N - 1)!                                         1
            = -------------------------------- * F(b) * F(c) * --------------------- * F(e) * F(f)
               size(b)! * size(c)! * size(d)                    size(e)! * size(f)!

               ...

                                 (N - 1)!
            = -------------------------------------------------------
               size(b) * size(c) * size(d) * size(e) * size(f) * ...

                                    N!
            = -----------------------------------------------------------------
               size(a) * size(b) * size(c) * size(d) * size(e) * size(f) * ...


    1.2) property

              a
            / | \
           b  c  d
         ... ... | \
                 e  f
                ... ...

                        (N - 1)!
       G(a) = ------------------------------- * F(b) * F(c) * F(d)
              size(b)! * size(c)! * size(d)!

                        (N - 1)!                                  (size(d) - 1)!
            = -------------------------------- * F(b) * F(c) * --------------------- * F(e) * F(f)
               size(b)! * size(c)! * size(d)!                   size(e)! * size(f)!

                        (N - 1)!                                         1
            = -------------------------------- * F(b) * F(c) * --------------------- * F(e) * F(f)
               size(b)! * size(c)! * size(d)                    size(e)! * size(f)!



              d
            / | \
           a  e  f
         / | ... ...
        b  c
      ... ...

                       (N - 1)!
       G(d) = ------------------------------- * F(a) * F(e) * F(f)
              size(a)! * size(e)! * size(f)!

                       (N - 1)!                                   (size(a) - 1)!
            = -------------------------------- * F(e) * F(f) * --------------------- * F(b) * F(c)
               size(a)! * size(e)! * size(f)!                   size(b)! * size(c)!

                       (N - 1)!                                          1
            = -------------------------------- * F(e) * F(f) * --------------------- * F(b) * F(c)
               size(a) * size(e)! * size(f)!                    size(b)! * size(c)!


                     1
                -----------
       G(d)       size(a)         <--- when d is root
      ------ = -------------
       G(a)          1
                -----------
                  size(d)         <--- when a is root


                (N - size(a))
             = ----------------
                   size(a)        <--- when d is root


                   size(d)
             = ----------------
                (N - size(d))     <--- when a is root


      therefore if the root is being changed from a to d

             G(d)       size(d)
            ------ = -------------
             G(a)     N - size(d)

          it means...

            if G(d) > G(a), then size(d) > (N - size(d))

             G(d)       size(d)
            ------ = ------------- > 1
             G(a)     N - size(d)

                  <=>  size(d) > N - size(d)

                  <=>  size(d) > N / 2
*/
template <int mod = 1'000'000'007>
struct DAGLikeReversedTree {
    int N;

    vector<vector<int>> edges;
    vector<int> subtreeSize;
    int root;

    DAGLikeReversedTree() : N(0), root(-1) {
    }

    explicit DAGLikeReversedTree(int n) : N(0), root(-1) {
        init(n);
    }


    void init(int n) {
        prepare(n);

        this->N = n;
        edges = vector<vector<int>>(n);
        subtreeSize = vector<int>(N);
        root = -1;
    }

    void add(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    //---

    void build(int root) {
        this->root = root;
        dfsBuild(root, -1);
    }

    // relative order by the number of topological order when each node is final node (root node)
    // - return relative order
    // O(N)
    vector<int> relativeOrderByTopologicalOrderCount() const {
        vector<double> cntLog(N);
        double curr = 0;
        for (int u = 0; u < N; u++) {
            if (u != root)
                curr -= logTable[subtreeSize[u]];
        }

        function<void(int, int, double)> dfs;
        dfs = [&](int u, int parent, double curr) {
            cntLog[u] = curr;
            for (int v : edges[u]) {
                if (v == parent)
                    continue;
                dfs(v, u, curr + logTable[subtreeSize[v]] - logTable[N - subtreeSize[v]]);
            }
        };
        dfs(root, -1, curr);

        vector<int> order(N);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            if (fabs(cntLog[a] - cntLog[b]) > 1e-10)
                return cntLog[a] < cntLog[b];
            return a < b;
        });
        return order;
    }


    // the number of topological order when each node is final node (root node)
    // O(N)
    vector<int> countTopologicalOrder() const {
        vector<int> res(N);

        long long curr = factorial[N];
        for (int i = 0; i < N; i++)
            curr = curr * inverse[subtreeSize[i]] % mod;

        function<void(int, int, long long)> dfs;
        dfs = [&](int u, int parent, long long curr) {
            res[u] = int(curr);
            for (int v : edges[u]) {
                if (v == parent)
                    continue;
                dfs(v, u, curr * subtreeSize[v] % mod * inverse[N - subtreeSize[v]] % mod);
            }
        };
        dfs(root, -1, curr);

        return res;
    }

    // the number of topological order when 'root' is final node (root node)
    // O(N)
    int countTopologicalOrder(int root) const {
        function<pair<int, int>(int, int)> dfsProdSize;
        dfsProdSize = [&](int u, int parent) -> pair<int, int> {
            long long res = 1;
            int size = 1;
            for (auto v : edges[u]) {
                if (v == parent)
                    continue;
                auto t = dfsProdSize(v, u);
                size += t.second;
                res = res * t.first % mod;
            }
            return make_pair(int(res * inverse[size] % mod), size);
        };
        return int(1ll * factorial[N] * dfsProdSize(root, -1).first % mod);
    }

private:
    vector<int> factorial;
    vector<int> inverse;
    vector<double> logTable;

    void prepare(int n) {
        if (N < n) {
            factorial.resize(max(2, n + 1));
            inverse.resize(max(2, n + 1));
            logTable.resize(max(2, n + 1));
        }

        if (N == 0) {
            factorial[0] = 1;
            inverse[0] = 0;
            logTable[0] = 0;

            factorial[1] = 1;
            inverse[1] = 1;
            logTable[1] = 0;
        }
        for (int i = max(2, N); i <= n; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);
            logTable[i] = log(i);
        }
    }

    void dfsBuild(int u, int parent) {
        subtreeSize[u] = 1;
        for (auto v : edges[u]) {
            if (v == parent)
                continue;
            dfsBuild(v, u);
            subtreeSize[u] += subtreeSize[v];
        }
    }
};
