# ~~~~~ COMMANDS ~~~~~
CC := g++
CCFLAGS := -O3 -pthread -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor
LINKFLAGS := 

RM := rm

# ~~~~~ BUILD DIRS & FOLDERS ~~~~~
TMP_PATH := tmp

SRC_PATH := src
BUILD_PATH := build
INCLUDE_PATH := include

TARGET := ml
# MAIN := main.cpp
# MAIN_OBJ :=$(MAIN:.cpp=.o)

SRC := $(wildcard $(SRC_PATH)/*.cpp) $(wildcard $(SRC_PATH)/**/*.cpp)
OBJ = $(patsubst $(SRC_PATH)/%,$(BUILD_PATH)/%,$(SRC:.cpp=.o))
# HEADER := $(wildcard $(SRC_PATH)/*.hpp)
# PRECOMPILED_HEADER := $(HEADER:.hpp=.hpp.gch)

# ~~~~~ BUILD RULES ~~~~~
.PHONY: build build-lib clean clean-all run test
.DEFAULT_GOAL := build

run: build
	@echo [RUNNING] $(TARGET)
	@./$(TARGET)

# ~~~~~ main ~~~~~
build:  $(TARGET) $(BUILD_PATH)/
build-lib: $(OBJ) $(BUILD_PATH)/

# Build .o first
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	@echo [CC] $<
	@$(CC) $(CCFLAGS) -o $@ -c $< -I ./$(INCLUDE_PATH)

# # precompute headers
# %.hpp.gch: %.hpp
# 	@echo [CC] $<
# 	@$(CC) $(CCFLAGS) -x c++-header -o $@ -c $< 

# Build final binary
$(TARGET): $(OBJ)
	@echo [INFO] Creating Binary: $(TARGET)
	@$(CC) -o $@ $^ $(LINKFLAGS)


# ~~~~~ other ~~~~~
test: build-lib
	@$(MAKE) -C test

clean-all: clean
	@$(MAKE) clean -C test 

clean:
	@echo "[Cleaning]"
	@$(RM) -rfv $(BUILD_PATH)/*
	@$(RM) -rfv $(TMP_PATH)/*
	@$(RM) -rfv $(TARGET)
	@$(RM) -rfv $(INCLUDE_PATH)/*.gch


$(BUILD_PATH):
	@if [ ! -d $(BUILD_PATH)/ ]; then mkdir $(BUILD_PATH)/; fi