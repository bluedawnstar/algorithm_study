#pragma once

#include <cassert>
#include "../memory/fixedSizeAllocator.h"

struct HAMTConfig {
    //* for bitmap
    //typedef unsigned int BitmapT;

    //static const int BITMAP_BITS = sizeof(unsigned int) * 8;
    //static const unsigned int BITMAP_ALL = 0xffffffffu;
    //static const unsigned int BITMAP_ONE = 1u;

    //* for hash values
    //static const int HASH_BITMAP_INDEX_BITS = 5;
    //static const unsigned int HASH_BITMAP_INDEX_MASK = 0x1F;

    //static const int ROOT_HASH_BITS = 2;
    //static const unsigned int ROOT_HASH_MASK = 0xc0000000u;

    //static const int HAMT_BITS = 30;
    //static const int MAX_HAMT_DEPTH = HAMT_BITS / BITMAP_INDEX_BITS;

    //* for memory allocation
    //static const int CHUNK_SIZE = 128;

    //static int calcAllocSize(int n) { ... }
};

// HAMT with 32bit
template <int _MAX_DEPTH = 5, int _HASH_BITS = 32, int _CHUNK_SIZE = 128>
struct HAMTConfigUInt32 {
    // for bitmap
    typedef unsigned int BitmapT;

    static const int BITMAP_BITS = sizeof(unsigned int) * 8;
    static const unsigned int BITMAP_ALL = 0xffffffffu;
    static const unsigned int BITMAP_ONE = 1u;

    // for hash values (unsigned int)
    static const int HASH_BITMAP_INDEX_BITS = 5;
    static const unsigned int HASH_BITMAP_INDEX_MASK = 0x1F;

    static const int ROOT_HASH_BITS = _HASH_BITS - _MAX_DEPTH * HASH_BITMAP_INDEX_BITS;
    static const unsigned int ROOT_HASH_MASK = 0xffffffffu << (_MAX_DEPTH * HASH_BITMAP_INDEX_BITS);

    static const int HAMT_BITS = _MAX_DEPTH * HASH_BITMAP_INDEX_BITS;
    static const int MAX_HAMT_DEPTH = HAMT_BITS / HASH_BITMAP_INDEX_BITS;

    // for memory allocation
    static const int CHUNK_SIZE = _CHUNK_SIZE;

    static int calcAllocSize(int n) {
        static const int sSizeTable[33] = {
             0,
             4,  4,  4,  4,  8,  8,  8,  8, //  1 ~  8
            10, 10, 12, 12, 14, 14, 16, 16, //  9 ~ 16
            18, 18, 20, 20, 22, 22, 24, 24, // 17 ~ 24
            26, 26, 28, 28, 30, 30, 32, 32  // 25 ~ 32
        };
        return sSizeTable[n];
    }
};

// HAMT with 64bit
template <int _MAX_DEPTH = 5, int _HASH_BITS = 32, int _CHUNK_SIZE = 128>
struct HAMTConfigUInt64 {
    // for bitmap
    typedef unsigned long long BitmapT;

    static const int BITMAP_BITS = sizeof(unsigned long long) * 8;
    static const unsigned long long BITMAP_ALL = 0xffffffffffffffffull;
    static const unsigned long long BITMAP_ONE = 1ull;

    // for hash values (unsigned int)
    static const int HASH_BITMAP_INDEX_BITS = 6;
    static const unsigned int HASH_BITMAP_INDEX_MASK = 0x3F;

    static const int ROOT_HASH_BITS = _HASH_BITS - _MAX_DEPTH * HASH_BITMAP_INDEX_BITS;
    static const unsigned int ROOT_HASH_MASK = 0xffffffffu << (_MAX_DEPTH * HASH_BITMAP_INDEX_BITS);

    static const int HAMT_BITS = _MAX_DEPTH * HASH_BITMAP_INDEX_BITS;
    static const int MAX_HAMT_DEPTH = HAMT_BITS / HASH_BITMAP_INDEX_BITS;

    // for memory allocation
    static const int CHUNK_SIZE = _CHUNK_SIZE;

