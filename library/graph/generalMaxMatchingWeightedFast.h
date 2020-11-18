#pragma once

/*
  Maximum Weighted Matching in General Undirected Graphs

  - time : O(N*M*log(N))
  - space : O(N + M)

  Note: each vertex is 1-indexed internally.

  from https://uoj.ac/submission/385253
*/
template <typename T, T INF = T(1) << (sizeof(T) * 8 - 2)>
struct FastWeightedGeneralMaxMatching {
    // modifiable heap
    template <typename T>
    struct BinaryHeap {
        struct Node {
            T   value;
            int id;

            bool operator <(const Node& rhs) const {
                return value < rhs.value;
            }
        };

        int N;
        vector<Node> node;
        vector<int> index;

        BinaryHeap() {
        }

        explicit BinaryHeap(int N) : N(0), node(N + 1), index(N, 0) {
        }

        int size() const {
            return N;
        }

        bool empty() const {
            return N == 0;
        }

        void clear() {
            while (N > 0)
                index[node[N--].id] = 0;
        }

        T min() const {
            return node[1].value;
        }

        int argmin() const {
            return node[1].id;
        }

        T getValue(int id) const {
            return node[index[id]].value;
        }

        bool has(int id) const {
            return index[id] != 0;
        }

        //---

        void push(int id, T v) {
            // assert(!has(id));
            index[id] = ++N;
            node[N] = { v, id };
            siftUp(N);
        }

        void pop() {
            if (N > 0)
                pop(1);
        }

        void erase(int id) {
            if (index[id])
                pop(index[id]);
        }

        void update(int id, T v) {
            if (!has(id))
                return push(id, v);

            bool up = (v < node[index[id]].value);
            node[index[id]].value = v;
            if (up)
                siftUp(index[id]);
            else
                siftDown(index[id]);
        }

        void decreaseKey(int id, T v) {
            if (!has(id))
                return push(id, v);
            if (v < node[index[id]].value) {
                node[index[id]].value = v;
                siftUp(index[id]);
            }
        }

    private:
        void pop(int pos) {
            index[node[pos].id] = 0;
            if (pos == N) {
                --N;
                return;
            }

            bool up = (node[N].value < node[pos].value);
            node[pos] = node[N--];
            index[node[pos].id] = pos;
            if (up)
                siftUp(pos);
            else
                siftDown(pos);
        }

        void swapNode(int a, int b) {
            swap(node[a], node[b]);
            index[node[a].id] = a;
            index[node[b].id] = b;
        }

        void siftDown(int pos) {
            for (int k = pos, nk = k; 2 * k <= N; k = nk) {
                if (node[2 * k] < node[nk])
                    nk = 2 * k;
                if (2 * k + 1 <= N && node[2 * k + 1] < node[nk])
                    nk = 2 * k + 1;
                if (nk == k)
                    break;
                swapNode(k, nk);
            }
        }

        void siftUp(int pos) {
            for (int k = pos; k > 1 && node[k] < node[k >> 1]; k >>= 1)
                swapNode(k, k >> 1);
        }
    };

    // modifiable heaps
    template <typename KeyT>
    struct PairingHeaps {
        struct Node {
            KeyT key;
            int child, next, prev;  // "prev < 0" means the node is unused.

            Node() : prev(-1) {
            }

            Node(KeyT v) : key(v), child(0), next(0), prev(0) {
            }
        };

        vector<int> heap;
        vector<Node> node;

        PairingHeaps(int H, int N) : heap(H), node(N) {
            // It consists of `H` Pairing heaps.
            // Each heap-node ID can appear at most 1 time(s) among heaps and should be in [1, N).
        }

        void clear(int h) {
            if (heap[h]) {
                recClear(heap[h]);
                heap[h] = 0;
            }
        }

        void clearAll() {
            for (size_t i = 0; i < heap.size(); ++i)
                heap[i] = 0;
            for (size_t i = 0; i < node.size(); ++i)
                node[i] = Node();
        }

        bool empty(int h) const {
            return !heap[h];
        }

        bool used(int v) const {
            return node[v].prev >= 0;
        }

        KeyT min(int h) const {
            return node[heap[h]].key;
        }

        int argmin(int h) const {
            return heap[h];
        }

