#include "skip_list.hpp"

template <typename key_T, typename T>
skip_list<key_T, T>::skip_list(size_t in_max_level, double in_skip_prob) 
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

template <typename key_T, typename T>
skip_list<key_T, T>::~skip_list() {
    clear();
    delete head;
    delete tail;
}

template <typename key_T, typename T>
size_t skip_list<key_T, T>::random_level() {
    size_t level = 1;
    while (dis(gen) < skip_probability && level < max_level) level++;
    return level;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, T>::find_node(const key_T& key) const {
    Node* current = head;
    for (int i = current_max_level - 1; i >= 0; --i) {
        while (current->next[i] && current->next[i]->key < key) current = current->next[i];
    }
    current = current->next[0];
    return (current && current->key == key) ? current : nullptr;
}

template <typename key_T, typename T>
std::vector<typename skip_list<key_T, T>::Node*> skip_list<key_T, T>::get_predecessors(const key_T& key) const {
    std::vector<Node*> predecessors(current_max_level, nullptr);
    Node* current = head;
    for (int i = current_max_level - 1; i >= 0; --i) {
        while (current->next[i] && current->next[i]->key < key) current = current->next[i];
        predecessors[i] = current;
    }
    return predecessors;
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

template <typename key_T, typename T>
void skip_list<key_T, T>::remove(const key_T& key) {
    auto predecessors = get_predecessors(key);
    Node* to_remove = predecessors[0]->next[0];
    
    if (to_remove == tail || to_remove->key != key) {
        return;
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
    
    while (current_max_level > 1 && head->next[current_max_level - 1] == tail) {
        current_max_level--;
    }
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
