#ifndef HASH_LRU_HPP
#define HASH_LRU_HPP

#include "core.hpp"
#include "lru_k.hpp"
#include "lru.hpp"

/**
 * hash_lru即在lru_k的基础上进行hash判断当前key在哪个lrucache中，
 * 减小lrucache的颗粒度，从而多线程的高并发环境下的线程竞争
 */
template<typename Key, typename Value>
class HashLRUCache{
    public:
        HashLRUCache(size_t _cap, int _sliceNum) :
                            capacity(_cap), slice_num(_sliceNum > 0 ? _sliceNum : thread::hardware_concurrency())
        {
            // 初始化切片lru
            size_t sliceSize = ceil(capacity * 1.0 / slice_num); // 计算切片lru容量
            cout << "sliceNum: " << slice_num << " " << "sliceSize: " << sliceSize << endl;
            for(int i = 0; i < slice_num; i++) {
                hash_lru_cache.emplace_back(new KLRUCache<Key, Value>(2, sliceSize, sliceSize)); 
            }
        }
        void put(Key key, Value val);
        bool get(Key key, Value& val);
        Value get(Key key);
    private:
        size_t capacity;
        int slice_num;
        vector<unique_ptr<KLRUCache<Key, Value>>> hash_lru_cache; // 切片lru缓存
        size_t Hash(Key key){
            hash<Key> hashFunc;
            return hashFunc(key);
        }
};

template class HashLRUCache<int, int>;
#endif