        void pop(int h) {
            // assert(!empty(h));
            erase(h, heap[h]);
        }

        void push(int h, int v, KeyT key) {
            // assert(!used(v));
            node[v] = Node(key);
            heap[h] = merge(heap[h], v);
        }

        void erase(int h, int v) {
            if (!used(v))
                return;
            int w = twoPassPairing(node[v].child);
            if (!node[v].prev)
                heap[h] = w;
            else {
                cut(v);
                heap[h] = merge(heap[h], w);
            }
            node[v].prev = -1;
        }

        void decreaseKey(int h, int v, KeyT key) {
            if (!used(v))
                return push(h, v, key);
            if (!node[v].prev)
                node[v].key = key;
            else {
                cut(v); node[v].key = key;
                heap[h] = merge(heap[h], v);
            }
        }

    private:
        void recClear(int v) {
            for (; v; v = node[v].next) {
                if (node[v].child)
                    recClear(node[v].child);
                node[v].prev = -1;
            }
        }

        void cut(int v) {
            auto& n = node[v];
            int pv = n.prev, nv = n.next;
            auto& pn = node[pv];
            if (pn.child == v)
                pn.child = nv;
            else
                pn.next = nv;
            node[nv].prev = pv;
            n.next = n.prev = 0;
        }

        int merge(int l, int r) {
            if (!l)
                return r;
            if (!r)
                return l;
            if (node[l].key > node[r].key)
                swap(l, r);

            int lc = node[r].next = node[l].child;
            node[l].child = node[lc].prev = r;
            return node[r].prev = l;
        }

        int twoPassPairing(int root) {
            if (!root)
                return 0;
            int a = root;
            root = 0;
            while (a) {
                int b = node[a].next, na = 0;
                node[a].prev = node[a].next = 0;
                if (b) {
                    na = node[b].next;
                    node[b].prev = node[b].next = 0;
                }
                a = merge(a, b);
                node[a].next = root;
                root = a;
                a = na;
            }

            int s = node[root].next;
            node[root].next = 0;
            while (s) {
                int t = node[s].next;
                node[s].next = 0;
                root = merge(root, s);
                s = t;
            }
            return root;
        }
    };

    // min heap
    template <typename T>
    struct PriorityQueue : public priority_queue<T, vector<T>, greater<T>> {
        PriorityQueue() {
        }

        PriorityQueue(int N) {
            this->c.reserve(N);
        }

        T min() {
            return this->top();
        }

        void clear() {
            this->c.clear();
        }
    };

    template <typename T>
    struct Queue {
        int qh, qt;
        vector<T> data;

        Queue() {
        }

        Queue(int N) : qh(0), qt(0), data(N) {
        }

        int size() const {
            return qt;
        }

        bool empty() const {
            return qh == qt;
        }

        void clear() {
            qh = qt = 0;
        }

        T operator [](int i) const {
            return data[i];
        }

        void enqueue(int u) {
            data[qt++] = u;
        }

        int dequeue() {
            return data[qh++];
        }
    };

    enum Label {
        kSeparated = -2,
        kInner = -1,
        kFree = 0,
        kOuter = 1
    };

public:
    // inputEdges = { (u, v, cost), ... }, 0 <= u, v < N
    FastWeightedGeneralMaxMatching(int N, const vector<tuple<int, int, T>>& inputEdges)
            : N(N), B((N - 1) / 2), S(N + B + 1), ofs(N + 2), edges(inputEdges.size() * 2),
              heap2(S), heap2s(S, S), heap3(int(edges.size())), heap4(S) {
        for (auto& e : inputEdges) {
            ofs[get<0>(e) + 1 + 1]++;
            ofs[get<1>(e) + 1 + 1]++;
        }
        for (int i = 1; i <= N + 1; ++i)
            ofs[i] += ofs[i - 1];
        for (auto& e : inputEdges) {
            edges[ofs[get<0>(e) + 1]++] = { get<1>(e) + 1, get<2>(e) * 2 };
            edges[ofs[get<1>(e) + 1]++] = { get<0>(e) + 1, get<2>(e) * 2 };
        }
        for (int i = N + 1; i > 0; --i)
            ofs[i] = ofs[i - 1];
        ofs[0] = 0;
    }