    static int calcAllocSize(int n) {
        static const int sSizeTable[65] = {
             0,
             8,  8,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16, 16, 16, 16, //  1 ~ 16
            20, 20, 20, 20, 24, 24, 24, 24, 28, 28, 28, 28, 32, 32, 32, 32, // 17 ~ 32
            34, 34, 36, 36, 38, 38, 40, 40, 42, 42, 44, 44, 46, 46, 48, 48, // 33 ~ 48
            50, 50, 52, 52, 54, 54, 56, 56, 58, 58, 60, 60, 62, 62, 64, 64  // 49 ~ 64
        };
        return sSizeTable[n];
    }
};

// 1-depth HAMT with 32bit
struct HAMTConfigUInt32Depth1 {
    // for bitmap
    typedef unsigned int BitmapT;

    static const int BITMAP_BITS = sizeof(unsigned int) * 8;
    static const unsigned int BITMAP_ALL = 0xffffffffu;
    static const unsigned int BITMAP_ONE = 1u;

    // for hash values (unsigned int)
    static const int HASH_BITMAP_INDEX_BITS = 5;
    static const unsigned int HASH_BITMAP_INDEX_MASK = 0x1F;

    static const int ROOT_HASH_BITS = 27;
    static const unsigned int ROOT_HASH_MASK = 0xffffffe0u;

    static const int HAMT_BITS = 5;
    static const int MAX_HAMT_DEPTH = HAMT_BITS / HASH_BITMAP_INDEX_BITS;

    // for memory allocation
    static const int CHUNK_SIZE = 128;

    static int calcAllocSize(int n) {
        static const int sSizeTable[33] = {
             0,
             4,  4,  4,  4,  8,  8,  8,  8, //  1 ~  8
            10, 10, 12, 12, 14, 14, 16, 16, //  9 ~ 16
            18, 18, 20, 20, 22, 22, 24, 24, // 17 ~ 24
            26, 26, 28, 28, 30, 30, 32, 32  // 25 ~ 32
        };
        return sSizeTable[n];
    }
};

// 1-depth HAMT with 64bit
struct HAMTConfigUInt64Depth1 {
    // for bitmap
    typedef unsigned long long BitmapT;

    static const int BITMAP_BITS = sizeof(unsigned long long) * 8;
    static const unsigned long long BITMAP_ALL = 0xffffffffffffffffull;
    static const unsigned long long BITMAP_ONE = 1ull;

    // for hash values (unsigned int)
    static const int HASH_BITMAP_INDEX_BITS = 6;
    static const unsigned int HASH_BITMAP_INDEX_MASK = 0x3F;

    static const int ROOT_HASH_BITS = 26;
    static const unsigned int ROOT_HASH_MASK = 0xffffffc0u;

    static const int HAMT_BITS = 6;
    static const int MAX_HAMT_DEPTH = HAMT_BITS / HASH_BITMAP_INDEX_BITS;

    // for memory allocation
    static const int CHUNK_SIZE = 128;

    static int calcAllocSize(int n) {
        static const int sSizeTable[65] = {
             0,
             8,  8,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16, 16, 16, 16, //  1 ~ 16
            20, 20, 20, 20, 24, 24, 24, 24, 28, 28, 28, 28, 32, 32, 32, 32, // 17 ~ 32
            34, 34, 36, 36, 38, 38, 40, 40, 42, 42, 44, 44, 46, 46, 48, 48, // 33 ~ 48
            50, 50, 52, 52, 54, 54, 56, 56, 58, 58, 60, 60, 62, 62, 64, 64  // 49 ~ 64
        };
        return sSizeTable[n];
    }
};

template <typename KeyT, typename ValueT, typename ConfigT = HAMTConfigUInt32<>>
class HashMapHAMT {
    typedef unsigned int HashValueT;
    typedef typename ConfigT::BitmapT BitmapT;

    static int popcount(unsigned x) {
#ifndef __GNUC__
        return (int)__popcnt(x);
#else
        return __builtin_popcount(x);
#endif
    }

    static int popcount(unsigned long long x) {
#if defined(_M_X64)
        return (int)__popcnt64(x);
#elif defined(__GNUC__)
        return __builtin_popcountll(x);
#else
        return (int)__popcnt(unsigned(x)) + (int)__popcnt(unsigned(x >> 32));
#endif
    }

    struct HAMTPointer {
        void*   p;

