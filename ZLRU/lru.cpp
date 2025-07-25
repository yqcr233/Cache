#include "lru.hpp"

template<typename Key, typename Value>
void LRUCache<Key, Value>::put(Key key, Value val){
    if(capacity <= 0) return;

    lock_guard<std::mutex> lock(mutex);
    auto it = nodemap.find(key);
    if(it != nodemap.end()){
        updateNode(it->second, val);
        return;
    }
    addNode(key, val);
}

template<typename Key, typename Value>
bool LRUCache<Key, Value>::get(Key key, Value& val){
    lock_guard<std::mutex> lock(mutex);
    auto it = nodemap.find(key);
    if(it != nodemap.end()){
        moveToRecent(it->second);
        val = it->second->getValue();
        return true;
    }
    return false;
}

template<typename Key, typename Value>
Value LRUCache<Key, Value>::get(Key key){
    Value val{};
    get(key, val);
    return val;
}

template<typename Key, typename Value>
void LRUCache<Key, Value>::remove(Key key){
    lock_guard<std::mutex> lock(mutex);
    auto it = nodemap.find(key);
    if(it != nodemap.end()){
        removeNode(it->second);
        nodemap.erase(it);
    }
}