    // return (weight-sum, mate vector),  -1 <= mate[i] < N
    pair<T, vector<int>> calcMaxWeightedMatching(bool initMatching = false) {
        initialize();
        setPotential();
        if (initMatching)
            findMaxMatching();

        for (int u = 1; u <= N; ++u) {
            if (!mate[u])
                doEdmondsSearch(u);
        }

        pair<T, vector<int>> res;
        res.first = computeOptimalValue();

        res.second.reserve(N);
        for (int i = 1; i <= N; i++)
            res.second.push_back(mate[i] - 1);

        return res;
    }

private:
    struct Node {
        struct NodeLink {
            int b, v;
        };

        int parent, size;
        NodeLink link[2];

        Node() {
        }
        
        Node(int u) : parent(0), size(1) {
            link[0] = link[1] = { u, u };
        }

        int nextV() const {
            return link[0].v;
        }

        int nextB() const {
            return link[0].b;
        }

        int prevV() const {
            return link[1].v;
        }

        int prevB() const {
            return link[1].b;
        }
    };

    struct EdgeEvent {
        T time;
        int from, to;

        EdgeEvent() {
        }

        EdgeEvent(T time, int from, int to) : time(time), from(from), to(to) {
        }

        bool operator > (const EdgeEvent& rhs) const {
            return time > rhs.time;
        }

        bool operator < (const EdgeEvent& rhs) const {
            return time < rhs.time;
        }
    };

    T computeOptimalValue() const {
        T res = 0;
        for (int u = 1; u <= N; ++u) {
            if (mate[u] > u) {
                T maxCost = 0;
                for (int eid = ofs[u]; eid < ofs[u + 1]; ++eid) {
                    if (edges[eid].first == mate[u])
                        maxCost = max(maxCost, edges[eid].second);
                }
                res += maxCost;
            }
        }
        return res >> 1;
    }

    T reducedCost(int u, int v, const pair<int, T>& e) const {
        return T(potential[u]) + potential[v] - e.second;
    }

    void rematch(int v, int w) {
        int t = mate[v];
        mate[v] = w;
        if (mate[t] != v)
            return;
        if (link[v].second == surface[link[v].second]) {
            mate[t] = link[v].first;
            rematch(mate[t], t);
        } else {
            int x = link[v].first;
            int y = link[v].second;
            rematch(x, y);
            rematch(y, x);
        }
    }

    void fixMateAndBase(int b) {
        if (b <= N)
            return;
        int bv = base[b];
        int mv = node[bv].link[0].v;
        int bmv = node[bv].link[0].b;
        int d = (node[bmv].link[1].v == mate[mv]) ? 0 : 1;
        while (true) {
            int mv = node[bv].link[d].v;
            int bmv = node[bv].link[d].b;
            if (node[bmv].link[1 ^ d].v != mate[mv])
                break;
            fixMateAndBase(bv);
            fixMateAndBase(bmv);
            bv = node[bmv].link[d].b;
        }
        base[b] = bv;
        fixMateAndBase(base[b]);
        mate[b] = mate[bv];
    }

    void resetTime() {
        currentTime = 0;
        event1 = { INF, 0 };
    }

    void resetBlossom(int b) {
        label[b] = kFree;
        link[b].first = 0;
        slack[b] = INF;
        lazy[b] = 0;
    }

    void resetAll() {
        label[0] = kFree;
        link[0].first = 0;
        for (int v = 1; v <= N; ++v) { // should be optimized for sparse graphs.
            if (label[v] == kOuter)
                potential[v] -= currentTime;
            else {
                int bv = surface[v];
                potential[v] += lazy[bv];
                if (label[bv] == kInner)
                    potential[v] += currentTime - timeCreated[bv];
            }
            resetBlossom(v);
        }
        for (int b = N + 1, r = B - unusedBidIdx; r > 0 && b < S; ++b) {
            if (base[b] != b) {
                if (surface[b] == b) {
                    fixMateAndBase(b);
                    if (label[b] == kOuter)
                        potential[b] += (currentTime - timeCreated[b]) << 1;
                    else if (label[b] == kInner)
                        fixBlossomPotential<kInner>(b);
                    else
                        fixBlossomPotential<kFree>(b);
                }
                heap2s.clear(b);
                resetBlossom(b);
                --r;
            }
        }

        que.clear();
        resetTime();
        heap2.clear();
        heap3.clear();
        heap4.clear();
    }

