#pragma once

template <typename KeyT, typename ValueT>
struct SimpleHashMap {
    ValueT  defaultValue;

    int     bucketCount;
    int     bucketMask;
    vector<list<pair<KeyT, ValueT>>> buckets;

    function<int(const KeyT&)> hash;

    explicit SimpleHashMap(const function<int(const KeyT&)>& hash, int bucketSizeBit = 24, ValueT dfltValue = ValueT())
        : defaultValue(dfltValue), bucketCount(1 << bucketSizeBit), bucketMask(bucketCount - 1), buckets(bucketCount), hash(hash) {
    }


    ValueT get(KeyT index) const {
        int bucket = hash(index) & bucketMask;

        auto& lst = buckets[bucket];

        auto itEnd = lst.end();
        for (auto it = lst.begin(); it != itEnd; ++it) {
            if (it->first == index)
                return it->second;
            else if (it->first > index)
                break;
        }
        return defaultValue;
    }

    void set(KeyT index, ValueT value) {
        int bucket = hash(index) & bucketMask;

        auto& lst = buckets[bucket];

        auto itEnd = lst.end();
        for (auto it = lst.begin(); it != itEnd; ++it) {
            if (it->first == index) {
                it->second = value;
                return;
            } else if (it->first > index) {
                lst.emplace(it, index, value);
                return;
            }
        }
        lst.emplace_back(index, value);
    }


    ValueT operator [](KeyT index) const {
        return get(index);
    }

    ValueT& operator [](KeyT index) {
        int bucket = hash(index) & bucketMask;

        auto& lst = buckets[bucket];

        auto itEnd = lst.end();
        for (auto it = lst.begin(); it != itEnd; ++it) {
            if (it->first == index)
                return it->second;
            else if (it->first > index) {
                return lst.emplace(it, index, defaultValue)->second;
            }
        }

        lst.emplace_back(index, defaultValue);
        return lst.back().second;
    }
};
