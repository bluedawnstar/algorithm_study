#pragma once

template <typename T>
struct IndexableSkipList {
    struct Node {
        T       value;
        int     height;
        vector<pair<Node*,int>> next;   // (next pointer, skip count)
    };

    Node*   head;           // full height header node
    int     N;

    int     maxHeight;      // 
    int     currHeight;     // 

    explicit IndexableSkipList(int maxHeight = 30) : maxHeight(maxHeight) {
        head = createNode(T(), maxHeight);
        N = 0;
        currHeight = 0;
    }

    ~IndexableSkipList() {
        clear();
        delete head;
    }

    void clear() {
        Node* it = head->next[0].first;
        while (it) {
            auto* next = it->next[0].first;
            delete it;
            it = next;
        }
        head->next.assign(maxHeight, make_pair(nullptr, 0));
    }

    bool empty() const {
        return N == 0;
    }

    int size() const {
        return N;
    }

    Node* begin() {
        return head->next[0].first;
    }

    Node* next(Node* node) {
        return node ? node->next[0].first : nullptr;
    }


    // O(logN)
    Node* lowerBound(const T& value) {
        vector<pair<Node*,int>> fix(maxHeight);
        return lowerBound(fix, value)->next[0].first;
    }

    // O(logN)
    Node* upperBound(const T& value) {
        vector<pair<Node*,int>> fix(maxHeight);
        return upperBound(fix, value)->next[0].first;
    }


    // O(logN)
    Node* find(const T& value) {
        auto* p = lowerBound(value);
        if (p && value == p->value)
            return p;
        return nullptr;
    }

    // O(logN)
    Node* operator[](int index) {
        Node* p = head;

        for (int i = currHeight - 1; i >= 0; i--) {
            while (p->next[i].first) {
                if (index < p->next[i].second)
                    break;
                index -= p->next[i].second;
                p = p->next[i].first;
            }
        }

        return p->next[0].first;
    }


    // O(logN)
    pair<Node*,bool> insert(const T& value) {
        vector<pair<Node*,int>> fix(maxHeight);
        auto* p = lowerBound(fix, value);

        if (p && p != head && value == p->value)
            return make_pair(p, false);

        return insert(fix, value);
    }

    // O(logN)
    bool erase(const T& value) {
        vector<pair<Node*,int>> fix(maxHeight);
        Node* p = lowerBound(fix, value)->next[0].first;

        if (!p || value != p->value)
            return false;

        return erase(fix, p);
    }


    // O(logN)
    pair<Node*,bool> insertAt(int index, const T& value) {
        vector<pair<Node*,int>> fix(maxHeight);
        getInsertPositionByIndex(fix, index);
        return insert(fix, value);
    }

    // O(logN)
    bool eraseAt(int index) {
        vector<pair<Node*,int>> fix(maxHeight);
        auto* p = getInsertPositionByIndex(fix, index)->next[0].first;
        return erase(fix, p);
    }


private:
    static Node* createNode(const T& value, int height) {
        Node* node = new Node;
        if (!node)
            return nullptr;

        node->value = value;
        node->height = height;
        node->next.assign(height, make_pair(nullptr, 0));

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


    Node* getInsertPositionByIndex(vector<pair<Node*,int>>& fix, int index) {
        Node* p = head;

        int cnt = 0;
        for (int i = currHeight - 1; i >= 0; i--) {
            while (p->next[i].first) {
                if (index < cnt + p->next[i].second)
                    break;
                cnt += p->next[i].second;
                p = p->next[i].first;
            }
            fix[i] = make_pair(p, cnt);
        }

        return p;
    }


    Node* lowerBound(vector<pair<Node*,int>>& fix, const T& value) {
        Node* p = head;

        int cnt = 0;
        for (int i = currHeight - 1; i >= 0; i--) {
            while (p->next[i].first) {
                if (value <= p->next[i].first->value)
                    break;
                cnt += p->next[i].second;
                p = p->next[i].first;
            }
            fix[i] = make_pair(p, cnt);
        }

        return p;
    }

    Node* upperBound(vector<pair<Node*,int>>& fix, const T& value) {
        Node* p = head;

        int cnt = 0;
        for (int i = currHeight - 1; i >= 0; i--) {
            while (p->next[i].first) {
                if (value < p->next[i].first->value)
                    break;
                cnt += p->next[i].second;
                p = p->next[i].first;
            }
            fix[i] = make_pair(p, cnt);
        }

        return p;
    }


    pair<Node*,bool> insert(vector<pair<Node*,int>>& fix, const T& value) {
        int h = getRandomLevel();
#if 0
        if (h > currHeight) {
            h = ++currHeight;
            fix[h - 1].first = head;
            fix[h - 1].first->next[h - 1].second = N;
        }
#else
        if (h > currHeight) {
            do {
                fix[currHeight].first = head;
                fix[currHeight].first->next[currHeight].second = N;
            } while (++currHeight < h);
        }
#endif

        Node* it = createNode(value, h);
        if (h > 0) {
            int index = fix[0].second + 1;
            for (int i = 0; i < h; i++) {
                int n = index - fix[i].second;
                it->next[i].first = fix[i].first->next[i].first;
                it->next[i].second = fix[i].first->next[i].second - n + 1;
                fix[i].first->next[i].first = it;
                fix[i].first->next[i].second = n;
            }
        }
        for (; h < currHeight; h++)
            ++fix[h].first->next[h].second;

        ++N;

        return make_pair(it, true);
    }

    bool erase(vector<pair<Node*,int>>& fix, Node* p) {
        int index = fix[0].second;
        for (int i = 0; i < currHeight; i++) {
            if (fix[i].first->next[i].first != p) {
                for (; i < currHeight; i++)
                    --fix[i].first->next[i].second;
                break;
            }
            fix[i].first->next[i].first = p->next[i].first;
            fix[i].first->next[i].second = (index - fix[i].second) + p->next[i].second;
        }
        destroy(p);

        while (currHeight > 0) {
            if (head->next[currHeight - 1].first)
                break;
            --currHeight;
        }

        --N;

        return true;
    }
};
