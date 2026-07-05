# C++ Thread-Safe LRU Cache

An industry-standard, header-only Least Recently Used (LRU) cache implementation in C++17. 

Provides $O(1)$ time complexity for both `get` and `put` operations by combining a doubly linked list with `std::unordered_map`. It is fully thread-safe, utilizing `std::mutex` for concurrent access.

## Features
* **Header-only**: Just drop `include/lru_cache.hpp` into your project.
* **Template-based**: Supports any data types for keys and values.
* **Thread-safe**: Safe for concurrent reads and writes.
* **No external dependencies** (except Google Test for unit tests).

## Usage
```cpp
#include "lru_cache.hpp"

// Initialize with capacity of 100
cache::LRUCache<std::string, int> cache(100);

cache.put("key1", 42);
auto value = cache.get("key1"); // Returns std::optional<int>