#include "hash_lfu.hpp"

template<typename Key, typename Value>
void HashLFUCache<Key, Value>::put(Key key, Value val){
    size_t slice_idx = Hash(key) % sliceNum;
    lfu_cache[slice_idx]->put(key, val);
}

template<typename Key, typename Value>
bool HashLFUCache<Key, Value>::get(Key key, Value &val){
    size_t slice_idx = Hash(key) %sliceNum;
    return lfu_cache[slice_idx]->get(key, val);
}

template<typename Key, typename Value>
Value HashLFUCache<Key, Value>::get(Key key){
    Value val{};
    get(key, val);
    return val;
}

template<typename Key, typename Value>
void HashLFUCache<Key, Value>::close(){
    for(auto& a: lfu_cache) {
        a->close();
    }
}

template class HashLFUCache<int, int>;