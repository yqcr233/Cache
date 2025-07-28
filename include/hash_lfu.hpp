#ifndef HASH_LFU_HPP
#define HASH_LFU_HPP

#include "core.hpp"
#include "lfu.hpp"

/**
 * 原理和hash_lru一样，为了降低缓存的颗粒度，将原缓存划分为若干个切片缓存，
 * 通过哈希将节点映射到不同的切片缓存中。
 */
template<typename Key, typename Value>
class HashLFUCache{
    private:
        size_t capacity_;
        int sliceNum;
        vector<unique_ptr<KLFUCache<Key, Value>>> lfu_cache;
    private:
        size_t Hash(Key key) {
            hash<Key> hashFunc;
            return hashFunc(key);
        }
    public:
        HashLFUCache(size_t capacity, int _sliceNum, int maxAvgNum = 10):
                        capacity_(capacity), sliceNum(_sliceNum > 0 ? _sliceNum : thread::hardware_concurrency()) 
        {
            size_t sliceSize = ceil(capacity_ * 1.0 / sliceNum);
            for(int i=0; i<sliceNum; i++) {
                lfu_cache.emplace_back(new KLFUCache<Key, Value>(sliceSize, maxAvgNum));
            }
        }
        void put(Key key, Value val);
        bool get(Key key, Value &val);
        Value get(Key key);
        void close();
};

extern template class HashLFUCache<int, int>;

#endif