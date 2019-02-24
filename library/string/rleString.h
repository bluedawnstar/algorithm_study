#pragma once

// run-length encoding string
struct RleString {
    int N;
    list<pair<char, int>> S;

    RleString() {
        N = 0;
    }

    //--- query

    // (iterator, offset)
    pair<list<pair<char, int>>::iterator, int> get(int index) {
        auto it = S.begin();
        while (it != S.end()) {
            if (index < it->second) {
                return make_pair(it, index);
            }
            index -= it->second;
            ++it;
        }
        return make_pair(it, 0);
    }

    // (iterator, offset)
    pair<list<pair<char, int>>::const_iterator, int> get(int index) const {
        auto it = S.begin();
        while (it != S.end()) {
            if (index < it->second) {
                return make_pair(it, index);
            }
            index -= it->second;
            ++it;
        }
        return make_pair(it, 0);
    }

    char operator [](int index) const {
        return get(index).first->first;
    }

    //--- update

    void assign(const string& s) {
        S.clear();
        for (auto c : s) {
            if (S.empty() || S.back().first != c)
                S.emplace_back(c, 1);
            else
                S.back().second++;
        }
        N = int(s.length());
    }

    void append(const RleString& rhs) {
        if (this != &rhs) {
            for (auto& it : rhs.S)
                insert(N, it.first, it.second);
        }
    }

    void append(RleString&& rhs) {
        if (S.back().first == rhs.S.front().first) {
            S.back().second += rhs.S.front().second;
            auto it = rhs.S.begin();
            S.splice(S.end(), rhs.S, ++it, rhs.S.end());
        } else {
            S.splice(S.end(), rhs.S);
        }
        N += rhs.N;

        rhs.N = 0;
        rhs.S.clear();
    }


    void insert(int pos, char c, int n = 1) {
        auto it = get(pos);
        if (it.first == S.end()) {
            if (it.first != S.begin()) {
                auto it2 = it.first;
                --it2;
                if (it2->first == c)
                    it2->second += n;
                else
                    S.emplace_back(c, n);
            }
            else {
                S.emplace_back(c, n);
            }
        } else if (it.first->first == c) {
            it.first->second += n;
        } else if (it.second == 0) {
            if (it.first != S.begin()) {
                auto it2 = it.first;
                --it2;
                if (it2->first == c)
                    it2->second += n;
                else
                    S.insert(it.first, make_pair(c, n));
            } else {
                S.insert(it.first, make_pair(c, n));
            }
        } else {
            S.insert(it.first, make_pair(it.first->first, it.second));
            S.insert(it.first, make_pair(c, n));
            it.first->second -= it.second;
        }

        N += n;
    }

    //--- special operations

    bool isPalindrom() const {
        auto it = S.begin();
        auto itR = S.end();

        --itR;

        int L = 0, R = N - 1;
        while (L < R) {
            if (it->first != itR->first || it->second != itR->second)
                return false;
            L += it->second;
            R -= itR->second;
            ++it;
            --itR;
        }

        return true;
    }
};
