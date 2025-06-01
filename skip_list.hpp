#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>

template <typename key_T, typename T>
class skip_list {
private:
    struct Node {
        key_T key;
        T value;
        Node* prev;
        std::vector<Node*> next;

        Node(const key_T& in_key, const T& in_value, Node* in_prev, size_t in_level);
        ~Node();

        Node* previous_on_level(size_t in_level);
        Node* next_on_level(size_t in_level);
    };

    Node* head;
    Node* tail;
    size_t max_level;
    double skip_probability;
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
        iterator(skip_list* list, Node* node);
        iterator(const iterator& other);

        operator bool() const;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        iterator& operator+=(size_t n);
        iterator& operator-=(size_t n);
        iterator& operator=(const iterator& other);
        T& operator*();
        T* operator->();
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
        void remove();
        key_T get_key() const;
    };

    skip_list(size_t in_max_level = 16, double in_skip_prob = 0.5);
    ~skip_list();

    iterator begin();
    iterator end();

    void insert(const key_T& key, const T& value);
    void remove(const key_T& key);
    iterator find(const key_T& key);
    T& operator[](const key_T& key);

    size_t size() const;
    bool empty() const;
    void clear();
};

#include "skip_list.cpp"
