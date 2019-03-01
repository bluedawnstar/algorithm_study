#pragma once

template <typename T>
struct SkipList {
    struct Node {
        T               value;
        int             height;
        vector<Node*>   next;
    };

    Node*   head;           // full height header node
    int     N;

    int     maxHeight;      // 
    int     currHeight;     // 

    explicit SkipList(int maxHeight = 30) : maxHeight(maxHeight) {
        head = createNode(T(), maxHeight);
        N = 0;
        currHeight = 0;
    }

    ~SkipList() {
        clear();
        delete head;
    }

    void clear() {
        Node* it = head->next[0];
        while (it) {
            auto* next = it->next[0];
            delete it;
            it = next;
        }
        head->next.assign(maxHeight, nullptr);
    }

    bool empty() const {
        return N == 0;
    }

    int size() const {
        return N;
    }

    Node* begin() {
        return head->next[0];
    }

    Node* next(Node* node) {
        return node ? node->next[0] : nullptr;
    }


    // O(logN)
    Node* lowerBound(const T& value) {
        vector<Node*> fix(maxHeight);
        return lowerBound(fix, value)->next[0];
    }

    // O(logN)
    Node* upperBound(const T& value) {
        vector<Node*> fix(maxHeight);
        return upperBound(fix, value)->next[0];
    }


    // O(logN)
    Node* find(const T& value) {
        auto* p = lowerBound(value);
        if (p && value == p->value)
            return p;
        return nullptr;
    }


    // O(logN)
    pair<Node*,bool> insert(const T& value) {
        vector<Node*> fix(maxHeight);
        auto* p = lowerBound(fix, value);

        if (p && p != head && value == p->value)
            return make_pair(p, false);

        int h = getRandomLevel();
#if 0
        if (h > currHeight) {
            h = ++currHeight;
            fix[h - 1] = head;
        }
#else
        if (h > currHeight) {
            do {
                fix[currHeight] = head;
            } while (++currHeight < h);
        }
#endif

        Node* it = createNode(value, h);
        while (--h >= 0) {
            it->next[h] = fix[h]->next[h];
            fix[h]->next[h] = it;
        }

        ++N;

        return make_pair(it, true);
    }

    // O(logN)
    bool erase(const T& value) {
        vector<Node*> fix(maxHeight);
        Node* p = lowerBound(fix, value)->next[0];

        if (!p || value != p->value)
            return false;

        for (int i = 0; i < currHeight; i++) {
            if (fix[i]->next[i] != p)
                break;
            fix[i]->next[i] = p->next[i];
        }
        destroy(p);

        while (currHeight > 0) {
            if (head->next[currHeight - 1])
                break;
            --currHeight;
        }

        --N;

        return true;
    }

private:
    static Node* createNode(const T& value, int height) {
        Node* node = new Node;
        if (!node)
            return nullptr;

        node->value = value;
        node->height = height;
        node->next.assign(height, nullptr);

        return node;
    }

    static void destroy(Node* p) {
        delete p;
    }

    int getRandomLevel() {
#if 1
        static std::mt19937_64 eng(7);
        static std::uniform_int_distribution<unsigned int> dist(0, std::numeric_limits<unsigned int>::max());

        static unsigned int bits = 0;
        static int reset = 0;

        int h, found = 0;

        for (h = 0; !found; h++) {
            if (!reset) {
                bits = dist(eng);
                reset = sizeof(unsigned int) * 8 - 1;
            }

            //found = bits % 3;
            //bits = bits / 3;
            found = bits & 1;
            bits = bits >> 1;
            --reset;
        }

        if (h > maxHeight)
            h = maxHeight;

        return h;
#else
        float r = (float)rand() / RAND_MAX; 
        int lvl = 1; 
        while (r < 0.5f && lvl < maxHeight) { 
            lvl++; 
            r = (float)rand() / RAND_MAX; 
        } 
        return lvl; 
#endif
    }


    Node* lowerBound(vector<Node*>& fix, const T& value) {
        Node* p = head;

        for (int i = currHeight - 1; i >= 0; i--) {
            while (p->next[i]) {
                if (value <= p->next[i]->value)
                    break;
                p = p->next[i];
            }
            fix[i] = p;
        }

        return p;
    }

    Node* upperBound(vector<Node*>& fix, const T& value) {
        Node* p = head;

        for (int i = currHeight - 1; i >= 0; i--) {
            while (p->next[i]) {
                if (value < p->next[i]->value)
                    break;
                p = p->next[i];
            }
            fix[i] = p;
        }

        return p;
    }
};
