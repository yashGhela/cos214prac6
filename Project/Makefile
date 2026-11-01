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



TEST_DIR   = tests
TEST_BUILD = $(BUILD_DIR)/tests
TEST_BIN   = runTests
TEST_SRCS  = $(wildcard $(TEST_DIR)/*.cpp)
APP_SRCS   = $(filter-out $(SRC_DIR)/Main.cpp,$(SOURCES))
TEST_OBJS  = $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(TEST_BUILD)/%.o) \
             $(APP_SRCS:$(SRC_DIR)/%.cpp=$(TEST_BUILD)/app_%.o)

$(TEST_BUILD)/%.o: $(TEST_DIR)/%.cpp
	mkdir -p $(TEST_BUILD)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(TEST_DIR) -MMD -MP -c $< -o $@

$(TEST_BUILD)/app_%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(TEST_BUILD)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -MMD -MP -c $< -o $@

$(TEST_BIN): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

test-asan:
	$(MAKE) test CXXFLAGS="$(CXXFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer" \
	             TEST_BUILD=$(BUILD_DIR)/tests_asan TEST_BIN=runTests_asan

-include $(TEST_OBJS:.o=.d)

docs:
	doxygen Doxyfile