/**
 * @file skip_list.hpp
 * @author Maksim Pastukhov (st131119@student.spbu.ru)
 * @brief A template implementation of a skip list data structure
 *
 * This file contains a complete implementation of a skip list, which is a probabilistic
 * data structure that allows for average-case O(log n) search, insertion, and deletion
 * operations. The skip list maintains multiple levels of linked lists, with higher levels
 * skipping over many elements to provide faster search paths.
 */

#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>

/**
 * @brief A skip list container that provides fast search, insertion, and deletion
 *
 * @tparam key_T The type of keys used for ordering elements
 * @tparam T The type of elements stored in the skip list
 */

template <typename key_T, typename T>
class skip_list {
private:
    /**
     * @brief Node structure representing elements in the skip list
     */
    struct Node {
        key_T key;               ///< The key used for ordering
        T value;                 ///< The stored value
        Node* prev;              ///< Pointer to previous node
        std::vector<Node*> next; ///< Vector of forward pointers (one for each level)

        /**
         * @brief Constructs a new Node
         * @param in_key The key for the new node
         * @param in_value The value for the new node
         * @param in_prev Pointer to the previous node
         * @param in_level The number of levels for this node
         */
        Node(const key_T& in_key, const T& in_value, Node* in_prev, size_t in_level);
        
        /// Destructor
        ~Node();

        /**
         * @brief Returns the previous node at a specific level
         * @param in_level The level to check
         * @return Pointer to the previous node at the specified level
         */
        Node* previous_on_level(size_t in_level);
        
        /**
         * @brief Returns the next node at a specific level
         * @param in_level The level to check
         * @return Pointer to the next node at the specified level
         */
        Node* next_on_level(size_t in_level);
    };

    Node* head;                ///< Pointer to head sentinel node
    Node* tail;                ///< Pointer to tail sentinel node
    size_t max_level;          ///< Maximum allowed levels in the skip list
    double skip_probability;   ///< Probability for promoting a node to higher levels
    size_t current_max_level;  ///< Current highest level in use
    size_t element_count;      ///< Number of elements in the skip list

    std::mt19937 gen;          ///< Random number generator
    std::uniform_real_distribution<> dis; ///< Uniform distribution for level generation

    /**
     * @brief Generates a random level for new nodes
     * @return A random level between 1 and max_level
     */
    size_t random_level();
    
    /**
     * @brief Finds a node with the given key (internal implementation)
     * @param key The key to search for
     * @return Pointer to the node if found, nullptr otherwise
     */
    Node* find_node(const key_T& key) const;
    
    /**
     * @brief Gets all predecessor nodes for insertion/deletion
     * @param key The key to find predecessors for
     * @return Vector of pointers to predecessor nodes at each level
     */
    std::vector<Node*> get_predecessors(const key_T& key) const;

public:
    class iterator {
    private:
        skip_list* current_list;  ///< Pointer to the containing skip list
        Node* current_node;       ///< Pointer to current node

    public:
        /**
         * @brief Constructs a new iterator
         * @param list Pointer to the skip list
         * @param node Pointer to the current node
         */
        iterator(skip_list* list, Node* node);
        
        /// Copy constructor
        iterator(const iterator& other);

        /**
         * @brief Conversion to bool (true if iterator points to valid element)
         * @return true if iterator points to a valid element, false otherwise
         */
        operator bool() const;
        
        /// Prefix increment
        iterator& operator++();
        
        /// Postfix increment
        iterator operator++(int);
        
        /// Prefix decrement
        iterator& operator--();
        
        /// Postfix decrement
        iterator operator--(int);
        
        /**
         * @brief Compound addition
         * @param n Number of positions to advance
         * @return Reference to the updated iterator
         */
        iterator& operator+=(size_t n);
        
        /**
         * @brief Compound subtraction
         * @param n Number of positions to retreat
         * @return Reference to the updated iterator
         */
        iterator& operator-=(size_t n);
        
        /// Assignment operator
        iterator& operator=(const iterator& other);
        
        /**
         * @brief Dereference operator
         * @return Reference to the current element's value
         * @throws std::out_of_range if iterator is invalid
         */
        T& operator*();
        
        /**
         * @brief Member access operator
         * @return Pointer to the current element's value
         * @throws std::out_of_range if iterator is invalid
         */
        T* operator->();
        
        /**
         * @brief Equality comparison
         * @param other Iterator to compare with
         * @return true if iterators point to same node, false otherwise
         */
        bool operator==(const iterator& other) const;
        
        /**
         * @brief Inequality comparison
         * @param other Iterator to compare with
         * @return true if iterators point to different nodes, false otherwise
         */
        bool operator!=(const iterator& other) const;
        
        /**
         * @brief Remove current element from the skip list
         */
        void remove();
        
        /**
         * @brief Get current element's key
         * @return The key of the current element
         * @throws std::out_of_range if iterator is invalid
         */
        key_T get_key() const;
    };

    /**
     * @brief Constructs a new skip list
     * @param in_max_level Maximum number of levels (default: 16)
     * @param in_skip_prob Probability for promoting nodes to higher levels (default: 0.5)
     */
    skip_list(size_t in_max_level = 16, double in_skip_prob = 0.5);
    
    /// Destructor
    ~skip_list();

    /**
     * @brief Returns iterator to first element
     * @return Iterator pointing to the first element
     */
    iterator begin();
    
    /**
     * @brief Returns iterator representing end of list
     * @return Iterator representing the end of the list
     */
    iterator end();

    /**
     * @brief Inserts a new element or updates existing one
     * @param key The key of the element to insert/update
     * @param value The value to associate with the key
     */
    void insert(const key_T& key, const T& value);
    
    /**
     * @brief Removes element with specified key
     * @param key The key of the element to remove
     */
    void remove(const key_T& key);
    
    /**
     * @brief Finds element with specific key
     * @param key The key to search for
     * @return Iterator pointing to the found element, or end() if not found
     */
    iterator find(const key_T& key);
    
    /**
     * @brief Accesses element with bounds checking (inserts if not found)
     * @param key The key of the element to access
     * @return Reference to the value associated with the key
     */
    T& operator[](const key_T& key);

    /**
     * @brief Returns number of elements
     * @return The number of elements in the skip list
     */
    size_t size() const;
    
    /**
     * @brief Checks if list is empty
     * @return true if the skip list is empty, false otherwise
     */
    bool empty() const;
    
    /**
     * @brief Removes all elements
     */
    void clear();
};


#include "skip_list.ipp"
