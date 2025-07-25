
#include "test.hpp"
using namespace std;

int main(){
    
    test_lru();
    
    test_k_lru();

    test_hash_lru();

    test_lfu();

    test_hash_lfu();

    test_arc();

    return 0;
}