    void doEdmondsSearch(int root) {
        if (potential[root] == 0)
            return;
        linkBlossom(surface[root], { 0, 0 });
        pushOuterAndFixPotentials(surface[root], 0);
        for (bool augmented = false; !augmented; ) {
            augmented = augment(root);
            if (augmented)
                break;
            augmented = adjustDualVariables(root);
        }
        resetAll();
    }

    template <Label Lab>
    T fixBlossomPotential(int b) {
        // return the amount.
        // (if v is an atom, the potential[v] will not be changed.)
        T d = lazy[b];
        lazy[b] = 0;
        if (Lab == kInner) {
            T dt = currentTime - timeCreated[b];
            if (b > N)
                potential[b] -= dt << 1;
            d += dt;
        }
        return d;
    }

    template <Label Lab>
    inline void updateHeap2(int x, int y, int by, T t) {
        if (t >= slack[y])
            return;
        slack[y] = t;
        bestFrom[y] = x;
        if (y == by) {
            if (Lab != kInner)
                heap2.decreaseKey(y, EdgeEvent(t + lazy[y], x, y));
        } else {
            int gy = group[y];
            if (gy != y) {
                if (t >= slack[gy])
                    return;
                slack[gy] = t;
            }
            heap2s.decreaseKey(by, gy, EdgeEvent(t, x, y));
            if (Lab == kInner)
                return;
            EdgeEvent m = heap2s.min(by);
            heap2.decreaseKey(by, EdgeEvent(m.time + lazy[by], m.from, m.to));
        }
    }

    void activateHeap2Node(int b) {
        if (b <= N) {
            if (slack[b] < INF)
                heap2.push(b, EdgeEvent(slack[b] + lazy[b], bestFrom[b], b));
        } else {
            if (heap2s.empty(b))
                return;
            EdgeEvent m = heap2s.min(b);
            heap2.push(b, EdgeEvent(m.time + lazy[b], m.from, m.to));
        }
    }

    void swapBlossom(int a, int b) {
        // Assume that `b` is a maximal blossom.
        swap(base[a], base[b]);
        if (base[a] == a)
            base[a] = b;
        swap(heavy[a], heavy[b]);
        if (heavy[a] == a)
            heavy[a] = b;
        swap(link[a], link[b]);
        swap(mate[a], mate[b]);
        swap(potential[a], potential[b]);
        swap(lazy[a], lazy[b]);
        swap(timeCreated[a], timeCreated[b]);
        for (int d = 0; d < 2; ++d)
            node[node[a].link[d].b].link[1 ^ d].b = b;
        swap(node[a], node[b]);
    }

    void setSurfaceAndGroup(int b, int sf, int g) {
        surface[b] = sf;
        group[b] = g;
        if (b <= N)
            return;
        for (int bb = base[b]; surface[bb] != sf; bb = node[bb].nextB()) {
            setSurfaceAndGroup(bb, sf, g);
        }
    }

    void mergeSmallerBlossoms(int bid) {
        int lb = bid;
        int largestSize = 1;
        for (int beta = base[bid], b = beta; ;) {
            if (node[b].size > largestSize) {
                largestSize = node[b].size;
                lb = b;
            }
            b = node[b].nextB();
            if (b == beta)
                break;
        }
        for (int beta = base[bid], b = beta; ;) {
            if (b != lb)
                setSurfaceAndGroup(b, lb, b);
            b = node[b].nextB();
            if (b == beta)
                break;
        }
        group[lb] = lb;
        if (largestSize > 1) {
            surface[bid] = heavy[bid] = lb;
            swapBlossom(lb, bid);
        } else {
            heavy[bid] = 0;
        }
    }

