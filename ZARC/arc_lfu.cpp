#include "arc_lfu.hpp"

template<typename Key, typename Value>
bool ArcLFUCache<Key, Value>::put(const Key& key, const Value& val){
    if(capacity == 0) return false;
    lock_guard<std::mutex> lock(mutex);
    auto it = main_cache.find(key);
    if(it != main_cache.end()) {
        return updateNode(it->second, val);
    }
    return addNewNode(key, val);
}
template<typename Key, typename Value>
bool ArcLFUCache<Key, Value>::get(const Key& key, Value& val){
    lock_guard<std::mutex> lock(mutex);
    auto it = main_cache.find(key);
    if(it != main_cache.end()) {
        updateNodeFreq(it->second);
        val = it->second->getValue();
        return true;
    }
    return false;
}
template<typename Key, typename Value>
bool ArcLFUCache<Key, Value>::checkGhost(Key key){
    auto it = ghost_cache.find(key);
    if(it != ghost_cache.end()) {
        removeFromGhost(it->second);
        ghost_cache.erase(it);
        return true;
    }
    return false;
}
template<typename Key, typename Value>
void ArcLFUCache<Key, Value>::increaseCapacity(){
    ++capacity;
}
template<typename Key, typename Value>
bool ArcLFUCache<Key, Value>::decreaseCapacity(){
    if(capacity <= 0) return false;
    if(main_cache.size() == capacity) {
        removeMinFreqNode();
    }
    --capacity; 
    return true;
}
template<typename Key, typename Value>
bool ArcLFUCache<Key, Value>::contain(Key key){
    return main_cache.find(key) != main_cache.end();
}

template class ArcLFUCache<int, int>;