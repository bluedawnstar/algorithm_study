#pragma once

// KeyT must be a integer type
template <typename KeyT, typename ValueT>
struct SimpleHashMap {
    struct Reference {
        ValueT& ref;

        Reference(ValueT& ref) : ref(ref) {
        }

        operator ValueT() const {
            return ref;
        }

        Reference& operator =(ValueT val) {
            ref = val;
            return *this;
        }
    };

    ValueT  defaultValue;

    int     bucketCount;
    int     bucketMask;
    vector<list<pair<KeyT, ValueT>>> buckets;

    explicit SimpleHashMap(int bucketSizeBit = 24, ValueT dfltValue = ValueT())
        : defaultValue(dfltValue), bucketCount(1 << bucketSizeBit), bucketMask(bucketCount - 1), buckets(bucketCount) {
    }


    ValueT get(KeyT index) const {
        int bucket = int(index & bucketMask);

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
        int bucket = int(index & bucketMask);

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

    Reference operator [](KeyT index) {
        int bucket = int(index % bucketCount);

        auto& lst = buckets[bucket];

        auto itEnd = lst.end();
        for (auto it = lst.begin(); it != itEnd; ++it) {
            if (it->first == index)
                return Reference(it->second);
            else if (it->first > index) {
                return Reference(lst.emplace(it, index, defaultValue)->second);
            }
        }

        lst.emplace_back(index, defaultValue);
        return Reference(lst.back().second);
    }
};
