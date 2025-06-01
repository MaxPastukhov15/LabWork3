# LabWork 3

## Author
Pastukhov Maksim Germanovich, группа 24.Б82-мм

## Contacts
st131119@student.spbu.ru

# Skip List Implementation

A template-based skip list implementation with STL-like interface.

## Methods

### Constructors
- `skip_list(size_t max_level = 16, double skip_prob = 0.5)`
  Creates a new skip list with specified maximum level and skip probability.

  **Example:**
  ```cpp
  skip_list<int, std::string> skipList; // Creates a skip list with default parameters
  skip_list<int, std::string> customSkipList(20, 0.6); // Creates a skip list with max_level 20 and skip_probability 0.6
  ```

### Modifiers
- `void insert(const key_T& key, const T& value)`
  Inserts a new element or updates an existing one.

  **Example:**
  ```cpp
  skipList.insert(1, "one");
  skipList.insert(2, "two");
  ```

- `void remove(const key_T& key)`
  Removes an element with the specified key.

  **Example:**
  ```cpp
  skipList.remove(1); // Removes the element with key 1
  ```

- `void clear()`
  Removes all elements.

  **Example:**
  ```cpp
  skipList.clear(); // Clears the entire skip list
  ```

### Accessors
- `T& operator[](const key_T& key)`
  Accesses an element by key (creates it if it doesn't exist).

  **Example:**
  ```cpp
  std::string value = skipList[1]; // Accesses the element with key 1
  skipList[3] = "three"; // Inserts or updates the element with key 3
  ```

- `iterator find(const key_T& key)`
  Finds an element by key.

  **Example:**
  ```cpp
  auto it = skipList.find(1);
  if (it) {
      std::cout << "Found: " << *it << std::endl;
  }
  ```

- `size_t size() const`
  Returns the number of elements.

  **Example:**
  ```cpp
  std::cout << "Size: " << skipList.size() << std::endl;
  ```

- `bool empty() const`
  Checks if the container is empty.

  **Example:**
  ```cpp
  if (skipList.empty()) {
      std::cout << "The skip list is empty." << std::endl;
  }
  ```

### Iterators
- `iterator begin()`
  Returns an iterator to the first element.

  **Example:**
  ```cpp
  auto it = skipList.begin();
  ```

- `iterator end()`
  Returns an iterator to the end.

  **Example:**
  ```cpp
  auto endIt = skipList.end();
  ```

- **Iterator Usage Example:**
  ```cpp
  for (auto it = skipList.begin(); it != skipList.end(); ++it) {
      std::cout << "Key: " << it.get_key() << ", Value: " << *it << std::endl;
  }
  ```

- **Iterator supports:** `++`, `--`, `+=`, `-=`, `*`, `->`, `==`, `!=`

  **Example:**
  ```cpp
  auto it = skipList.begin();
  ++it; // Move to the next element
  --it; // Move to the previous element
  std::cout << "Value: " << *it << std::endl; // Dereference the iterator
  ```

