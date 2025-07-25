#ifndef LFU_HPP
#define LFU_HPP

#include "core.hpp"

template<typename Key, typename Value> class KLFUCache;

/**
 * 定义固定频率双向链表
 */
template<typename Key, typename Value>
class FreqList{
    private:
        struct Node{
            int freq;
            Key key;
            Value val;
            weak_ptr<Node> pre;
            shared_ptr<Node> next;
            Node() : freq(1), next(nullptr) {}
            Node(Key _key, Value _val) : freq(1), key(_key), val(_val) {}
        };  
        int freq_;
        shared_ptr<Node> head, tail;       
    public:
        explicit FreqList(int _freq) : freq_(_freq) 
        {
            // 定义哨兵头尾节点
            head = make_shared<Node>();
            tail = make_shared<Node>();
            head->next = tail;
            tail->pre = head;
        }
        bool isEmpty() const{ return head->next == tail; }
        void addNode(shared_ptr<Node> node) {
            if(!node || !head || !tail) return;
            node->pre = tail->pre; // 尾插法加入新节点
            node->next = tail;
            tail->pre.lock()->next = node;
            tail->pre = node;
        }
        void removeNode(shared_ptr<Node> node){
            if(!node || !head || !tail) return;
            // 确保当前节点有效且不是哨兵，expired效率高于lock，expired只返回指针状态lock会尝试添加引用计数
            if(!node->pre.expired() || !node->next) return;
            node->pre.lock()->next = node->next;
            node->next->pre = node->pre;
            node->next = nullptr;
        }
        shared_ptr<Node> getFirstNode() const { return head->next; }

        friend class KLFUCache<Key, Value>;
};

template<typename Key, typename Value> 
class KLFUCache{
    private:
        int capacity_;
        int minFreq;
        int maxAvgFreq;
        int curAvgFreq;
        int curtotalFreq;
        unordered_map<Key, shared_ptr<typename FreqList<Key, Value>::Node>> nodemap;
        unordered_map<int, shared_ptr<FreqList<Key, Value>>> freqToFreqList; // 访问频次到频次链表的映射
        std::mutex mutex;
    private:
        void putInternal(Key key, Value val){
            if(nodemap.size() == capacity_) {
                kickNode();
            }
            shared_ptr<typename FreqList<Key, Value>::Node> node = make_shared<typename FreqList<Key, Value>::Node>(key, val);
            nodemap[key] = node;
            addNode(node);
            addFreqNum();
            minFreq = min(1, minFreq);
        }   
        void getInternal(shared_ptr<typename FreqList<Key, Value>::Node> node, Value &val){
            val = node->val;
            // 更新节点在链表队列中的位置
            removeNode(node);
            node->freq++;
            addNode(node);
            // 如果当前节点之前是唯一一个最小频率节点则更新最小频率
            if(node->freq - 1 == minFreq && freqToFreqList[node->freq - 1]->isEmpty()) {
                minFreq++;
            }
            addFreqNum();
        }
        void kickNode(){ // 移除过期数据
            shared_ptr<typename FreqList<Key, Value>::Node> node = freqToFreqList[minFreq]->getFirstNode();
            removeNode(node);
            nodemap.erase(node->key);
            decreaseFreqNum(node->freq);
        }
        void addFreqNum(){
            curtotalFreq ++;
            if(nodemap.empty()){
                curAvgFreq = 0;
            }
            else {
                curAvgFreq = curtotalFreq / nodemap.size();
            }
            if(curAvgFreq > maxAvgFreq){
                handleMaxAvgFreq();
            }
        }
        void decreaseFreqNum(int num){ // 只有移除节点时触发所以要带num参数
            curtotalFreq -= num;
            if(nodemap.empty()) {
                curAvgFreq = 0;
            }
            else{
                curAvgFreq = curtotalFreq / nodemap.size();
            }
        }
        void removeNode(shared_ptr<typename FreqList<Key, Value>::Node> node){ // 将节点从链表删除
            if(!node) return;
            freqToFreqList[node->freq]->removeNode(node);
        }
        void addNode(shared_ptr<typename FreqList<Key, Value>::Node> node){ // 将节点添加到链表
            if(!node) return;
            auto freq = node->freq;
            if(freqToFreqList.find(freq) == freqToFreqList.end()) { // 该频率链表不存在则创建
                freqToFreqList[freq] = make_shared<FreqList<Key, Value>>(freq);
            }
            freqToFreqList[freq]->addNode(node);
        }
        void updateMinFreq(){
            minFreq = INT8_MAX;
            for(auto &pair: freqToFreqList){
                if(pair.second && !pair.second->isEmpty()) { // 该频率链表存在则更新
                    minFreq = min(minFreq, pair.first);
                }
            }
            if(minFreq == INT8_MAX) minFreq = 1;
        }
        void handleMaxAvgFreq(){
            if(nodemap.empty()) return;
            curtotalFreq = 0;
            // lfu优化，当平均freq大于设置的最大freq时所有节点减去最大freq的一半，防止短期热点长期霸占缓存
            for(auto it = nodemap.begin(); it!=nodemap.end(); it++) {
                if(!it->second) return;
                auto node = it->second;
                removeNode(node);
                node->freq -= maxAvgFreq/2;
                node->freq = max(node->freq, 1); // freq变化后最小为1
                curtotalFreq += node->freq;
                addNode(node);
            }
            curAvgFreq = curtotalFreq / nodemap.size(); // 更新总频率和平均频率
            updateMinFreq();    
        }

    public:
        KLFUCache(int capacity, int _maxAvgFreq) : capacity_(capacity), maxAvgFreq(_maxAvgFreq),
                                    minFreq(INT8_MAX), curAvgFreq(0), curtotalFreq(0){}
        ~KLFUCache(){ close(); }
        void put(Key key, Value val);
        bool get(Key key, Value& val);
        Value get(Key key);
        void close(); // 清空缓存
};

template class KLFUCache<int, int>;
#endif