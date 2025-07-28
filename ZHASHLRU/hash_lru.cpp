#include "hash_lru.hpp"

template<typename Key, typename Value>
void HashLRUCache<Key, Value>::put(Key key, Value val){
    size_t hash_idx = Hash(key) % slice_num;
    hash_lru_cache[hash_idx]->put(key, val);
}

template<typename Key, typename Value>
bool HashLRUCache<Key, Value>::get(Key key, Value& val){
    size_t hash_idx = Hash(key) % slice_num;
    return hash_lru_cache[hash_idx]->LRUCache<Key, Value>::get(key, val);
}

template<typename Key, typename Value>
Value HashLRUCache<Key, Value>::get(Key key){
    Value val{};
    size_t hash_idx = Hash(key) % slice_num;
    return hash_lru_cache[hash_idx]->get(key);
}

template class HashLRUCache<int, int>;