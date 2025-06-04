# Skip List Test Suite

## Overview
This test suite verifies the functionality of the `skip_list` template class implementation using the Google Test framework. The tests cover basic operations, edge cases, and iterator functionality.

## Test Categories

### 1. Basic Functionality
- **ConstructorAndEmpty**: Verifies that a newly constructed skip list is empty
- **InsertAndSize**: Tests insertion and size tracking
- **Clear**: Tests the clear operation

### 2. Element Access
- **FindExistingElement**: Tests finding existing elements
- **FindNonExistingElement**: Tests behavior when searching for non-existent elements
- **OperatorBrackets**: Tests element access using operator[]

### 3. Removal Operations
- **RemoveExistingElement**: Tests removal of existing elements
- **RemoveNonExistingElement**: Tests behavior when removing non-existent elements

### 4. Iterator Functionality
- **IteratorBasic**: Tests basic iterator operations
- **IteratorRangeBasedFor**: Tests compatibility with range-based for loops
- **IteratorPostIncrement**: Tests post-increment behavior
- **IteratorDecrement**: Tests decrement operations
- **IteratorRemove**: Tests element removal via iterators

### 5. Performance and Scaling
- **LargeInsertAndFind**: Tests with 1000 elements to verify scaling
- **StringKeys**: Tests with string keys to verify template flexibility

## Test Details

### ConstructorAndEmpty
```cpp
TEST(SkipListTest, ConstructorAndEmpty) {
    skip_list<int, std::string> sl;
    EXPECT_TRUE(sl.empty());
    EXPECT_EQ(sl.size(), 0);
}
