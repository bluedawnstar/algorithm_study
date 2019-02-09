#pragma once

// Hash Map with Open Addressing Hash Table
// from https://github.com/MikeMirzayanov/open_addressing_hash_table/blob/master/oaht.h

template <typename KeyT, typename ValueT, typename HashT = std::hash<KeyT>>
class HashMapOAHT {
    enum class NodeStateT {
        nsFree,
        nsInUse,
        nsErased
    };

    template <typename KeyT, typename ValueT>
    struct node {
        KeyT        key;
        ValueT      value;
        NodeStateT  state = NodeStateT::nsFree;
    };

public:
    explicit HashMapOAHT(size_t capacity = 3) : size(0), nodes(capacity) {
    }

    bool exist(const KeyT& key) const {
        return find(key) < nodes.size();
    }

    const ValueT& get(const KeyT& key, const ValueT& defaultValue) const {
        size_t index = find(key);
        if (index >= nodes.size())
            return defaultValue;
        return nodes[index].value;
    }

    void set(const KeyT& key, const ValueT& value) {
        if ((size << 1) > nodes.size())
            rehash();

        size_t index;
        if (put(key, index, nodes))
            size++;

        nodes[index].value = value;
    }

    ValueT& operator[](const KeyT& key) {
        if ((size << 1) > nodes.size())
            rehash();

        size_t index;
        if (put(key, index, nodes))
            size++;

        return nodes[index].value;
    }

private:
    size_t getIndex(const KeyT& key, size_t size) const {
        return (h(key) * 22543) % size;
    }

    void rehash() {
        size_t n_capacity = (nodes.size() << 1);
        vector<node<KeyT, ValueT>> n_nodes(n_capacity);

        auto N = nodes.size();
        for (size_t i = 0; i < N; i++) {
            if (nodes[i].state == NodeStateT::nsInUse) {
                size_t index;
                put(nodes[i].key, index, n_nodes);
                n_nodes[index].value = nodes[i].value;
            }
        }
        swap(nodes, n_nodes);
    }

    size_t find(const KeyT& key) const {
        size_t index = getIndex(key, nodes.size());

        auto N = nodes.size();
        for (size_t d = 0; d < N; d++) {
            if (nodes[index].state == NodeStateT::nsFree)
                break;
            if (nodes[index].state == NodeStateT::nsInUse && nodes[index].key == key)
                return index;

            if (++index >= N)
                index = 0;
        }

        return nodes.size();
    }

    bool put(const KeyT& key, size_t& index, vector<node<KeyT, ValueT>>& nodes) {
        index = getIndex(key, nodes.size());

        auto N = nodes.size();
        for (size_t i = 0; i < N; i++) {
            if (nodes[index].state == NodeStateT::nsFree || nodes[index].state == NodeStateT::nsErased) {
                nodes[index].key = key;
                nodes[index].state = NodeStateT::nsInUse;
                return true;
            }

            if (nodes[index].key == key)
                return false;

            if (++index >= N)
                index = 0;
        }

        //throw std::logic_error("Unexpected case.");

        return false;
    }

    size_t size;
    vector<node<KeyT, ValueT>> nodes;

    HashT h;
};
