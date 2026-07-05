#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <unordered_map>
#include <mutex>
#include <optional>
#include <stdexcept>

namespace cache {

template <typename Key, typename Value>
class LRUCache {
private:
    struct Node {
        Key key;
        Value value;
        Node* prev;
        Node* next;
        
        Node(Key k, Value v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    size_t capacity;
    std::unordered_map<Key, Node*> cache_map;
    Node* head;
    Node* tail;
    
    // Mutable allows locking even in const methods (if we had any)
    mutable std::mutex mtx; 

    // Remove a node from the linked list
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // Insert a node right after the dummy head (mark as most recent)
    void insertToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    explicit LRUCache(size_t cap) : capacity(cap) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
        // Initialize dummy nodes
        head = new Node(Key(), Value());
        tail = new Node(Key(), Value());
        head->next = tail;
        tail->prev = head;
    }

    // Destructor to free allocated memory
    ~LRUCache() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // Delete copy constructor and assignment operator to prevent double-free issues
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    // Retrieve a value by key
    std::optional<Value> get(const Key& key) {
        std::lock_guard<std::mutex> lock(mtx);
        
        if (cache_map.find(key) == cache_map.end()) {
            return std::nullopt; // Key not found
        }
        
        // Move the accessed node to the head (most recently used)
        Node* node = cache_map[key];
        removeNode(node);
        insertToHead(node);
        
        return node->value;
    }

    // Insert or update a key-value pair
    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mtx);
        
        if (cache_map.find(key) != cache_map.end()) {
            // Key exists: update value and move to head
            Node* node = cache_map[key];
            node->value = value;
            removeNode(node);
            insertToHead(node);
        } else {
            // Key does not exist: create new node
            if (cache_map.size() >= capacity) {
                // Cache full: remove the least recently used node (tail->prev)
                Node* lru = tail->prev;
                removeNode(lru);
                cache_map.erase(lru->key);
                delete lru;
            }
            
            Node* newNode = new Node(key, value);
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