#pragma once

class LRUCache {
public:
    int capacity;
    list<pair<int, int>> cache;                                     // (key, value)
    unordered_map<int, list<pair<int, int>>::iterator> keyToItem;   // 

    LRUCache(int capacity) : capacity(capacity) {
    }

    int get(int key) {
        auto it = keyToItem.find(key);
        if (it == keyToItem.end())
            return -1;

        int value = it->second->second;
        cache.erase(it->second);
        keyToItem[key] = cache.emplace(cache.end(), key, value);
        return value;
    }

    void put(int key, int value) {
        auto it = keyToItem.find(key);
        if (it != keyToItem.end()) {
            cache.erase(it->second);
        } else if (cache.size() == capacity) {
            keyToItem.erase(cache.front().first);
            cache.pop_front();
        }
        keyToItem[key] = cache.emplace(cache.end(), key, value);
    }
};
