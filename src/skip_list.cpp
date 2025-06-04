#include "skip_list.hpp"

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>

template <typename key_T, typename T>
skip_list<key_T, T>::Node::Node(const key_T& in_key, const T& in_value, Node* in_prev, size_t in_level)
    : key(in_key), value(in_value), prev(in_prev), next(in_level, nullptr) {}

template <typename key_T, typename T>
skip_list<key_T, T>::Node::~Node() {}

template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, T>::Node::previous_on_level(size_t in_level) {
    if (in_level >= next.size()) return nullptr;
    Node* current = prev;
    while (current && current->next.size() <= in_level) {
        current = current->prev;
    }
    return current;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, T>::Node::next_on_level(size_t in_level) {
    if (in_level >= next.size()) return nullptr;
    return next[in_level];
}

template <typename key_T, typename T>
skip_list<key_T, T>::iterator::iterator(skip_list* list, Node* node)
    : current_list(list), current_node(node) {}

template <typename key_T, typename T>
skip_list<key_T, T>::iterator::iterator(const iterator& other)
    : current_list(other.current_list), current_node(other.current_node) {}

template <typename key_T, typename T>
skip_list<key_T, T>::iterator::operator bool() const {
    return current_node != nullptr && current_node != current_list->head && current_node != current_list->tail;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator++() {
    if (current_node) {
        current_node = current_node->next[0];
        if (current_node && current_node == current_list->tail) {
            current_node = nullptr;
        }
    }
    return *this;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator--() {
    if (current_node) {
        current_node = current_node->prev;
        if (current_node && current_node == current_list->head) {
            current_node = nullptr;
        }
    } else {
        current_node = current_list->tail->prev;
        if (current_node == current_list->head) {
            current_node = nullptr;
        }
    }
    return *this;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::iterator::operator--(int) {
    iterator temp = *this;
    --(*this);
    return temp;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator+=(size_t n) {
    while (n-- > 0 && current_node) {
        current_node = current_node->next[0];
    }
    return *this;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator-=(size_t n) {
    while (n-- > 0 && current_node) {
        current_node = current_node->prev;
    }
    return *this;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator& skip_list<key_T, T>::iterator::operator=(const iterator& other) {
    current_list = other.current_list;
    current_node = other.current_node;
    return *this;
}

template <typename key_T, typename T>
T& skip_list<key_T, T>::iterator::operator*() {
    if (!current_node) throw std::out_of_range("Dereferencing end iterator");
    return current_node->value;
}

template <typename key_T, typename T>
T* skip_list<key_T, T>::iterator::operator->() {
    if (!current_node) throw std::out_of_range("Dereferencing end iterator");
    return &(current_node->value);
}

template <typename key_T, typename T>
bool skip_list<key_T, T>::iterator::operator==(const iterator& other) const {
    return current_node == other.current_node;
}

template <typename key_T, typename T>
bool skip_list<key_T, T>::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

template <typename key_T, typename T>
void skip_list<key_T, T>::iterator::remove() {
    if (current_node) {
        current_list->remove(current_node->key);
    }
}

template <typename key_T, typename T>
key_T skip_list<key_T, T>::iterator::get_key() const {
    if (!current_node) throw std::out_of_range("Accessing key of end iterator");
    return current_node->key;
}

template <typename key_T, typename T>
skip_list<key_T, T>::skip_list(size_t in_max_level, double in_skip_prob)
    : max_level(in_max_level), skip_probability(in_skip_prob),
      current_max_level(1), element_count(0),
      gen(std::time(0)), dis(0.0, 1.0) {
    head = new Node(key_T(), T(), nullptr, max_level);
    tail = new Node(key_T(), T(), head, max_level);
    for (size_t i = 0; i < max_level; ++i) {
        head->next[i] = tail;
    }
    tail->prev = head;
}

template <typename key_T, typename T>
skip_list<key_T, T>::~skip_list() {
    clear();
    delete head;
    delete tail;
}

template <typename key_T, typename T>
size_t skip_list<key_T, T>::random_level() {
    size_t level = 1;
    while (dis(gen) < skip_probability && level < max_level) {
        level++;
    }
    return std::min(level, max_level);
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, T>::find_node(const key_T& key) const {
    Node* current = head;
    for (int i = current_max_level - 1; i >= 0; --i) {
        while (current->next[i] != tail && current->next[i]->key < key) {
            current = current->next[i];
        }
    }
    current = current->next[0];
    return (current != tail && current->key == key) ? current : nullptr;
}

template <typename key_T, typename T>
std::vector<typename skip_list<key_T, T>::Node*> skip_list<key_T, T>::get_predecessors(const key_T& key) const {
    std::vector<Node*> predecessors(current_max_level, nullptr);
    Node* current = head;
    for (int i = current_max_level - 1; i >= 0; --i) {
        while (current->next[i] != tail && current->next[i]->key < key) {
            current = current->next[i];
        }
        predecessors[i] = current;
    }
    return predecessors;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::begin() {
    return iterator(this, head->next[0] != tail ? head->next[0] : nullptr);
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::end() {
    return iterator(this, nullptr);
}

template <typename key_T, typename T>
void skip_list<key_T, T>::insert(const key_T& key, const T& value) {
    auto predecessors = get_predecessors(key);

    if (predecessors[0]->next[0] != tail && predecessors[0]->next[0]->key == key) {
        predecessors[0]->next[0]->value = value;
        return;
    }

    size_t new_node_level = random_level();
    if (new_node_level > current_max_level) {
        predecessors.resize(new_node_level, head);
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

template <typename key_T, typename T>
void skip_list<key_T, T>::remove(const key_T& key) {
    auto predecessors = get_predecessors(key);
    Node* to_remove = predecessors[0]->next[0];

    if (to_remove == tail || to_remove->key != key) return;

    for (size_t i = 0; i < to_remove->next.size(); ++i) {
        if (i < predecessors.size()) {
            predecessors[i]->next[i] = to_remove->next[i];
        }
    }

    if (to_remove->next[0] != tail) {
        to_remove->next[0]->prev = to_remove->prev;
    } else {
        tail->prev = to_remove->prev;
    }

    delete to_remove;
    element_count--;

    while (current_max_level > 1 && head->next[current_max_level - 1] == tail) {
        current_max_level--;
    }
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::iterator skip_list<key_T, T>::find(const key_T& key) {
    Node* node = find_node(key);
    if (node && node != head && node != tail) {
        return iterator(this, node);
    }
    return end();
}

template <typename key_T, typename T>
T& skip_list<key_T, T>::operator[](const key_T& key) {
    Node* node = find_node(key);
    if (!node) {
        insert(key, T());
        node = find_node(key);
    }
    return node->value;
}

template <typename key_T, typename T>
size_t skip_list<key_T, T>::size() const {
    return element_count;
}

template <typename key_T, typename T>
bool skip_list<key_T, T>::empty() const {
    return element_count == 0;
}

template <typename key_T, typename T>
void skip_list<key_T, T>::clear() {
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
