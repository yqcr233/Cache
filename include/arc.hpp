#ifndef ARC_HPP
#define ARC_HPP

#include "core.hpp"
#include "arc_lfu.hpp"
#include "arc_lru.hpp"

template<typename Key, typename Value>
class ArcCache{
    private:
        size_t capacity;
        size_t transThreshold;
        unique_ptr<ArcLFUCache<Key, Value>> arcLfuCache;
        unique_ptr<ArcLRUCache<Key, Value>> arcLruCache;
    private:
        bool checkGhostCache(Key key){
            bool isGhost = false;
            if(arcLfuCache->checkGhost(key)) {
                if(arcLruCache->decreaseCapacity()) {
                    arcLfuCache->increaseCapacity();
                }
                isGhost = true;
            } else if(arcLruCache->checkGhost(key)) {
                if(arcLfuCache->decreaseCapacity()){
                    arcLruCache->increaseCapacity();
                }
                isGhost = true;
            }
            return isGhost; 
        }
    public:
        ArcCache(size_t _capacity, size_t _transThreshold): capacity(_capacity), transThreshold(_transThreshold)
        {
            arcLfuCache = make_unique<ArcLFUCache<Key, Value>>(capacity / 2);
            arcLruCache = make_unique<ArcLRUCache<Key, Value>>(capacity / 2, transThreshold);
        }
        void put(const Key& key, const Value& val);
        bool get(Key key, Value& val);
        Value get(Key key);
};

template class ArcCache<int, int>;
#endif