# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -Werror -Wpedantic 
LDFLAGS = -lgtest -lgtest_main -lpthread

# Targets
TARGET = skip_list
TEST_TARGET = sk_list_test

# Source files
MAIN_SRCS = skip_list.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)

TEST_SRCS = test.cpp skip_list.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(MAIN_OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

format:
	astyle -A1 -s4 *.cpp *.hpp

.PHONY: all run test clean format
