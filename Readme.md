# C++ Thread-Safe LRU Cache with TTL

An industry-standard, header-only Least Recently Used (LRU) cache implementation in C++17. 

This project provides $O(1)$ time complexity for both `get` and `put` operations by combining a doubly linked list with `std::unordered_map`. It is fully thread-safe for concurrent environments and supports optional **Time-To-Live (TTL)** expiration to prevent serving stale data.

## 🚀 Key Features
* **Time-To-Live (TTL) Support:** Employs a highly efficient **Lazy Eviction** strategy to remove expired items automatically using `std::chrono`.
* **Thread-Safe:** Utilizes `std::mutex` to ensure safe concurrent reads and writes across multiple threads.
* **Header-Only:** Zero build configuration required. Just drop `include/lru_cache.hpp` into your project.
* **Template-Driven:** Strictly typed and supports any C++ data types for keys and values.
* **Backward Compatible:** TTL is optional; it functions as a standard capacity-based LRU if no time limit is provided.

## 💻 Quick Start

Because of default arguments, you can instantiate the cache in two different ways depending on your system's needs.

```cpp
#include <iostream>
#include <string>
#include "lru_cache.hpp"

int main() {
    // ---------------------------------------------------------
    // USE CASE 1: Standard LRU (Capacity only)
    // ---------------------------------------------------------
    cache::LRUCache<std::string, int> memory_cache(100);
    memory_cache.put("user_1", 1001); // Stays until capacity is exceeded

    // ---------------------------------------------------------
    // USE CASE 2: LRU with TTL Expiration
    // ---------------------------------------------------------
    // Cache with capacity of 100, where items expire after 5 seconds
    cache::LRUCache<std::string, int> session_cache(100, std::chrono::seconds(5));
    
    session_cache.put("session_token", 42);
    
    // If accessed after 5 seconds, this will return std::nullopt
    if (auto val = session_cache.get("session_token")) {
        std::cout << "Valid session: " << *val << "\n";
    } else {
        std::cout << "Session expired or evicted.\n";
    }

    return 0;
}