# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g
LDFLAGS = -lgtest -lgtest_main -lpthread

SRC = test.cpp
HEADER = skip_list.hpp
OBJ = $(SRC:.cpp=.o)
# Targets
TARGET = skip_list_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

format:
	astyle -A1 -s4 *.cpp *.hpp

.PHONY: all test clean format