    void contract(int x, int y, int eid) {
        int bx = surface[x];
        int by = surface[y];
        assert(bx != by);
        const int h = -(eid + 1);
        link[surface[mate[bx]]].first = link[surface[mate[by]]].first = h;

        int lca = -1;
        while (1) {
            if (mate[by] != 0)
                swap(bx, by);
            bx = lca = surface[link[bx].first];
            if (link[surface[mate[bx]]].first == h)
                break;
            link[surface[mate[bx]]].first = h;
        }

        const int bid = unusedBid[--unusedBidIdx];
        assert(unusedBidIdx >= 0);
        int treeSize = 0;
        for (int d = 0; d < 2; ++d) {
            for (int bv = surface[x]; bv != lca; ) {
                int mv = mate[bv];
                int bmv = surface[mv];
                int v = mate[mv];
                int f = link[v].first;
                int t = link[v].second;
                treeSize += node[bv].size + node[bmv].size;
                link[mv] = { x, y };

                if (bv > N)
                    potential[bv] += (currentTime - timeCreated[bv]) << 1;
                if (bmv > N)
                    heap4.erase(bmv);
                pushOuterAndFixPotentials(bmv, fixBlossomPotential<kInner>(bmv));

                node[bv].link[d] = { bmv, mv };
                node[bmv].link[1 ^ d] = { bv, v };
                node[bmv].link[d] = { bv = surface[f], f };
                node[bv].link[1 ^ d] = { bmv, t };
            }
            node[surface[x]].link[1 ^ d] = { surface[y], y };
            swap(x, y);
        }
        if (lca > N)
            potential[lca] += (currentTime - timeCreated[lca]) << 1;
        node[bid].size = treeSize + node[lca].size;
        base[bid] = lca;
        link[bid] = link[lca];
        mate[bid] = mate[lca];
        label[bid] = kOuter;
        surface[bid] = bid;
        timeCreated[bid] = currentTime;
        potential[bid] = 0;
        lazy[bid] = 0;

        mergeSmallerBlossoms(bid);  // O(N*logN) time / Edmonds search
    }

    void linkBlossom(int v, pair<int, int> lnk) {
        link[v] = { lnk.first, lnk.second };
        if (v <= N)
            return;
        int b = base[v];
        linkBlossom(b, lnk);
        int pb = node[b].prevB();
        lnk = { node[pb].nextV(), node[b].prevV() };
        for (int bv = b; ; ) {
            int bw = node[bv].nextB();
            if (bw == b)
                break;
            linkBlossom(bw, lnk);
            pair<int, int> nl = { node[bw].prevV(), node[bv].nextV() };
            bv = node[bw].nextB();
            linkBlossom(bv, nl);
        }
    }

    void pushOuterAndFixPotentials(int v, T d) {
        label[v] = kOuter;
        if (v > N) {
            for (int b = base[v]; label[b] != kOuter; b = node[b].nextB()) {
                pushOuterAndFixPotentials(b, d);
            }
        } else {
            potential[v] += currentTime + d;
            if (potential[v] < event1.first)
                event1 = { potential[v], v };
            que.enqueue(v);
        }
    }

    bool grow(int root, int x, int y) {
        int by = surface[y];
        bool visited = (label[by] != kFree);
        if (!visited)
            linkBlossom(by, { 0, 0 });
        label[by] = kInner;
        timeCreated[by] = currentTime;
        heap2.erase(by);
        if (y != by)
            heap4.update(by, currentTime + (potential[by] >> 1));
        int z = mate[by];
        if (z == 0 && by != surface[root]) {
            rematch(x, y);
            rematch(y, x);
            return true;
        }
        int bz = surface[z];
        if (!visited)
            linkBlossom(bz, { x, y });
        else
            link[bz] = link[z] = { x, y };
        pushOuterAndFixPotentials(bz, fixBlossomPotential<kFree>(bz));
        timeCreated[bz] = currentTime;
        heap2.erase(bz);
        return false;
    }

    void freeBlossom(int bid) {
        unusedBid[unusedBidIdx++] = bid;
        base[bid] = bid;
    }

    int recalculateMinimumSlack(int b, int g) {
        // Return the destination of the best edge of blossom `g`.
        if (b <= N) {
            if (slack[b] >= slack[g])
                return 0;
            slack[g] = slack[b];
            bestFrom[g] = bestFrom[b];
            return b;
        }
        int v = 0;
        for (int beta = base[b], bb = beta; ; ) {
            int w = recalculateMinimumSlack(bb, g);
            if (w != 0)
                v = w;
            bb = node[bb].nextB();
            if (bb == beta)
                break;
        }
        return v;
    }

