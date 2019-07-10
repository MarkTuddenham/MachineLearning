# ~~~~~ COMMANDS ~~~~~
CXX := g++
CCFLAGS := -pthread -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -fprofile-arcs -ftest-coverage #-fsanitize=address
LINKFLAGS := -lgcov --coverage

RM := rm

# ~~~~~ BUILD DIRS & FOLDERS ~~~~~
TMP_PATH := tmp

SRC_PATH := src
BUILD_PATH := build
INCLUDE_PATH := include

LIB_SRC_PATH ?= src
LIB_BUILD_PATH ?= build

TARGET := run-tests

SRC := $(wildcard $(SRC_PATH)/*.cpp) $(wildcard $(SRC_PATH)/**/*.cpp)
OBJ = $(patsubst $(SRC_PATH)/%,$(BUILD_PATH)/%,$(SRC:.cpp=.o))

# ~~~~~ BUILD RULES ~~~~~
.PHONY: build clean
.DEFAULT_GOAL := build

# ~~~~~ main ~~~~~
build: $(TARGET) 

# Build .o first
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	@echo [CXX] $<
	@mkdir -p $(@D)
	@$(CXX) $(CCFLAGS) -o $@ -c $< -I $(INCLUDE_PATH) -I ../$(LIB_SRC_PATH)

# Build final binary
$(TARGET): $(OBJ)
	@echo [INFO] Creating Binary: $(TARGET)
	@mkdir -p $(@D)
	@$(CXX) -o $@ $^ $(LINKFLAGS) ../$(LIB_BUILD_PATH)/* -I ../$(LIB_SRC_PATH)/


# ~~~~~ other ~~~~~
clean:
	@echo "[Cleaning test]"
	@$(RM) -rfv $(BUILD_PATH)/*
	@$(RM) -rfv $(TMP_PATH)/*
	@$(RM) -rfv $(TARGET)
	@$(RM) -rfv $(INCLUDE_PATH)/*.gch

