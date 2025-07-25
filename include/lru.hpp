#ifndef LRU_HPP
#define LRU_HPP

#include "core.hpp"

template<typename Key, typename Value> class LRUCache;

template<typename Key, typename Value>
class LRUNode{
    public:
        LRUNode(Key _key, Value _val) : key(_key), val(_val), access_count(1){}
        ~LRUNode() = default;

        Key getKey() const { return key; }
        Value getValue() const { return val; }
        void setValue(const Value& _val) { val = _val; }
        size_t getCount() const { return access_count; }
        void addCount() { access_count++; }
        friend class LRUCache<Key, Value>;
    private:
        Key key;
        Value val;
        size_t  access_count;
        weak_ptr<LRUNode<Key, Value>> pre; // 设置成弱指针避免循环引用
        shared_ptr<LRUNode<Key, Value>> next;
};

template<typename Key, typename Value>
class LRUCache{
    public:
        LRUCache(int _capacity = 10) : capacity(_capacity) { initList(); }
        ~LRUCache() = default;
        void put(Key key, Value val);
        bool get(Key key, Value& val);
        Value get(Key key);
        void remove(Key key);
        int getCap(){ return nodemap.size(); }
        unordered_map<Key, shared_ptr<LRUNode<Key, Value>>>* getMap(){
            return &nodemap;
        }

    private:
        void initList(){
            head = make_shared<LRUNode<Key, Value>>(Key(), Value());
            tail = make_shared<LRUNode<Key, Value>>(Key(), Value());
            head->next = tail;
            tail->pre = head;  
        }
        void updateNode(shared_ptr<LRUNode<Key, Value>> node, const Value& val){
            node->setValue(val);
            moveToRecent(node);
            return;
        }
        void addNode(const Key& key, const Value& val){
            if(nodemap.size() >= capacity) 
                removeLeastRecent();
            shared_ptr<LRUNode<Key, Value>> node = make_shared<LRUNode<Key, Value>>(key, val);
            insertNode(node);
            nodemap[key] = node;
        }
        void removeNode(shared_ptr<LRUNode<Key, Value>> node){
            // 只会出现删除表头的情况
            if(node->pre.lock() == nullptr || node->next == nullptr) return; // 确保节点不是哨兵节点
            auto prev = node->pre.lock(); // 使用lock获取pre节点的shared_ptr,因为weak_ptr只是为了打破循环不能访问对象
            prev->next = node->next;
            node->next->pre = prev;
            node->next = nullptr;
        }
        void insertNode(shared_ptr<LRUNode<Key, Value>> node){
            node->next = tail;
            node->pre = tail->pre;
            tail->pre.lock()->next = node;
            tail->pre = node;
        }

        void moveToRecent(shared_ptr<LRUNode<Key, Value>> node){
            removeNode(node);
            insertNode(node);
        }
        void removeLeastRecent(){
            shared_ptr<LRUNode<Key, Value>> node = head->next;
            removeNode(node);
            nodemap.erase(node->getKey());
        }

    private:
        uint32_t  capacity;
        unordered_map<Key, shared_ptr<LRUNode<Key, Value>>> nodemap; // 哈希表，便于查找节点
        std::mutex mutex;
        // 头尾节点用于维护双向链表，管理节点生命周期
        shared_ptr<LRUNode<Key, Value>> head; 
        shared_ptr<LRUNode<Key, Value>> tail;
};

template class LRUCache<int, int>;
template class LRUCache<int, unsigned long long>;
#endif