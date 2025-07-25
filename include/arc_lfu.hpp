#ifndef ARCLFU_HPP
#define ARCLFU_HPP

#include "arc_node.hpp"
#include "core.hpp"
template<typename Key, typename Value>
class ArcLFUCache{
    private:
        size_t capacity;
        size_t ghost_capacity;
        size_t minFreq;
        std::mutex mutex;
        // unordered_map<Key, NodePtr> main_cache;
        unordered_map<Key, shared_ptr<ArcNode<Key, Value>>> main_cache;
        unordered_map<Key, shared_ptr<ArcNode<Key, Value>>> ghost_cache;
        shared_ptr<ArcNode<Key, Value>> ghost_head, ghost_tail;
        map<size_t, list<shared_ptr<ArcNode<Key, Value>>>> freqMap;
    private:
        void initializeLists(){
            ghost_head = make_shared<ArcNode<Key, Value>>();
            ghost_tail = make_shared<ArcNode<Key, Value>>();
            ghost_head->next = ghost_tail;
            ghost_tail->pre = ghost_head;
        }
        void removeMinFreqNode(){
            if(freqMap.empty()) return;
            
            auto& minFreqList = freqMap[minFreq];
            if(minFreqList.empty()) return;
            shared_ptr<ArcNode<Key, Value>> node = minFreqList.front();
            minFreqList.pop_front();
            if(minFreqList.empty()) {
                freqMap.erase(minFreq);
                if(!freqMap.empty()){
                    minFreq = freqMap.begin()->first;
                }
                else{  //// 新添加
                    minFreq = 0;
                }
            }
            // 将节点移入幽灵缓存
            if(ghost_cache.size() >= ghost_capacity) {
                removeOldestGhost();
            }
            addToGhost(node);
            main_cache.erase(node->getKey());
        }
        bool addNewNode(const Key& key, const Value& val){
            if(main_cache.size() >= capacity) {
                removeMinFreqNode();
            }
            shared_ptr<ArcNode<Key, Value>> node = make_shared<ArcNode<Key, Value>>(key, val);
            main_cache[key] = node;
            if(freqMap.find(1) == freqMap.end()) {
                freqMap[1] = list<shared_ptr<ArcNode<Key, Value>>>();
            }
            freqMap[1].push_back(node);
            minFreq = 1;
            return true;
        }
        void removeNode(shared_ptr<ArcNode<Key, Value>> node){
            if(!node->pre.expired() && node->next) {
                auto _pre = node->pre.lock();
                _pre->next = node->next;
                node->next->pre = _pre;
                node->next = nullptr;
            }
        }
        bool updateNode(shared_ptr<ArcNode<Key, Value>> node, const Value& val){
            node->setValue(val);
            updateNodeFreq(node);
            return true;
        }
        void updateNodeFreq(shared_ptr<ArcNode<Key, Value>> node){
            node->incrementCount();
            size_t _freq = node->getCount();

            auto& oldList = freqMap[_freq-1];
            oldList.remove(node);
            if(oldList.empty()) { 
                freqMap.erase(_freq - 1);
                if(_freq - 1 == minFreq) {
                    minFreq = _freq;
                }
            }
            if(freqMap.find(_freq) == freqMap.end()) {  
                freqMap[_freq] = list<shared_ptr<ArcNode<Key, Value>>>();
            }
            freqMap[_freq].push_back(node);
        }
        void addToGhost(shared_ptr<ArcNode<Key, Value>> node){
            if(ghost_cache.size() == ghost_capacity) {
                removeOldestGhost();
            }
            node->next = ghost_tail;
            node->pre = ghost_tail->pre;
            if(ghost_tail->pre.expired()) {
                ghost_tail->pre.lock()->next = node;
            }   
            ghost_tail->pre = node;
            ghost_cache[node->getKey()] = node;
        }
        void removeOldestGhost(){
            shared_ptr<ArcNode<Key, Value>> node = ghost_head->next;
            if(node != ghost_tail){
                removeNode(node);
                ghost_cache.erase(node->getKey());
            }
        }
        void removeFromGhost(shared_ptr<ArcNode<Key, Value>> node){
            removeNode(node);
        }

    public:
        ArcLFUCache(size_t _capacity): capacity(_capacity), ghost_capacity(_capacity), minFreq(0)
        {
            initializeLists();
        }
        bool put(const Key& key, const Value& val);
        bool get(const Key& key, Value& val);
        bool checkGhost(Key key);
        bool contain(Key key);
        void increaseCapacity();
        bool decreaseCapacity();
};

template class ArcLFUCache<int, int>;
#endif