        HAMTPointer() : p(nullptr) {
        }

        HAMTPointer(void* p, bool internalNode) {
            set(p, internalNode);
        }

        void set(void* p, bool internalNode) {
            if (internalNode)
                this->p = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(p) | intptr_t(1));
            else
                this->p = p;
        }

        void reset() {
            p = nullptr;
        }

        bool isNull() const {
            return p == nullptr;
        }

        bool isInternal() const {
            return (reinterpret_cast<intptr_t>(p) & intptr_t(1)) != 0;
        }

        template <typename T>
        T* getAsInternal(const T*) const {
            return reinterpret_cast<T*>(reinterpret_cast<intptr_t>(p) & ~intptr_t(1));
        }

        template <typename T>
        T* getAsLeaf(const T*) const {
            return reinterpret_cast<T*>(p);
        }
    };

    struct HAMTAllocator {
        vector<unique_ptr<FixedSizeAllocator>> allocators;

        explicit HAMTAllocator(unsigned int chunkSize) {
            allocators.resize(ConfigT::BITMAP_BITS + 1);
            for (int i = 1; i <= ConfigT::BITMAP_BITS; i++) {
                allocators[i].reset(new FixedSizeAllocator(static_cast<unsigned int>(i) * sizeof(HAMTPointer), chunkSize));
            }
        }

        HAMTPointer* allocate(int count) {
            return reinterpret_cast<HAMTPointer*>(allocators[ConfigT::calcAllocSize(count)]->allocate());
        }

        void deallocate(int count, HAMTPointer* p) {
            return allocators[ConfigT::calcAllocSize(count)]->deallocate(p);
        }
    };

    class HAMTNode {
    public:
        HAMTNode() : bitmap(0), data(nullptr) {
        }

        HAMTNode(HAMTNode&& rhs) : bitmap(rhs.bitmap), data(rhs.data) {
            rhs.bitmap = 0;
            rhs.data = nullptr;
        }

        ~HAMTNode() {
            assert(data == nullptr);
        }

        HAMTNode& operator =(HAMTNode&& rhs) {
            if (this != &rhs) {
                data = rhs.data;
                bitmap = rhs.bitmap;

                rhs.bitmap = 0;
                rhs.data = nullptr;
            }
            return *this;
        }


        void destroy(HAMTAllocator& allocator) {
            allocator.deallocate(count(), data);
            data = nullptr;
        }


        void swap(HAMTNode& rhs) {
            ::swap(bitmap, rhs.bitmap);
            ::swap(data, rhs.data);
        }

        int count() const {
            return popcount(bitmap);
        }

        int capacity() const {
            return ConfigT::calcAllocSize(popcount(bitmap));
        }

        int maxSize() const {
            return ConfigT::BITMAP_BITS;
        }


        bool test(int pos) const {
            return (bitmap & (ConfigT::BitmapT(1) << pos)) != 0;
        }

        HAMTPointer* get(int pos) {
            if (test(pos))
                return &data[posToOffset(pos)];
            return nullptr;
        }

        const HAMTPointer* get(int pos) const {
            if (test(pos))
                return &data[posToOffset(pos)];
            return nullptr;
        }

        HAMTPointer* getWithOffset(int offset) {
            return &data[offset];
        }

        const HAMTPointer* getWithOffset(int offset) const {
            return &data[offset];
        }


        bool insert(int pos, HAMTPointer value, HAMTAllocator& allocator) {
            auto offset = posToOffset(pos);
            if (bitmap & (ConfigT::BitmapT(1) << pos)) {
                data[offset] = value;
                return false;
            } else {
                insertOffset(offset, value, allocator);
                bitmap |= ConfigT::BitmapT(1) << pos;
                return true;
            }
        }

        void erase(int pos, HAMTAllocator& allocator) {
            auto offset = posToOffset(pos);
            if (bitmap & (ConfigT::BitmapT(1) << pos)) {
                eraseOffset(offset, allocator);
                bitmap &= ~(ConfigT::BitmapT(1) << pos);
            }
        }

    private:
        int posToOffset(int pos) const {
            return popcount(bitmap & ((ConfigT::BitmapT(1) << pos) - 1));
        }

        int offsetToPos(int offset) const {
            ConfigT::BitmapT bm = bitmap;
            while (offset-- > 0)
                bm &= bm - 1;
            return popcount((bm & -bm) - 1);
        }

        void insertOffset(int offset, HAMTPointer value, HAMTAllocator& allocator) {
            int cnt = count();
            int cap = capacity();

            if (cnt >= cap) {
                int newSize = ConfigT::calcAllocSize(cnt + 1);
                if (data) {
                    auto* psrc = data;
                    auto* pdst = reinterpret_cast<HAMTPointer*>(allocator.allocate(newSize));
                    memcpy(pdst, psrc, offset * sizeof(HAMTPointer));
                    memcpy(pdst + offset + 1, psrc + offset, (cnt - offset) * sizeof(HAMTPointer));
                    destroy(allocator);
                    data = pdst;
                } else {
                    data = reinterpret_cast<HAMTPointer*>(allocator.allocate(newSize));
                }
            } else if (offset < cnt) {
                memmove(data + offset + 1, data + offset, (cnt - offset) * sizeof(HAMTPointer));
            }
            data[offset] = value;
        }

        void eraseOffset(int offset, HAMTAllocator& allocator) {
            int cnt = count();
            if (cnt == 1) {
                destroy(allocator);
            } else {
                int newSize = ConfigT::calcAllocSize(cnt - 1);
                if (newSize != capacity()) {
                    auto* psrc = data;
                    auto* pdst = reinterpret_cast<HAMTPointer*>(allocator.allocate(newSize));
                    memcpy(pdst, psrc, offset * sizeof(HAMTPointer));
                    memcpy(pdst + offset, psrc + offset + 1, (cnt - offset - 1) * sizeof(HAMTPointer));

                    destroy(allocator);
                    data = pdst;
                } else if (offset < cnt - 1) {
                    memcpy(data + offset, data + offset + 1, (cnt - offset - 1) * sizeof(HAMTPointer));
                }
            }
        }

    private:
        HAMTNode(const HAMTNode& rhs) = delete;
        HAMTNode& operator =(const HAMTNode& rhs) = delete;

    private:
        BitmapT         bitmap; // 
        HAMTPointer*    data;   // 
    };

    struct HAMTListNode {
        union {
            unsigned char val[sizeof(pair<KeyT, ValueT>)];
            pair<KeyT, ValueT> val2;
        };
        HAMTListNode* next;

        HAMTListNode() {
            next = nullptr;
        }

        HAMTListNode(const KeyT& key, const ValueT& value) {
            ::new (val) pair<KeyT, ValueT>(key, value);
            next = nullptr;
        }

        ~HAMTListNode() {
            val2.first.~KeyT();
            val2.second.~ValueT();
        }

        const KeyT& getFirst() const {
            return reinterpret_cast<const pair<KeyT, ValueT>*>(val)->first;
        }

        const ValueT& getSecond() const {
            return reinterpret_cast<const pair<KeyT, ValueT>*>(val)->second;
        }

        KeyT& getFirst() {
            return reinterpret_cast<pair<KeyT, ValueT>*>(val)->first;
        }

        ValueT& getSecond() {
            return reinterpret_cast<pair<KeyT, ValueT>*>(val)->second;
        }

        bool operator <(const HAMTListNode& rhs) const {
            return reinterpret_cast<const pair<KeyT, ValueT>*>(val)->first < reinterpret_cast<const pair<KeyT, ValueT>*>(rhs.val)->first;
        }

        bool operator <(const KeyT& rhs) const {
            return reinterpret_cast<const pair<KeyT, ValueT>*>(val)->first < rhs;
        }

        bool operator ==(const HAMTListNode& rhs) const {
            return reinterpret_cast<const pair<KeyT, ValueT>*>(val)->first == reinterpret_cast<const pair<KeyT, ValueT>*>(rhs.val)->first;
        }

        bool operator ==(const KeyT& rhs) const {
            return reinterpret_cast<const pair<KeyT, ValueT>*>(val)->first == rhs;
        }
    };

    class HAMTList {
    private:
        template <typename... Args>
        static HAMTListNode* create(TypeAllocator<HAMTListNode>& allocator, Args&&... args) {
            HAMTListNode* p = allocator.allocate();
            ::new (p->val) pair<KeyT, ValueT>(std::forward<Args>(args)...);
            return p;
        }

        static void destroy(TypeAllocator<HAMTListNode>& allocator, HAMTListNode* node) {
            reinterpret_cast<pair<KeyT, ValueT>*>(node->val)->~pair<KeyT, ValueT>();
            allocator.deallocate(node);
        }

    public:
        explicit HAMTList(unsigned int hashValue)
            : hashValue(hashValue), head(nullptr), tail(nullptr) {
        }

        ~HAMTList() {
            assert(head == nullptr);
            assert(tail == nullptr);
        }

        void setHashValue(unsigned int hashValue) {
            this->hashValue = hashValue;
        }

        unsigned int getHashValue() const {
            return hashValue;
        }


        void clear(TypeAllocator<HAMTListNode>& allocator) {
            HAMTListNode* p = head;

            while (p) {
                HAMTListNode* del = p;
                p = p->next;
                destroy(allocator, del);
            }

            head = tail = nullptr;
        }


        pair<HAMTListNode*, HAMTListNode*> lowerBound(const KeyT& key) {
            if (head == nullptr)
                return make_pair(nullptr, nullptr);

            HAMTListNode* prev = nullptr;
            HAMTListNode* curr = head;
            while (curr) {
                if (*curr < key)
                    break;
                else if (*curr == key)
                    break;
                prev = curr;
                curr = curr->next;
            }

            return make_pair(prev, curr);
        }

        HAMTListNode* find(const KeyT& key) {
            auto r = lowerBound(key);
            if (r.second && *r.second == key)
                return r.second;
            return nullptr;
        }

        const HAMTListNode* find(const KeyT& key) const {
            return const_cast<HAMTList*>(this)->find(key);
        }


        HAMTListNode* insert(TypeAllocator<HAMTListNode>& allocator, const KeyT& key, const ValueT& value) {
            if (head == nullptr) {
                auto* node = allocator.construct(key, value);
                insertFirst(node);
                return node;
            }

            auto r = lowerBound(key);
            if (r.second && *r.second == key) {
                r.second->getSecond() = value;
                return r.second;
            } else {
                auto* node = allocator.construct(key, value);
                insertAfter(r.first, node);
                return node;
            }
        }

        HAMTListNode* remove(const KeyT& key) {
            if (head == nullptr)
                return nullptr;

            auto r = lowerBound(key);
            if (r.second && *r.second == key)
                removeAfter(r.first, r.second);
            return nullptr;
        }

    protected:
        void insertAfter(HAMTListNode* prev, HAMTListNode* node) {
            if (head == nullptr) {
                head = tail = node;
                return;
            }

            if (prev == nullptr) {
                node->next = head;
                head = node;
            } else {
                node->next = prev->next;
                prev->next = node;
                if (prev == tail)
                    tail = node;
            }
        }

        void insertFirst(HAMTListNode* node) {
            insertAfter(nullptr, node);
        }

        void removeAfter(HAMTListNode* prev, HAMTListNode* node) {
            if (node == head)
                head = head->next;
            if (node == tail)
                tail = prev;

            if (prev)
                prev->next = node->next;
        }

        void removeFirst(HAMTListNode* node) {
            removeAfter(nullptr, node);
        }

    private:
        unsigned int hashValue;
        HAMTListNode* head;
        HAMTListNode* tail;
    };

    class HAMT {
    public:
        explicit HAMT(const function<unsigned int(const KeyT&)>& hash)
            : nodeAllocator(ConfigT::CHUNK_SIZE), pointerAllocator(ConfigT::CHUNK_SIZE),
              listNodeAllocator(ConfigT::CHUNK_SIZE), listAllocator(ConfigT::CHUNK_SIZE),
              hash(hash), root(1u << ConfigT::ROOT_HASH_BITS) {
        }

        ~HAMT() {
            clear();
        }


        void clear() {
            for (auto& it : root)
                clearSub(it);
        }

        HAMTListNode* find(const KeyT& key) {
            unsigned int hashValue = hash(key);

            unsigned int rootHash = hashValue >> ConfigT::HAMT_BITS;
            unsigned int hamtHash = hashValue & ~ConfigT::ROOT_HASH_MASK;

            return findSub(root[rootHash], hamtHash, 0, key);
        }

        const HAMTListNode* find(const KeyT& key) const {
            return const_cast<HAMT*>(this)->find(key);
        }

        HAMTListNode* insert(const KeyT& key, const ValueT& value) {
            unsigned int hashValue = hash(key);

            unsigned int rootHash = hashValue >> ConfigT::HAMT_BITS;
            unsigned int hamtHash = hashValue & ~ConfigT::ROOT_HASH_MASK;

            return insertSub(root[rootHash], hamtHash, 0, key, value);
        }

        bool erase(const KeyT& key) {
            unsigned int hashValue = hash(key);

            unsigned int rootHash = hashValue >> ConfigT::HAMT_BITS;
            unsigned int hamtHash = hashValue & ~ConfigT::ROOT_HASH_MASK;

            return eraseSub(root[rootHash], hamtHash, 0, key);
        }

    protected:
        HAMTListNode* insertSub(HAMTPointer& pointer, unsigned int hamtHash, int depth, const KeyT& key, const ValueT& value) {
            if (pointer.isNull()) {
                auto* list = listAllocator.construct(hamtHash);
                pointer.set(list, false);
                return list->insert(listNodeAllocator, key, value);
            }

            int shiftBits = (ConfigT::MAX_HAMT_DEPTH - depth - 1) * ConfigT::HASH_BITMAP_INDEX_BITS;
            unsigned int hamtIndex = (hamtHash >> shiftBits) & ConfigT::HASH_BITMAP_INDEX_MASK;

            if (pointer.isInternal()) {
                auto* hamtNode = pointer.getAsInternal(static_cast<HAMTNode*>(nullptr));
                if (!hamtNode->test(hamtIndex)) {
                    auto* list = listAllocator.construct(hamtHash);
                    hamtNode->insert(hamtIndex, HAMTPointer(list, false), pointerAllocator);
                    return list->insert(listNodeAllocator, key, value);
                } else {
                    return insertSub(*hamtNode->get(hamtIndex), hamtHash, depth + 1, key, value);
                }
            } else {
                auto* hamtList = pointer.getAsLeaf(static_cast<HAMTList*>(nullptr));
                if (hamtList->getHashValue() == hamtHash) {
                    return hamtList->insert(listNodeAllocator, key, value);
                } else {
                    pointer.reset();
                    return split(pointer, hamtHash, depth, key, value, hamtList);
                }
            }
        }

        HAMTListNode* split(HAMTPointer& pointer, unsigned int hamtHash, int depth,
                const KeyT& key, const ValueT& value, HAMTList* oldList) {
            int shiftBits = (ConfigT::MAX_HAMT_DEPTH - depth - 1) * ConfigT::HASH_BITMAP_INDEX_BITS;

            unsigned int hamtIndex = (hamtHash >> shiftBits) & ConfigT::HASH_BITMAP_INDEX_MASK;
            unsigned int hamtIndex2 = (oldList->getHashValue() >> shiftBits) & ConfigT::HASH_BITMAP_INDEX_MASK;

            auto* hamtNode = nodeAllocator.construct();
            pointer.set(hamtNode, true);

            if (hamtIndex == hamtIndex2) {
                assert(depth < ConfigT::MAX_HAMT_DEPTH);
                hamtNode->insert(hamtIndex, HAMTPointer(), pointerAllocator);
                return split(*hamtNode->get(hamtIndex), hamtHash, depth + 1, key, value, oldList);
            } else {
                auto* list = listAllocator.construct(hamtHash);
                hamtNode->insert(hamtIndex, HAMTPointer(list, false), pointerAllocator);
                hamtNode->insert(hamtIndex2, HAMTPointer(oldList, false), pointerAllocator);
                return list->insert(listNodeAllocator, key, value);
            }
        }

        bool eraseSub(HAMTPointer& pointer, unsigned int hamtHash, int depth, const KeyT& key) {
            if (pointer.isNull())
                return false;

            int shiftBits = (ConfigT::MAX_HAMT_DEPTH - depth - 1) * ConfigT::HASH_BITMAP_INDEX_BITS;
            unsigned int hamtIndex = (hamtHash >> shiftBits) & ConfigT::HASH_BITMAP_INDEX_MASK;

            if (pointer.isInternal()) {
                assert(depth < ConfigT::MAX_HAMT_DEPTH);
                auto* hamtNode = pointer.getAsInternal(static_cast<HAMTNode<BitmapT>*>(nullptr));
                if (!hamtNode->test(hamtIndex))
                    return false;
                return eraseSub(*hamtNode->get(hamtIndex), hamtHash, depth + 1, key);
            } else {
                auto* hamtList = pointer.getAsLeaf(static_cast<ListT*>(nullptr));
                if (hamtList->getHashValue() != hamtHash)
                    return false;
                listNodeAllocator.deallocate(hamtList->remove(key));
                return true;
            }
        }

        void clearSub(HAMTPointer& pointer) {
            if (pointer.isNull())
                return;

            if (pointer.isInternal()) {
                auto* hamtNode = pointer.getAsInternal(static_cast<HAMTNode*>(nullptr));
                for (int i = 0, cnt = hamtNode->count(); i < cnt; i++)
                    clearSub(*hamtNode->getWithOffset(i));
                hamtNode->destroy(pointerAllocator);
                nodeAllocator.deallocate(hamtNode);
            } else {
                auto* hamtList = pointer.getAsLeaf(static_cast<HAMTList*>(nullptr));
                hamtList->clear(listNodeAllocator);
                listAllocator.deallocate(hamtList);
            }
        }

        HAMTListNode* findSub(HAMTPointer& pointer, unsigned int hamtHash, int depth, const KeyT& key) {
            if (pointer.isNull())
                return nullptr;

            int shiftBits = (ConfigT::MAX_HAMT_DEPTH - depth - 1) * ConfigT::HASH_BITMAP_INDEX_BITS;
            unsigned int hamtIndex = (hamtHash >> shiftBits) & ConfigT::HASH_BITMAP_INDEX_MASK;

            if (pointer.isInternal()) {
                assert(depth < ConfigT::MAX_HAMT_DEPTH);
                auto* hamtNode = pointer.getAsInternal(static_cast<HAMTNode*>(nullptr));
                if (!hamtNode->test(hamtIndex))
                    return nullptr;
                return findSub(*hamtNode->get(hamtIndex), hamtHash, depth + 1, key);
            } else {
                auto* hamtList = pointer.getAsLeaf(static_cast<HAMTList*>(nullptr));
                if (hamtList->getHashValue() != hamtHash)
                    return nullptr;
            
                auto* node = hamtList->find(key);
                if (!node)
                    return nullptr;
                return node;
            }
        }

    private:
        HAMT(const HAMT& rhs) = delete;
        HAMT(HAMT&& rhs) = delete;
        HAMT& operator =(const HAMT& rhs) = delete;
        HAMT& operator =(HAMT&& rhs) = delete;

    private:
        TypeAllocator<HAMTNode> nodeAllocator;
        HAMTAllocator pointerAllocator;
        TypeAllocator<HAMTListNode> listNodeAllocator;
        TypeAllocator<HAMTList> listAllocator;

        function<unsigned int(const KeyT&)> hash;

        vector<HAMTPointer> root;
    };

public:
    explicit HashMapHAMT(const function<unsigned int(const KeyT&)>& hash, ValueT dfltValue = ValueT())
        : defaultValue(dfltValue), hamt(hash) {
    }

    const ValueT* find(KeyT key) const {
        auto* p = hamt.find(key);
        if (!p)
            return nullptr;
        return &p->getSecond();
    }

    ValueT* find(KeyT key) {
        auto* p = hamt.find(key);
        if (!p)
            return nullptr;
        return &p->getSecond();
    }


    const ValueT& get(KeyT key) const {
        auto* p = hamt.find(key);
        return !p ? defaultValue : p->getSecond();
    }

    void set(const KeyT& key, const ValueT& value) {
        hamt.insert(key, value);
    }


    const ValueT& operator [](const KeyT& key) const {
        return get(key);
    }

    ValueT& operator [](const KeyT& key) {
        auto* p = hamt.find(key);
        if (!p)
            p = hamt.insert(key, defaultValue);
        return p->getSecond();
    }


private:
    ValueT  defaultValue;
    HAMT    hamt;
};
