#include "arc_lru.hpp"

template<typename Key, typename Value>
bool ArcLRUCache<Key, Value>::put(Key key, Value val, bool &shouldTransfrom){
    if(capacity <= 0) return false;
    lock_guard<std::mutex> lock(mutex);
    auto it = main_cache.find(key);
    if(it != main_cache.end()) {
        shouldTransfrom = updateNode(it->second, val);
        return true;    
    }
    return addNewNode(key, val);
}
template<typename Key, typename Value>
bool ArcLRUCache<Key, Value>::get(Key key, Value &val, bool &shouldTransfrom){
    lock_guard<std::mutex> lock(mutex);
    auto it = main_cache.find(key);
    if(it != main_cache.end()) {
        shouldTransfrom = updateNodeAccess(it->second);
        val = it->second->getValue();
        return true;
    }
    return false;
}
template<typename Key, typename Value>
bool ArcLRUCache<Key, Value>::checkGhost(Key key){ // 暂时不知道是干什么的
    auto it = ghost_cache.find(key);
    if(it != ghost_cache.end()){
        removeFromGhost(it->second);
        ghost_cache.erase(it);
        return true;
    }
    return false;
}
template<typename Key, typename Value>
void ArcLRUCache<Key, Value>::increaseCapacity(){ ++capacity; }
template<typename Key, typename Value>
bool ArcLRUCache<Key, Value>::decreaseCapacity(){
    if(capacity <= 0) 
        return false;
    if(main_cache.size() == capacity) 
        removeOldestMain();
    --capacity;
    return true;
}