#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>

template <typename key_T, typename T>
class skip_list {
private:
    // Node structure representing elements in the skip list
    struct Node {
        key_T key;               // The key used for ordering
        T value;                 // The stored value
        Node* prev;              // Pointer to previous node
        std::vector<Node*> next; // Vector of forward pointers (one for each level)

        // Constructor initializes node with given parameters
        Node(const key_T& in_key, const T& in_value, Node* in_prev, size_t in_level);
        
        // Destructor
        ~Node();

        // Returns the previous node at a specific level
        Node* previous_on_level(size_t in_level);
        
        // Returns the next node at a specific level
        Node* next_on_level(size_t in_level);
    };

    // Skip list member variables
    Node* head;                // Pointer to head sentinel node
    Node* tail;                // Pointer to tail sentinel node
    size_t max_level;          // Maximum allowed levels in the skip list
    double skip_probability;   // Probability for promoting a node to higher levels
    size_t current_max_level;  // Current highest level in use
    size_t element_count;      // Number of elements in the skip list

    // Random number generation for determining node levels
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    // Generates a random level for new nodes
    size_t random_level();
    
    // Finds a node with the given key (internal implementation)
    Node* find_node(const key_T& key) const;
    
    // Gets all predecessor nodes for insertion/deletion
    std::vector<Node*> get_predecessors(const key_T& key) const;

public:
    // Iterator class for traversing the skip list
    class iterator {
    private:
        skip_list* current_list;  // Pointer to the containing skip list
        Node* current_node;       // Pointer to current node

    public:
        // Constructor
        iterator(skip_list* list, Node* node);
        
        // Copy constructor
        iterator(const iterator& other);

        // Conversion to bool (true if iterator points to valid element)
        operator bool() const;
        
        // Prefix increment
        iterator& operator++();
        
        // Postfix increment
        iterator operator++(int);
        
        // Prefix decrement
        iterator& operator--();
        
        // Postfix decrement
        iterator operator--(int);
        
        // Compound addition
        iterator& operator+=(size_t n);
        
        // Compound subtraction
        iterator& operator-=(size_t n);
        
        // Assignment operator
        iterator& operator=(const iterator& other);
        
        // Dereference operator
        T& operator*();
        
        // Member access operator
        T* operator->();
        
        // Equality comparison
        bool operator==(const iterator& other) const;
        
        // Inequality comparison
        bool operator!=(const iterator& other) const;
        
        // Remove current element
        void remove();
        
        // Get current key
        key_T get_key() const;
    };

    // Constructor with configurable max level and skip probability
    skip_list(size_t in_max_level = 16, double in_skip_prob = 0.5);
    
    // Destructor
    ~skip_list();

    // Returns iterator to first element
    iterator begin();
    
    // Returns iterator representing end of list
    iterator end();

    // Inserts a new element or updates existing one
    void insert(const key_T& key, const T& value);
    
    // Removes element with specified key
    void remove(const key_T& key);
    
    // Finds element with specific key
    iterator find(const key_T& key);
    
    // Accesses element with bounds checking (inserts if not found)
    T& operator[](const key_T& key);

    // Returns number of elements
    size_t size() const;
    
    // Checks if list is empty
    bool empty() const;
    
    // Removes all elements
    void clear();
};

// Node constructor implementation
template <typename key_T, typename T>
skip_list<key_T, T>::Node::Node(const key_T& in_key, const T& in_value, Node* in_prev, size_t in_level)
    : key(in_key), value(in_value), prev(in_prev), next(in_level, nullptr) {}

// Node destructor implementation
template <typename key_T, typename T>
skip_list<key_T, T>::Node::~Node() {}

// Finds previous node at specific level
template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, T>::Node::previous_on_level(size_t in_level) {
    if (in_level >= next.size()) return nullptr;  // Level exceeds node's height
    Node* current = prev;
    // Traverse backwards until finding a node that exists at this level
    while (current && current->next.size() <= in_level) {
        current = current->prev;
    }
    return current;
}

// Gets next node at specific level
template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, T>::Node::next_on_level(size_t in_level) {
    if (in_level >= next.size()) return nullptr;  // Level exceeds node's height
    return next[in_level];
}

// Iterator constructor implementation
template <typename key_T, typename T>
skip_list<key_T, T>::iterator::iterator(skip_list* list, Node* node)
    : current_list(list), current_node(node) {}

