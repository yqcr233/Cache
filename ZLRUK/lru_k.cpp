#include "lru_k.hpp"

template<typename Key, typename Value>
Value KLRUCache<Key, Value>::get(Key key){
    // 更新访问计数
    size_t h_count = h_cache->get(key); // 如果不存在返回0
    h_count ++;
    h_cache->put(key, h_count);

    // 先在主缓存中查看
    Value val{};
    bool in_main_cache = LRUCache<Key, Value>::get(key, val);
    if(in_main_cache) {
        return val;
    }

    // 不在主缓存但达到热点数据门槛
    // if(h_count >= k){
    //     // 判断历史缓存中是否有值
    //     auto it = h_map.find(key);
    //     if(it != h_map.end()){
    //         Value _val = it->second;
    //         // 移除历史缓存到主缓存
    //         h_cache->remove(key);
    //         h_map.erase(it);
    //         LRUCache<Key, Value>::put(key, _val);
    //         return _val;
    //     }
    // }
    auto it = h_map.find(key);
    if(it != h_map.end()) {
        val = it->second;
        if(h_count >= k) {
            h_cache->remove(key);
            h_map.erase(it);
            LRUCache<Key, Value>::put(key, val);
        }
    }
    return val;
}

template<typename Key, typename Value>
void KLRUCache<Key, Value>::put(Key key, Value val){
    // 检查主缓存是否存在元素
    Value _val{};
    bool in_main_cache = LRUCache<Key, Value>::get(key, _val);
    if(in_main_cache) { // 主缓存key存在则直接更新缓存
        LRUCache<Key, Value>::put(key, val);
        return;
    }       

    // 更新历史
    size_t h_count = h_cache->get(key);
    h_count ++;
    h_cache->put(key, h_count);
    h_map[key] = val;

    // 到达热点数据门槛，则加入主缓存
    if(h_count >= k){
        h_cache->remove(key);
        h_map.erase(key);
        LRUCache<Key, Value>::put(key, val);
    }
}

template class KLRUCache<int, int>;