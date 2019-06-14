# ~~~~~ COMMANDS ~~~~~
CC := g++
CCFLAGS := -O2 -pthread -Wall -Wextra -Wpedantic
LINKFLAGS := 

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
build:  $(TARGET) $(BUILD_PATH)/

# Build .o first
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	@echo [CC] $<
	@$(CC) $(CCFLAGS) -o $@ -c $< -I $(INCLUDE_PATH) -I ../$(LIB_SRC_PATH)

# Build final binary
$(TARGET): $(OBJ)
	@echo [INFO] Creating Binary: $(TARGET)
	@$(CC) -o $@ $^ $(LINKFLAGS) -I ../$(LIB_BUILD_PATH)/*


# ~~~~~ other ~~~~~
clean:
	@echo "[Cleaning test]"
	@$(RM) -rfv $(BUILD_PATH)/*
	@$(RM) -rfv $(TMP_PATH)/*
	@$(RM) -rfv $(TARGET)
	@$(RM) -rfv $(INCLUDE_PATH)/*.gch

$(BUILD_PATH):
	@if [ ! -d $(BUILD_PATH)/ ]; then mkdir $(BUILD_PATH)/; fi