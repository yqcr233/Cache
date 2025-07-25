#ifndef ARCNODE_HPP
#define ARCNODE_HPP

#include "core.hpp"

template<typename Key, typename Value>
class ArcNode{
    private:
        Key key;
        Value val;
        size_t accessCount;
        weak_ptr<ArcNode> pre;
        shared_ptr<ArcNode> next;
    public:
        ArcNode() : accessCount(1), next(nullptr){}
        ArcNode(Key _key, Value _val) : key(_key), val(_val), accessCount(1), next(nullptr) {}

        Key getKey() const { return key; }
        Value getValue() const { return val; }
        size_t getCount() const { return accessCount; }
        void setValue(const Value& _val) { val = _val; }
        void incrementCount() { ++accessCount; }

        // 声明友元时，不需要完整定义，只需要友元类的声明即可
        template<typename K, typename V> friend class ArcLRUCache;
        template<typename K, typename V> friend class ArcLFUCache;
};

#endif