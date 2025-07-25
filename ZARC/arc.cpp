#include "arc.hpp"

template<typename Key, typename Value>
void ArcCache<Key, Value>::put(const Key& key, const Value& val){
    checkGhostCache(key);
    bool influ = arcLfuCache->contain(key);
    bool shouldTrans = false;
    arcLruCache->put(key, val, shouldTrans);
    if(influ || shouldTrans) {
        arcLfuCache->put(key, val);
    }
}
template<typename Key, typename Value>
bool ArcCache<Key, Value>::get(Key key, Value& val){
    checkGhostCache(key);
    bool shouldTrans = false;
    if(arcLruCache->get(key, val, shouldTrans)) {
        if(shouldTrans) {
            arcLfuCache->put(key, val);
        }
        return true;
    }
    return arcLfuCache->get(key, val);
}
template<typename Key, typename Value>
Value ArcCache<Key, Value>::get(Key key){
    Value val{};
    get(key, val);
    return val;
}