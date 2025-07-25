#include "test.hpp"

void test_lru(){
    const int cap = 10;
    LRUCache<int, int> cache(cap);
    // 测试普通添加和获取
    for(int i=0; i<cap; i++) cache.put(i, i+1);
    for(int i=0; i<cap; i++) {
        auto val = cache.get(i);
        cout<<val<<" ";
    }
    cout<<endl;

    // 测试满容量时自动净化缓存
    cache.put(10, 11);
    auto _map = cache.getMap();
    for(auto it: *_map){
        auto node = it.second;
        cout<<node->getValue()<<" ";
    }
    cout<<endl;

    // 测试删除缓存元素
    cache.remove(10);
     for(auto it: *_map){
        auto node = it.second;
        cout<<node->getValue()<<" ";
    }
    cout<<endl;
}

void test_k_lru(){
    KLRUCache<int,int> k_cache{2, 5, 5};
    const int cap = 5;

    // 测试历史缓存lru
    for(int i=0;i<cap;i++) k_cache.put(i, i+1);
    for(int i=0;i<cap;i++) {
        int a = k_cache.get(i);
        cout<<a<<" ";
    }
    cout<<endl;

    // 测试热点数据是否存入主缓存并腾出历史缓存空间
    for(int i = cap; i<2*cap; i++) k_cache.put(i, i+1);
    for(int i=cap; i<2*cap; i++) {
        int a = k_cache.get(i);
        a = k_cache.get(i);
        cout<<a<<" ";
    }
    cout<<endl;
}

void test_hash_lru(){
    HashLRUCache<int, int> hash_cache{100, -1};
    // 初始化随机数
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 100);
    for(int i=0;i<100;i++) {
        int r_idx = dis(gen);
        cout << r_idx << " ";
        hash_cache.put(r_idx, r_idx + 1);
    }
    cout<<endl;

    for(int i=0;i<100;i++) {
        int a = hash_cache.get(i);
        cout << a << " ";
    }
    cout<<endl;
}

void test_lfu(){
    KLFUCache<int, int> cache{10, 3};
    for(int i=0;i<=10;i++) {
        cache.put(i, i+1);
    }
    for(int i=0;i<=10;i++) {
        int a = cache.get(i);
        cout<<a<<" ";
    }
    cout<<endl;
}

void test_hash_lfu(){
    HashLFUCache<int, int> cache{12, 3};
    for(int i=0;i<=12;i++) {
        cache.put(i, i+1);
    }
    for(int i=0;i<=12;i++) {
        int a = cache.get(i);
        cout<<a<<" ";
    }
    cout<<endl;
}   

void test_arc(){
    ArcCache<int, int> cache{24, 3};
    for(int i=0;i<=24;i++) {
        cache.put(i, i+1);
    }   
    for(int i=0;i<=24;i++) {
        int a = cache.get(i);
        cout<<a<<" ";
    }
    cout<<endl;
}