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
    //yet to implement here

    }
}