    void constructSmallerComponents(int b, int sf, int g) {
        surface[b] = sf;
        group[b] = g;   // `group[b] = g` is unneeded.
        if (b <= N)
            return;
        for (int bb = base[b]; surface[bb] != sf; bb = node[bb].nextB()) {
            if (bb == heavy[b]) {
                constructSmallerComponents(bb, sf, g);
            } else {
                setSurfaceAndGroup(bb, sf, bb);
                int to = 0;
                if (bb > N) {
                    slack[bb] = INF;
                    to = recalculateMinimumSlack(bb, bb);
                } else if (slack[bb] < INF) {
                    to = bb;
                }
                if (to > 0)
                    heap2s.push(sf, bb, EdgeEvent(slack[bb], bestFrom[bb], to));
            }
        }
    }

    void moveToLargestBlossom(int bid) {
        const int h = heavy[bid];
        T d = (currentTime - timeCreated[bid]) + lazy[bid];
        lazy[bid] = 0;
        for (int beta = base[bid], b = beta; ;) {
            timeCreated[b] = currentTime;
            lazy[b] = d;
            if (b != h) {
                constructSmallerComponents(b, b, b);
                heap2s.erase(bid, b);
            }
            b = node[b].nextB();
            if (b == beta)
                break;
        }
        if (h > 0) {
            swapBlossom(h, bid);
            bid = h;
        }
        freeBlossom(bid);
    }

    void expand(int bid) {
        int mv = mate[base[bid]];
        moveToLargestBlossom(bid); // O(n log n) time / Edmonds search
        pair<int, int> oldLink = link[mv];
        int oldBase = surface[mate[mv]];
        int root = surface[oldLink.second];
        int d = (mate[root] == node[root].link[0].v) ? 1 : 0;
        for (int b = node[oldBase].link[d ^ 1].b; b != root; ) {
            label[b] = kSeparated;
            activateHeap2Node(b);
            b = node[b].link[d ^ 1].b;

            label[b] = kSeparated;
            activateHeap2Node(b);
            b = node[b].link[d ^ 1].b;
        }
        for (int b = oldBase; ; b = node[b].link[d].b) {
            label[b] = kInner;
            int nb = node[b].link[d].b;
            if (b == root)
                link[mate[b]] = oldLink;
            else
                link[mate[b]] = { node[b].link[d].v, node[nb].link[d ^ 1].v };
            link[surface[mate[b]]] = link[mate[b]]; // fix tree links
            if (b > N) {
                if (potential[b] == 0)
                    expand(b);
                else
                    heap4.push(b, currentTime + (potential[b] >> 1));
            }
            if (b == root)
                break;
            pushOuterAndFixPotentials(nb, fixBlossomPotential<kInner>(b = nb));
        }
    }

    bool augment(int root) {
        // Return true if an augmenting path is found.
        while (!que.empty()) {
            int x = que.dequeue();
            int bx = surface[x];
            if (potential[x] == currentTime) {
                if (x != root)
                    rematch(x, 0);
                return true;
            }
            for (int eid = ofs[x]; eid < ofs[x + 1]; ++eid) {
                auto& e = edges[eid];
                int y = e.first;
                int by = surface[y];
                if (bx == by)
                    continue;
                Label l = label[by];
                if (l == kOuter) {
                    T t = reducedCost(x, y, e) >> 1;    // < 2 * INF
                    if (t == currentTime) {
                        contract(x, y, eid);
                        bx = surface[x];
                    } else if (t < event1.first) {
                        heap3.emplace(t, x, eid);
                    }
                } else {
                    T t = reducedCost(x, y, e);         // < 3 * INF
                    if (t >= INF)
                        continue;
                    if (l != kInner) {
                        if (T(t) + lazy[by] == currentTime) {
                            if (grow(root, x, y))
                                return true;
                        } else {
                            updateHeap2<kFree>(x, y, by, t);
                        }
                    } else {
                        if (mate[x] != y)
                            updateHeap2<kInner>(x, y, by, t);
                    }
                }
            }
        }
        return false;
    }