// Iterator copy constructor implementation
template <typename key_T, typename T>
skip_list<key_T, T>::iterator::iterator(const iterator& other)
    : current_list(other.current_list), current_node(other.current_node) {}

// Iterator bool conversion implementation
template <typename key_T, typename T>
skip_list<key_T, T>::iterator::operator bool() const {
    // Valid if pointing to a real node (not null, head, or tail)
    return current_node != nullptr && current_node != current_list->head && current_node != current_list->tail;
}

// Prefix increment implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator++() {
    if (current_node) {
        current_node = current_node->next[0];  // Move to next node at base level
        if (current_node && current_node == current_list->tail) {
            current_node = nullptr;  // Reached end of list
        }
    }
    return *this;
}

// Postfix increment implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

// Prefix decrement implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator--() {
    if (current_node) {
        current_node = current_node->prev;  // Move to previous node
        if (current_node && current_node == current_list->head) {
            current_node = nullptr;  // Reached beginning of list
        }
    } else {
        // Special case: decrementing end() iterator
        current_node = current_list->tail->prev;
        if (current_node == current_list->head) {
            current_node = nullptr;  // List was empty
        }
    }
    return *this;
}

// Postfix decrement implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::iterator::operator--(int) {
    iterator temp = *this;
    --(*this);
    return temp;
}

// Compound addition implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator+=(size_t n) {
    // Move forward n times at base level
    while (n-- > 0 && current_node) {
        current_node = current_node->next[0];
    }
    return *this;
}

// Compound subtraction implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator-=(size_t n) {
    // Move backward n times
    while (n-- > 0 && current_node) {
        current_node = current_node->prev;
    }
    return *this;
}

// Assignment operator implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator=(const iterator& other) {
    current_list = other.current_list;
    current_node = other.current_node;
    return *this;
}

// Dereference operator implementation
template <typename key_T, typename T>
T& skip_list<key_T, T>::iterator::operator*() {
    if (!current_node) throw std::out_of_range("Dereferencing end iterator");
    return current_node->value;
}

// Member access operator implementation
template <typename key_T, typename T>
T* skip_list<key_T, T>::iterator::operator->() {
    if (!current_node) throw std::out_of_range("Dereferencing end iterator");
    return &(current_node->value);
}

// Equality operator implementation
template <typename key_T, typename T>
bool skip_list<key_T, T>::iterator::operator==(const iterator& other) const {
    return current_node == other.current_node;
}

// Inequality operator implementation
template <typename key_T, typename T>
bool skip_list<key_T, T>::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

// Remove current element implementation
template <typename key_T, typename T>
void skip_list<key_T, T>::iterator::remove() {
    if (current_node) {
        current_list->remove(current_node->key);
    }
}

// Get key implementation
template <typename key_T, typename T>
key_T skip_list<key_T, T>::iterator::get_key() const {
    if (!current_node) throw std::out_of_range("Accessing key of end iterator");
    return current_node->key;
}

// Skip list constructor implementation
template <typename key_T, typename T>
skip_list<key_T, T>::skip_list(size_t in_max_level, double in_skip_prob)
    : max_level(in_max_level), skip_probability(in_skip_prob),
      current_max_level(1), element_count(0),
      gen(std::time(0)), dis(0.0, 1.0) {
    // Initialize head and tail sentinel nodes
    head = new Node(key_T(), T(), nullptr, max_level);
    tail = new Node(key_T(), T(), head, max_level);
    
    // Connect all head levels to tail
    for (size_t i = 0; i < max_level; ++i) {
        head->next[i] = tail;
    }
    tail->prev = head;
}

// Skip list destructor implementation
template <typename key_T, typename T>
skip_list<key_T, T>::~skip_list() {
    clear();    // Delete all nodes
    delete head; // Delete head sentinel
    delete tail; // Delete tail sentinel
}

// Random level generation implementation
template <typename key_T, typename T>
size_t skip_list<key_T, T>::random_level() {
    size_t level = 1;
    // Keep promoting with given probability until max level reached
    while (dis(gen) < skip_probability && level < max_level) {
        level++;
    }
    return std::min(level, max_level);
}

