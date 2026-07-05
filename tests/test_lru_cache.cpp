#include <gtest/gtest.h>
#include "lru_cache.hpp"

using namespace cache;

// Test basic Put and Get operations
TEST(LRUCacheTest, BasicPutAndGet) {
    LRUCache<int, int> cache(2);
    
    cache.put(1, 100);
    cache.put(2, 200);
    
    EXPECT_EQ(cache.get(1).value(), 100);
    EXPECT_EQ(cache.get(2).value(), 200);
    EXPECT_FALSE(cache.get(3).has_value()); // Key 3 doesn't exist
}

// Test that the cache evicts the least recently used item when full
TEST(LRUCacheTest, EvictionLogic) {
    LRUCache<int, int> cache(2);
    
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300); // This should evict key 1 (least recently used)
    
    EXPECT_FALSE(cache.get(1).has_value()); 
    EXPECT_EQ(cache.get(2).value(), 200);
    EXPECT_EQ(cache.get(3).value(), 300);
}

// Test that accessing an item updates its "recently used" status
TEST(LRUCacheTest, LRUPromotionOnAccess) {
    LRUCache<int, int> cache(2);
    
    cache.put(1, 100);
    cache.put(2, 200);
    
    // Access key 1, making it the most recently used
    cache.get(1); 
    
    cache.put(3, 300); // This should now evict key 2, not key 1
    
    EXPECT_TRUE(cache.get(1).has_value());
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_EQ(cache.get(3).value(), 300);
}

// Test updating an existing key
TEST(LRUCacheTest, UpdateExistingKey) {
    LRUCache<int, int> cache(2);
    
    cache.put(1, 100);
    cache.put(1, 150); // Update value for key 1
    
    EXPECT_EQ(cache.get(1).value(), 150);
    EXPECT_EQ(cache.size(), 1); // Size should still be 1
}