    bool adjustDualVariables(int root) {
        // delta1 : rematch
        T time1 = event1.first;

        // delta2 : grow
        T time2 = INF;
        if (!heap2.empty())
            time2 = heap2.min().time;

        // delta3 : contract : O(m log n) time / Edmonds search [ bottleneck (?) ]
        T time3 = INF;
        while (!heap3.empty()) {
            EdgeEvent e = heap3.min();
            int x = e.from;
            int y = edges[e.to].first;  // e.to is some edge id.
            if (surface[x] != surface[y]) {
                time3 = e.time;
                break;
            } else {
                heap3.pop();
            }
        }

        // delta4 : expand
        T time4 = INF;
        if (!heap4.empty())
            time4 = heap4.min();

        // -- events --
        T time_next = min(min(time1, time2), min(time3, time4));
        assert(currentTime <= time_next && time_next < INF);
        currentTime = time_next;

        if (currentTime == event1.first) {
            int x = event1.second;
            if (x != root)
                rematch(x, 0);
            return true;
        }
        while (!heap2.empty() && heap2.min().time == currentTime) {
            int x = heap2.min().from;
            int y = heap2.min().to;
            if (grow(root, x, y))
                return true;        // `grow` function will call `heap2.erase(by)`.
        }
        while (!heap3.empty() && heap3.min().time == currentTime) {
            int x = heap3.min().from;
            int eid = heap3.min().to;
            int y = edges[eid].first;
            heap3.pop();
            if (surface[x] == surface[y])
                continue;
            contract(x, y, eid);
        }
        while (!heap4.empty() && heap4.min() == currentTime) {
            int b = heap4.argmin();
            heap4.pop();
            expand(b);
        }
        return false;
    }

private:
    void initialize() {
        que = Queue<int>(N);
        mate.assign(S, 0);
        link.assign(S, { 0, 0 });
        label.assign(S, kFree);
        base.resize(S);
        iota(base.begin(), base.end(), 0);
        surface.resize(S);
        iota(surface.begin(), surface.end(), 0);

        potential.resize(S);
        node.resize(S);
        for (int b = 1; b < S; ++b)
            node[b] = Node(b);

        unusedBid.resize(B);
        for (int i = 0; i < B; ++i)
            unusedBid[i] = N + B - i;
        unusedBidIdx = B;

        // for O(N*M*logN) implementation
        resetTime();
        timeCreated.resize(S);
        slack.assign(S, INF);
        bestFrom.assign(S, 0);
        heavy.assign(S, 0);
        lazy.assign(S, 0);
        group.resize(S);
        iota(group.begin(), group.end(), 0);
    }

    void setPotential() {
        for (int u = 1; u <= N; ++u) {
            T maxCost = 0;
            for (int eid = ofs[u]; eid < ofs[u + 1]; ++eid)
                maxCost = max(maxCost, edges[eid].second);
            potential[u] = maxCost >> 1;
        }
    }

    void findMaxMatching() {
        // find a max matching naively.
        for (int u = 1; u <= N; ++u) {
            if (!mate[u]) {
                for (int eid = ofs[u]; eid < ofs[u + 1]; ++eid) {
                    auto& e = edges[eid];
                    int v = e.first;
                    if (mate[v] > 0 || reducedCost(u, v, e) > 0)
                        continue;
                    mate[u] = v;
                    mate[v] = u;
                    break;
                }
            }
        }
    }

private:
    int N, B, S;                        // N = |V|, B = (|V| - 1) / 2, S = N + B + 1
    vector<pair<int, T>>    edges;      // edges[u] = { (v, cost), ... }

    vector<int>             ofs;

    Queue<int>              que;
    vector<int>             mate, surface, base;
    vector<pair<int, int>>   link;       // { (from, to), ... }
    vector<Label>           label;
    vector<T>               potential;

    vector<int>             unusedBid;
    int                     unusedBidIdx;
    vector<Node>            node;

    // for O(N*M*logN) implementation
    vector<int>             heavy, group;
    vector<T>               timeCreated, lazy, slack;
    vector<int>             bestFrom;

    T                       currentTime;
    pair<T, int>             event1;    // (time, id)
    BinaryHeap<EdgeEvent>    heap2;
    PairingHeaps<EdgeEvent>  heap2s;
    PriorityQueue<EdgeEvent> heap3;
    BinaryHeap<T>            heap4;
};