// Internal find node implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, T>::find_node(const key_T& key) const {
    Node* current = head;
    // Start at highest level and work down
    for (int i = current_max_level - 1; i >= 0; --i) {
        // Move right while next node's key is less than target
        while (current->next[i] != tail && current->next[i]->key < key) {
            current = current->next[i];
        }
    }
    // Move to actual node at base level
    current = current->next[0];
    // Return node if found, nullptr otherwise
    return (current != tail && current->key == key) ? current : nullptr;
}

// Get predecessors implementation (for insertion/deletion)
template <typename key_T, typename T>
std::vector<typename skip_list<key_T, T>::Node*> skip_list<key_T, T>::get_predecessors(const key_T& key) const {
    std::vector<Node*> predecessors(current_max_level, nullptr);
    Node* current = head;
    // Similar to find_node but records predecessors at each level
    for (int i = current_max_level - 1; i >= 0; --i) {
        while (current->next[i] != tail && current->next[i]->key < key) {
            current = current->next[i];
        }
        predecessors[i] = current;
    }
    return predecessors;
}

// Begin iterator implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::begin() {
    return iterator(this, head->next[0] != tail ? head->next[0] : nullptr);
}

// End iterator implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::end() {
    return iterator(this, nullptr);
}

// Insert implementation
template <typename key_T, typename T>
void skip_list<key_T, T>::insert(const key_T& key, const T& value) {
    // Get predecessors at all levels
    auto predecessors = get_predecessors(key);

    // Update value if key already exists
    if (predecessors[0]->next[0] != tail && predecessors[0]->next[0]->key == key) {
        predecessors[0]->next[0]->value = value;
        return;
    }

    // Determine new node's height
    size_t new_node_level = random_level();
    
    // Adjust predecessors if new node is taller than current max
    if (new_node_level > current_max_level) {
        predecessors.resize(new_node_level, head);
        current_max_level = new_node_level;
    }

    // Create new node
    Node* new_node = new Node(key, value, predecessors[0], new_node_level);

    // Update links at all levels
    for (size_t i = 0; i < new_node_level; ++i) {
        new_node->next[i] = predecessors[i]->next[i];
        predecessors[i]->next[i] = new_node;
    }

    // Update backward links
    new_node->prev = predecessors[0];
    if (new_node->next[0] != tail) {
        new_node->next[0]->prev = new_node;
    } else {
        tail->prev = new_node;
    }

    element_count++;
}

// Remove implementation
template <typename key_T, typename T>
void skip_list<key_T, T>::remove(const key_T& key) {
    // Get predecessors at all levels
    auto predecessors = get_predecessors(key);
    Node* to_remove = predecessors[0]->next[0];

    // Return if key not found
    if (to_remove == tail || to_remove->key != key) return;

    // Update forward links at all levels
    for (size_t i = 0; i < to_remove->next.size(); ++i) {
        if (i < predecessors.size()) {
            predecessors[i]->next[i] = to_remove->next[i];
        }
    }

    // Update backward links
    if (to_remove->next[0] != tail) {
        to_remove->next[0]->prev = to_remove->prev;
    } else {
        tail->prev = to_remove->prev;
    }

    // Delete node and update count
    delete to_remove;
    element_count--;

    // Adjust current max level if needed
    while (current_max_level > 1 && head->next[current_max_level - 1] == tail) {
        current_max_level--;
    }
}

// Find implementation
template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::find(const key_T& key) {
    Node* node = find_node(key);
    if (node && node != head && node != tail) {
        return iterator(this, node);
    }
    return end();
}

// Subscript operator implementation
template <typename key_T, typename T>
T& skip_list<key_T, T>::operator[](const key_T& key) {
    Node* node = find_node(key);
    if (!node) {
        // Insert default-constructed value if key not found
        insert(key, T());
        node = find_node(key);
    }
    return node->value;
}

// Size implementation
template <typename key_T, typename T>
size_t skip_list<key_T, T>::size() const {
    return element_count;
}

// Empty check implementation
template <typename key_T, typename T>
bool skip_list<key_T, T>::empty() const {
    return element_count == 0;
}

// Clear implementation
template <typename key_T, typename T>
void skip_list<key_T, T>::clear() {
    // Delete all nodes except sentinels
    Node* current = head->next[0];
    while (current != tail) {
        Node* temp = current;
        current = current->next[0];
        delete temp;
    }

    // Reset all head pointers to tail
    for (size_t i = 0; i < max_level; ++i) {
        head->next[i] = tail;
    }
    tail->prev = head;

    // Reset counters
    current_max_level = 1;
    element_count = 0;
}
