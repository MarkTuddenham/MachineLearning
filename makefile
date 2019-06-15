# ~~~~~ COMMANDS ~~~~~
CXX := g++
CCFLAGS := -O3 -pthread -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -fprofile-arcs -ftest-coverage
LINKFLAGS :=  -lgcov --coverage

RM := rm

# ~~~~~ BUILD DIRS & FOLDERS ~~~~~
TMP_PATH := tmp

SRC_PATH := src
BUILD_PATH := build
INCLUDE_PATH := include

TEST_PATH := test

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
build: $(TARGET) 
build-lib:  $(OBJ) 

# Build .o first
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	@echo [CXX] $<
	@mkdir -p $(@D)
	@$(CXX) $(CCFLAGS) -o $@ -c $< -I ./$(INCLUDE_PATH)

# # precompute headers
# %.hpp.gch: %.hpp
# 	@echo [CXX] $<
# 	@$(CXX) $(CCFLAGS) -x c++-header -o $@ -c $< 

# Build final binary
$(TARGET): $(OBJ)
	@echo [INFO] Creating Binary: $(TARGET)
	@mkdir -p $(@D)
	@$(CXX) -o $@ $^ $(LINKFLAGS)


# ~~~~~ other ~~~~~
test: build-lib
	@$(MAKE) -C $(TEST_PATH) 

check:
	@echo [CHECK] Checking using cppcheck
	@cppcheck --enable=all --inconclusive --template=gcc $(SRC_PATH)/ $(TEST_PATH)/src/

clean-all: clean
	@$(MAKE) clean -C $(TEST_PATH) 

clean:
	@echo "[Cleaning]"
	@$(RM) -rfv $(BUILD_PATH)/*
	@$(RM) -rfv $(TMP_PATH)/*
	@$(RM) -rfv $(TARGET)
	@$(RM) -rfv $(INCLUDE_PATH)/*.gch
	@$(RM) -rfv *.gcov
