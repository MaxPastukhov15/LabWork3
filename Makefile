# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -g -Iinclude
LDFLAGS = -lgtest -lgtest_main -lpthread -pthread

# Source files
SRC = src/skip_list.cpp
TEST_SRC = tests/test.cpp

# Object files
OBJ = $(SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Target
TARGET = test_sl

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJ) $(TEST_OBJ)
    $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
    ./$(TARGET)

clean:
    rm -f $(TARGET) $(OBJ) $(TEST_OBJ)

format:
    astyle -A1 -s4 include/*.hpp src/*.cpp tests/*.cppTARGET) $(OBJ) $(TEST_OBJ)

format:
	astyle -A1 -s4 *.cpp *.hpp

.PHONY: all test clean format

