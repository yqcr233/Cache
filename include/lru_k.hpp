#ifndef LRUK_HPP
#define LRUK_HPP

#include "core.hpp"
#include "lru.hpp"

template<typename Key, typename Value>
class KLRUCache : public LRUCache<Key, Value>{
    public:
        KLRUCache(int _k, int _capacity, int h_capacity):
                    k(_k), LRUCache<Key, Value>(_capacity), h_cache(make_unique<LRUCache<Key, size_t>>(h_capacity)){}
        
        Value get(Key key); // 对原LRUCache中同名函数进行隐藏，可通过添加作用域来调用原get函数
        void put(Key key, Value val);

    private:
        int k; // 缓存队列门槛
        // 这里只在map中存Value，lru中只存次数，当到达门槛才以节点形式存入主缓存
        unique_ptr<LRUCache<Key, size_t>> h_cache; 
        unordered_map<Key, Value> h_map;
};

template class KLRUCache<int, int>;
#endif