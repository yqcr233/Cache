#include "lfu.hpp"

template<typename Key, typename Value>
void KLFUCache<Key, Value>::put(Key key, Value val){
    if(capacity_ == 0) return;
    lock_guard<std::mutex> lock(mutex);
    auto it = nodemap.find(key);
    if(it != nodemap.end()) {
        it->second->val = val;
        getInternal(it->second, val); // 如果该key已存在则直接更新
        return;
    }
    putInternal(key, val); // key不存在则进行添加
}
template<typename Key, typename Value>
bool KLFUCache<Key, Value>::get(Key key, Value& val){
    lock_guard<std::mutex> lock(mutex);
    auto it = nodemap.find(key);
    if(it != nodemap.end()) {
        getInternal(it->second, val);
        return true;
    }
    return false;
}
template<typename Key, typename Value>
Value KLFUCache<Key, Value>::get(Key key){
    Value val{};
    get(key, val);
    return val;
}
template<typename Key, typename Value>
void KLFUCache<Key, Value>::close(){
    nodemap.clear();
    freqToFreqList.clear();
}