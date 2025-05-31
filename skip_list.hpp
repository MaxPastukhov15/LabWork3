#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>

template <typename key_T, typename T>
class skip_list {
private:
    size_t max_level;
    double skip_probability;
    
    struct Node {
        key_T key;
        T value;
        Node* prev;
        std::vector<Node*> next;
        
        Node(const key_T& in_key, const T& in_value, Node* in_prev, size_t in_level) 
            : key(in_key), value(in_value), prev(in_prev), next(in_level, nullptr) {}
        
        ~Node() { for (auto& ptr : next) ptr = nullptr; }
        
        Node* previous_on_level(size_t in_level) {
            if (in_level >= next.size()) return nullptr;
            Node* current = prev;
            while (current && current->next.size() <= in_level) current = current->prev;
            return current;
        }
        
        Node* next_on_level(size_t in_level) {
            if (in_level >= next.size()) return nullptr;
            return next[in_level];
        }
    };
    
    Node* head;
    Node* tail;
    size_t current_max_level;
    size_t element_count;
    
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
    
    size_t random_level();
    Node* find_node(const key_T& key) const;
    std::vector<Node*> get_predecessors(const key_T& key) const;

public:
    class iterator {
    private:
        skip_list* current_list;
        Node* current_node;
        
    public:
        iterator(skip_list* list, Node* node) : current_list(list), current_node(node) {}
        iterator(const iterator& other) : current_list(other.current_list), current_node(other.current_node) {}
        
        operator bool() const { return current_node != nullptr; }
        iterator& operator++() { if (current_node) current_node = current_node->next[0]; return *this; }
        iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
        iterator& operator--() { if (current_node) current_node = current_node->prev; return *this; }
        iterator operator--(int) { iterator temp = *this; --(*this); return temp; }
        iterator& operator+=(size_t n) { while (n-- > 0 && current_node) current_node = current_node->next[0]; return *this; }
        iterator& operator-=(size_t n) { while (n-- > 0 && current_node) current_node = current_node->prev; return *this; }
        iterator& operator=(const iterator& other) { current_list = other.current_list; current_node = other.current_node; return *this; }
        T& operator*() { return current_node->value; }
        T* operator->() { return &(current_node->value); }
        bool operator==(const iterator& other) const { return current_node == other.current_node; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        void remove() { if (current_node) current_list->remove(current_node->key); }
        key_T get_key() const { return current_node->key; }
    };
    
    skip_list(size_t in_max_level = 16, double in_skip_prob = 0.5);
    ~skip_list();
    
    iterator begin() { return iterator(this, head->next[0] != tail ? head->next[0] : nullptr); }
    iterator end() { return iterator(this, nullptr); }
    void insert(const key_T& key, const T& value);
    void remove(const key_T& key);
    iterator find(const key_T& key) { return iterator(this, find_node(key)); }
    T& operator[](const key_T& key);
    size_t size() const { return element_count; }
    bool empty() const { return element_count == 0; }
    void clear();
};
