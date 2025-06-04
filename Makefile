# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g -Iinclude
LDFLAGS = -lgtest -lgtest_main -lpthread -pthread

# Source files
TEST_SRC = tests/test.cpp

# Object files
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Target
TARGET = test_sl

.PHONY: all clean test format

all: $(TARGET)

$(TARGET): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST_OBJ)

format:
	astyle -A1 -s4 include/*.hpp tests/*.cpp
