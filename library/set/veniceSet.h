#pragma once

// http://codeforces.com/blog/entry/58316

template <typename T>
struct VeniceSet {
    multiset<T> S;
    T waterLevel = 0;

    void add(T value) {
        S.insert(value + waterLevel);
    }
    
    void remove(T value) {
        S.erase(S.find(value + waterLevel));
    }

    void updateAll(T value) {
        waterLevel += value;
    }

    T getMin() const {
        return *S.begin() - waterLevel;
    }

    int size() const {
        return (int)S.size();
    }
};