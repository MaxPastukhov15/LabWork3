#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include "skip_list.hpp"

using std::swap;

template <typename key_T, typename T> void skip_list::insert(const key_T& key, const T& value) {
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
    
template <typename key_T> void skip_list::remove(const key_T& key) {
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

    
void skip_list::clear() {
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












