#include <iostream>
#include <string>
#include "lru_cache.hpp"

int main() {
    // Create a cache with capacity of 3
    cache::LRUCache<std::string, int> user_sessions(3);

    std::cout << "Adding users to cache...\n";
    user_sessions.put("Alice", 101);
    user_sessions.put("Bob", 102);
    user_sessions.put("Charlie", 103);

    // Access Alice so she becomes the most recently used
    if (auto val = user_sessions.get("Alice")) {
        std::cout << "Alice's ID: " << *val << "\n";
    }

    std::cout << "Adding Dave (Cache is full, least recently used will be evicted)...\n";
    user_sessions.put("Dave", 104);

    // Bob should be evicted because we accessed Alice, and Charlie was added after Bob
    if (!user_sessions.get("Bob").has_value()) {
        std::cout << "Bob was successfully evicted from the cache.\n";
    }

    return 0;
}