#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>

using std::swap;

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
        
        ~Node() {
            for (auto& ptr : next) {
                ptr = nullptr;
            }
        }
        
        Node* previous_on_level(size_t in_level) {
            if (in_level >= next.size()) return nullptr;
            Node* current = prev;
            while (current && current->next.size() <= in_level) {
                current = current->prev;
            }
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
    
    size_t random_level() {
        size_t level = 1;
        while (dis(gen) < skip_probability && level < max_level) {
            level++;
        }
        return level;
    }
    
    Node* find_node(const key_T& key) const {
        Node* current = head;
        for (int i = current_max_level - 1; i >= 0; --i) {
            while (current->next[i] && current->next[i]->key < key) {
                current = current->next[i];
            }
        }
        current = current->next[0];
        if (current && current->key == key) {
            return current;
        }
        return nullptr;
    }
    
    std::vector<Node*> get_predecessors(const key_T& key) const {
        std::vector<Node*> predecessors(current_max_level, nullptr);
        Node* current = head;
        for (int i = current_max_level - 1; i >= 0; --i) {
            while (current->next[i] && current->next[i]->key < key) {
                current = current->next[i];
            }
            predecessors[i] = current;
        }
        return predecessors;
    }

public:
    class iterator {
    private:
        skip_list* current_list;
        Node* current_node;
        
    public:
        iterator(skip_list* list, Node* node) : current_list(list), current_node(node) {}
        
        iterator(const iterator& other) : current_list(other.current_list), current_node(other.current_node) {}
        
        operator bool() const {
            return current_node != nullptr;
        }
        
        iterator& operator++() {
            if (current_node) {
                current_node = current_node->next[0];
            }
            return *this;
        }
        
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        iterator& operator--() {
            if (current_node) {
                current_node = current_node->prev;
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }
        
        iterator& operator+=(size_t n) {
            while (n-- > 0 && current_node) {
                current_node = current_node->next[0];
            }
            return *this;
        }
        
        iterator& operator-=(size_t n) {
            while (n-- > 0 && current_node) {
                current_node = current_node->prev;
            }
            return *this;
        }
        
        iterator& operator=(const iterator& other) {
            current_list = other.current_list;
            current_node = other.current_node;
            return *this;
        }
        
        T& operator*() {
            return current_node->value;
        }
        
        T* operator->() {
            return &(current_node->value);
        }
        
        bool operator==(const iterator& other) const {
            return current_node == other.current_node;
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        
        void remove() {
            if (current_node) {
                current_list->remove(current_node->key);
            }
        }
        
        key_T get_key() const {
            return current_node->key;
        }
    };
    
    skip_list(size_t in_max_level = 16, double in_skip_prob = 0.5) 
        : max_level(in_max_level), skip_probability(in_skip_prob), 
          current_max_level(1), element_count(0),
          gen(std::time(0)), dis(0.0, 1.0) {
        head = new Node(key_T(), T(), nullptr, max_level);
        tail = new Node(key_T(), T(), nullptr, max_level);
        for (size_t i = 0; i < max_level; ++i) {
            head->next[i] = tail;
        }
        tail->prev = head;
    }
    
    ~skip_list() {
        clear();
        delete head;
        delete tail;
    }
    
    iterator begin() {
        return iterator(this, head->next[0] != tail ? head->next[0] : nullptr);
    }
    
    iterator end() {
        return iterator(this, nullptr);
    }
    
    void insert(const key_T& key, const T& value) {
        auto predecessors = get_predecessors(key);
        
        if (predecessors[0]->next[0] != tail && predecessors[0]->next[0]->key == key) {
            // Key already exists, update value
            predecessors[0]->next[0]->value = value;
            return;
        }
        
        size_t new_node_level = random_level();
        if (new_node_level > current_max_level) {
            for (size_t i = current_max_level; i < new_node_level; ++i) {
                predecessors[i] = head;
            }
            current_max_level = new_node_level;
        }
        
        Node* new_node = new Node(key, value, predecessors[0], new_node_level);
        
        for (size_t i = 0; i < new_node_level; ++i) {
            new_node->next[i] = predecessors[i]->next[i];
            predecessors[i]->next[i] = new_node;
        }
        
        new_node->prev = predecessors[0];
        if (new_node->next[0] != tail) {
            new_node->next[0]->prev = new_node;
        } else {
            tail->prev = new_node;
        }
        
        element_count++;
    }
    
    void remove(const key_T& key) {
        auto predecessors = get_predecessors(key);
        Node* to_remove = predecessors[0]->next[0];
        
        if (to_remove == tail || to_remove->key != key) {
            return; // Key not found
        }
        
        for (size_t i = 0; i < to_remove->next.size(); ++i) {
            predecessors[i]->next[i] = to_remove->next[i];
        }
        
        if (to_remove->next[0] != tail) {
            to_remove->next[0]->prev = to_remove->prev;
        } else {
            tail->prev = to_remove->prev;
        }
        
        delete to_remove;
        element_count--;
        
        // Update current_max_level if needed
        while (current_max_level > 1 && head->next[current_max_level - 1] == tail) {
            current_max_level--;
        }
    }
    
    iterator find(const key_T& key) {
        Node* node = find_node(key);
        return iterator(this, node);
    }
    
    T& operator[](const key_T& key) {
        Node* node = find_node(key);
        if (!node) {
            insert(key, T());
            node = find_node(key);
        }
        return node->value;
    }
    
    size_t size() const {
        return element_count;
    }
    
    bool empty() const {
        return element_count == 0;
    }
    
    void clear() {
        Node* current = head->next[0];
        while (current != tail) {
            Node* temp = current;
            current = current->next[0];
            delete temp;
        }
        
        for (size_t i = 0; i < max_level; ++i) {
            head->next[i] = tail;
        }
        tail->prev = head;
        
        current_max_level = 1;
        element_count = 0;
    }
};













