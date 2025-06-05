# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -g -Iinclude -Isrc
LDFLAGS = -lgtest -lgtest_main -lpthread -pthread

# Source files
TEST_SRC = tests/test.cpp 

# Object files
TEST_OBJ = $(TEST_SRC:.cpp=.o)

#Check
DEPS := $(wildcard include/*.hpp src/*.ipp)

# Target
TARGET = test_sl

.PHONY: all clean test format

all: $(TARGET)

$(TARGET): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST_OBJ)

format:
	astyle -A1 -s4 include/*.hpp tests/*.cpp
