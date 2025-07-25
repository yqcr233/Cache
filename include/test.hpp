#ifndef TEST_HPP
#define TEST_HPP

#include "core.hpp"
#include "lru.hpp"
#include "lru_k.hpp"
#include "hash_lru.hpp"
#include "lfu.hpp"
#include "hash_lfu.hpp"
#include "arc_node.hpp"
#include "arc_lfu.hpp"
#include "arc_lru.hpp"
#include "arc.hpp"

void test_lru();

void test_k_lru();

void test_hash_lru();

void test_lfu();

void test_hash_lfu();

void test_arc();
#endif