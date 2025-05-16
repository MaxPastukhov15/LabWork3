#pragma once 

#include <vector>

template <typename key_T, typename T>
class skip_list{
private:
   size_t max_level;
   double skip_divisor;
   struct Node{
   	key_T key;
   	T value;
   	Node* prev;
   	std::vector<Node*> next;
   	Node(const key_T& in_key, const T& in_value, Node* in_prev, size_t in_level);
   	~Node();
   	Node& operator= (const Node& in_node);
   	Node* previous_on_level(size_t in_level);
   	Node* next_on_level(size_t in_level);
   };
   Node start;
   Node* new_previous(const key_T& in_key);
   Node* node_by_key(const key_T& in_key);
   size_t new_level();
public:
	class iterator{}; 

};
