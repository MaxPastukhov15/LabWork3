# LabWork 1
## Author
Pastukhov Maksim Germanovich, группа 24.Б82-мм
## Contacts
st131119@student.spbu.ru

# Skip List Implementation

A template-based skip list implementation with STL-like interface.

## Methods

### Constructors
- `skip_list(size_t max_level = 16, double skip_prob = 0.5)`  
  Creates a new skip list with specified maximum level and skip probability

### Modifiers
- `void insert(const key_T& key, const T& value)`  
  Inserts a new element or updates existing one
- `void remove(const key_T& key)`  
  Removes element with specified key
- `void clear()`  
  Removes all elements

### Accessors
- `T& operator[](const key_T& key)`  
  Access element by key (creates if not exists)
- `iterator find(const key_T& key)`  
  Finds element by key
- `size_t size() const`  
  Returns number of elements
- `bool empty() const`  
  Checks if container is empty

### Iterators
- `iterator begin()`  
  Returns iterator to first element
- `iterator end()`  
  Returns iterator to end
- Iterator supports: `++`, `--`, `+=`, `-=`, `*`, `->`, `==`, `!=`


}
