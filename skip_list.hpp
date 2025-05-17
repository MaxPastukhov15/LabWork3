#pragma once 

#include <algorithm>
#include <vector>

using std::swap;

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
	class iterator{
	  private:
	  	skip_list* current_list;
	  	Node* current_node;
	  	friend class skip_list<key_T, T>;
	  public:
	  	iterator(const iterator& in_iterator);
	  	iterator(skip_list& in_skip_list);
	  	operator bool();
	  	iterator& operator++();
	  	iterator& operator--();
	  	iterator& operator++(int);
	  	iterator& operator--(int);
	  	iterator& operator+=(size_t n);
	  	iterator& operator-=(size_t n);
	  	iterator& operator=(const iterator& in_iterator);
	  	iterator& operator=(const key_T& in_key);
	  	T& operator*();
	  	void remove();
	}; 
	
	skip_list(size_t in_num_of_lanes = 3, double in_skip_div= 1.0/4.0);
	~skip_list();
	iterator get_begin();
	iterator get_end();
	void push(const key_T& in_key, const T& in_data);
	void remove(const key_T& in_key);
	T& operator[](const key_T& in_key);
	size_t count();
	void clear();
	iterator find(const T& unknown);
	bool is_empty();
};

template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, 
T>::Node::previous_on_level(size_t in_level){
	if (!this) return NULL;
	Node* current = prev;
	for (; current && !(current->next.count() >= in_level+1));
	current = current->next[in_level - 1] {}
	return current;
}

template <typename key_T, typename T>
typename skip_list<key_T, T>::Node* skip_list<key_T, 
T>::Node::next_on_level(size_t in_level){
	if (!this) return NULL;
	Node* current = next[in_level - 1];
	for(; current && !(current-> next.count() >= in_level + 1));
	current = current->next[in_level - 1] {}
	return current;
}














