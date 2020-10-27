#pragma once

template <typename KeyT, typename ValueT>
class LRUCache {
public:
    size_t capacity;
    ValueT invalidValue;

    typename list<pair<KeyT, ValueT>> cache;                                     // (key, value)
    unordered_map<KeyT, typename list<pair<KeyT, ValueT>>::iterator> keyToItem;  // 

    LRUCache(size_t capacity, ValueT invalidValue)
        : capacity(capacity), invalidValue(invalidValue) {
    }

    // O(1)
    ValueT get(KeyT key) {
        auto it = keyToItem.find(key);
        if (it == keyToItem.end())
            return invalidValue;

        ValueT value = it->second->second;
        cache.erase(it->second);
        keyToItem[key] = cache.emplace(cache.end(), key, value);
        return value;
    }

    // O(1)
    void put(KeyT key, ValueT value) {
        auto it = keyToItem.find(key);
        if (it != keyToItem.end()) {
            cache.erase(it->second);
        } else if (cache.size() >= capacity) {
            keyToItem.erase(cache.front().first);
            cache.pop_front();
        }
        keyToItem[key] = cache.emplace(cache.end(), key, value);
    }
};
