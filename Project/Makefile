CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

SRC_DIR =src
INC_DIR =include
BUILD_DIR =build

TARGET =TIVIDY 

SOURCES= $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS= $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*

debug: $(TARGET)
	gdb ./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

.PHONY: all run clean debug valgrind