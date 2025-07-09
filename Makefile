CXX := g++
CXXFLAGS := -O3 -Wall -Wextra -std=c++20 -Iinclude

LDFLAGS := -lglfw -lGLEW -lGL -lglm

SRC_DIR := src
OBJ_DIR := build
BIN := main

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
