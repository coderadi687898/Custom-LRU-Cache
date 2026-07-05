#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <unordered_map>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <chrono> // Added for TTL

namespace cache {

template <typename Key, typename Value>
class LRUCache {
private:
    struct Node {
        Key key;
        Value value;
        // Store the exact time this node was created/updated
        std::chrono::steady_clock::time_point timestamp; 
        Node* prev;
        Node* next;
        
        Node(Key k, Value v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    size_t capacity;
    std::chrono::milliseconds ttl; // Cache-wide TTL setting
    std::unordered_map<Key, Node*> cache_map;
    Node* head;
    Node* tail;
    mutable std::mutex mtx; 

    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insertToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    // Added an optional TTL parameter (default is 0 = no expiration)
    explicit LRUCache(size_t cap, std::chrono::milliseconds ttl_duration = std::chrono::milliseconds::zero()) 
        : capacity(cap), ttl(ttl_duration) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
        head = new Node(Key(), Value());
        tail = new Node(Key(), Value());
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    std::optional<Value> get(const Key& key) {
        std::lock_guard<std::mutex> lock(mtx);
        
        if (cache_map.find(key) == cache_map.end()) {
            return std::nullopt; 
        }
        
        Node* node = cache_map[key];

        // --- TTL EXPIRATION CHECK ---
        if (ttl > std::chrono::milliseconds::zero()) {
            auto now = std::chrono::steady_clock::now();
            if (now - node->timestamp > ttl) {
                // Item has expired! Delete it.
                removeNode(node);
                cache_map.erase(key);
                delete node;
                return std::nullopt;
            }
        }
        
        // Item is still valid. Move to head.
        removeNode(node);
        insertToHead(node);
        
        return node->value;
    }

    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mtx);
        
        if (cache_map.find(key) != cache_map.end()) {
            Node* node = cache_map[key];
            node->value = value;
            // Reset the expiration timer since it was just updated
            node->timestamp = std::chrono::steady_clock::now(); 
            removeNode(node);
            insertToHead(node);
        } else {
            if (cache_map.size() >= capacity) {
                Node* lru = tail->prev;
                removeNode(lru);
                cache_map.erase(lru->key);
                delete lru;
            }
            
            Node* newNode = new Node(key, value);
            // Set the creation timer
            newNode->timestamp = std::chrono::steady_clock::now();
            insertToHead(newNode);
            cache_map[key] = newNode;
        }
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return cache_map.size();
    }
};

} // namespace cache

#endif // LRU_CACHE_HPP