#ifndef ARCLRU_HPP
#define ARCLRU_HPP

#include "core.hpp"
#include "arc_node.hpp"

template<typename Key, typename Value>
class ArcLRUCache{
    private:
        size_t capacity, ghost_capacity, transThreshold;
        unordered_map<Key, shared_ptr<ArcNode<Key, Value>>> main_cache, ghost_cache;
        shared_ptr<ArcNode<Key, Value>> main_head, main_tail;
        shared_ptr<ArcNode<Key, Value>> ghost_head, ghost_tail;
        std::mutex mutex;
    private:
        void initializeLists(){
            // 初始化主lru缓存和幽灵lru缓存
            main_head = make_shared<ArcNode<Key, Value>>();
            main_tail = make_shared<ArcNode<Key, Value>>();
            ghost_head = make_shared<ArcNode<Key, Value>>();
            ghost_tail = make_shared<ArcNode<Key, Value>>();

            main_head->next = main_tail;
            main_tail->pre = main_head;
            ghost_head->next = ghost_tail;
            ghost_tail->pre = ghost_head;
        }
        bool addNewNode(const Key& key, const Value& val){
            if(main_cache.size() >= capacity) {
                removeOldestMain();
            }
            shared_ptr<ArcNode<Key, Value>> node = make_shared<ArcNode<Key, Value>>(key, val);
            main_cache[key] = node;
            addToFront(node);
            return true;
        }
        bool updateNode(shared_ptr<ArcNode<Key, Value>> node, const Value &val){
            node->setValue(val);
            // moveToFront(node);
            return updateNodeAccess(node);
        }
        bool updateNodeAccess(shared_ptr<ArcNode<Key, Value>> node){
            moveToFront(node);
            node->incrementCount();
            return node->getCount() >= transThreshold;
        }
        void moveToFront(shared_ptr<ArcNode<Key, Value>> node){
            removeFromMain(node);
            addToFront(node);
        }
        void addToFront(shared_ptr<ArcNode<Key, Value>> node){
            node->next = main_head->next;
            node->pre = main_head;
            main_head->next->pre = node;
            main_head->next = node;
        }
        void removeNode(shared_ptr<ArcNode<Key, Value>> node){
            if(!node->pre.expired() && node->next) {
                auto _pre = node->pre.lock();
                _pre->next = node->next;
                node->next->pre = _pre;
                node->next = nullptr;
            }           
        }
        void removeFromMain(shared_ptr<ArcNode<Key, Value>> node){ removeNode(node); }
        void removeOldestMain(){
            shared_ptr<ArcNode<Key, Value>> node = main_tail->pre.lock();
            if(!node || node == main_head) {
                return;
            }
            removeFromMain(node);
            if(ghost_cache.size() >= ghost_capacity) {
                removeOldestGhost();
            } 
            addToGhost(node);
            main_cache.erase(node->getKey());
        }
        void addToGhost(shared_ptr<ArcNode<Key, Value>> node){
            node->accessCount = 1; // 重置节点的访问计数
            // 将节点头插入幽灵列表和幽灵映射列表 
            node->next = ghost_head->next;
            node->pre = ghost_head;
            ghost_head->next->pre = node;
            ghost_head->next = node;
            ghost_cache[node->getKey()] = node;
        }
        void removeFromGhost(shared_ptr<ArcNode<Key, Value>> node){ removeNode(node); }
        void removeOldestGhost(){
            shared_ptr<ArcNode<Key, Value>> node = ghost_tail->pre.lock();
            if(!node || node == ghost_head) {
                return;
            }
            removeFromGhost(node);
            ghost_cache.erase(node->getKey());
        }
    public:
        ArcLRUCache(size_t _capacity, size_t _transThreshold):
                    capacity(_capacity), transThreshold(_transThreshold), ghost_capacity(_capacity)
        {
            initializeLists();
        }
        bool put(Key key, Value val, bool &shouldTransfrom);
        bool get(Key key, Value &val, bool &shouldTransfrom);
        bool checkGhost(Key key);
        void increaseCapacity();
        bool decreaseCapacity();
};

template class ArcLRUCache<int, int>;
#endif
