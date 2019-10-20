LD_LIBRARY_PATH+=:$(shell pwd)

# ~~~~~ COMMANDS ~~~~~
CXX := $(CXX)

LIB_CCFLAGS := -std=c++17 -O3 -pthread -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -fPIC
LIB_LINKFLAGS := 

TEST_CCFLAGS := -std=c++17 -O0 -pthread -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -fPIC -fprofile-arcs -ftest-coverage -fsanitize=address
TEST_LINKFLAGS :=  -fprofile-arcs --coverage -fsanitize=address

RM := rm

# ~~~~~ BUILD DIRS & FOLDERS ~~~~~
BUILD_PATH := build
INCLUDE_PATH := include
SRC_PATH_TEST := src
SRC_PATH_LIB := teslyn

LIB_PATH := lib
TEST_PATH := test
EX_PATH := ex

LIB_SRC := $(wildcard $(LIB_PATH)/$(SRC_PATH_LIB)/*.cpp) $(wildcard $(LIB_PATH)/$(SRC_PATH_LIB)/**/*.cpp)
LIB_OBJ = $(patsubst $(LIB_PATH)/$(SRC_PATH_LIB)/%,$(LIB_PATH)/$(BUILD_PATH)/%,$(LIB_SRC:.cpp=.o))
# HEADER := $(wildcard $(SRC_PATH_LIB)/*.hpp)
# PRECOMPILED_HEADER := $(HEADER:.hpp=.hpp.gch)

LIB_SO := libteslyn.so
TEST_SO := libteslyn_test.so

# ~~~~~ BUILD RULES ~~~~~
.PHONY: build build-lib build-test \
clean clean-all clean-lib clean-test \
examples test

.DEFAULT_GOAL := build-lib

clean: clean-all
build: build-lib

# ~~~~~ main ~~~~~
build-lib: $(LIB_SO)

$(LIB_SO): $(LIB_OBJ)
	@echo "[INFO] Building $@"
	@$(CXX) -shared -Wl,-soname,$@ -o $@ $(LIB_LINKFLAGS) $^


$(LIB_PATH)/$(BUILD_PATH)/%.o: $(LIB_PATH)/$(SRC_PATH_LIB)/%.cpp
	@echo [CXX] $<
	@mkdir -p $(@D)
	@$(CXX) $(LIB_CCFLAGS) -o $@ -c $< -I $(LIB_PATH)/$(INCLUDE_PATH) -I $(LIB_PATH)/


# INSTALL_DIR := /usr/local/lib 
# install: build-lib
# ifneq ($(shell id -u), 0)
# 	@echo "You are not root, please run this target as root."
# else
# 	@echo [Installing $(LIB_SO)]
# 	@mkdir -p $(INSTALL_DIR)
# 	mv $(LIB_SO) $(INSTALL_DIR)
# endif


# ~~~~~ test ~~~~~
TEST_TARGET := run-tests

TEST_LIB_OBJ := $(patsubst $(LIB_PATH)/$(SRC_PATH_LIB)/%,$(TEST_PATH)/$(BUILD_PATH)/%,$(LIB_SRC:.cpp=_lib.o))

TEST_SRC := $(wildcard $(TEST_PATH)/$(SRC_PATH_TEST)/*.cpp) $(wildcard $(TEST_PATH)/$(SRC_PATH_TEST)/**/*.cpp)
TEST_OBJ = $(patsubst $(TEST_PATH)/$(SRC_PATH_TEST)/%,$(TEST_PATH)/$(BUILD_PATH)/%,$(TEST_SRC:.cpp=.o))

test: build-test $(TEST_TARGET)
	@echo "[INFO] Running Tests: $@"
	@./$(TEST_TARGET)

build-test: $(TEST_SO)

$(TEST_SO): $(TEST_LIB_OBJ)
	@echo "[INFO] Building $@"
	@$(CXX) -shared -Wl,-soname,$@ -o $@ $(TEST_LINKFLAGS) $^

$(TEST_PATH)/$(BUILD_PATH)/%_lib.o: $(LIB_PATH)/$(SRC_PATH_LIB)/%.cpp
	@echo [CXX] $<
	@mkdir -p $(@D)
	@$(CXX) $(TEST_CCFLAGS) -o $@ -c $< -I $(LIB_PATH)/$(INCLUDE_PATH) -I $(LIB_PATH)/

$(TEST_PATH)/$(BUILD_PATH)/%.o: $(TEST_PATH)/$(SRC_PATH_TEST)/%.cpp
	@echo [CXX] $<
	@mkdir -p $(@D)
	@$(CXX) $(LIB_CCFLAGS) -o $@ -c $< -I $(TEST_PATH)/$(INCLUDE_PATH) -I $(LIB_PATH)/

$(TEST_TARGET): $(TEST_OBJ)
	@echo [INFO] Creating Binary: $@
	@$(CXX)  $^ -o $@ $(TEST_LINKFLAGS) -L. -I $(LIB_PATH)/ -lteslyn_test

# ~~~~~ check ~~~~~
check:
	@echo [CHECK] Checking using cppcheck
	@cppcheck --enable=all --inconclusive --suppress=missingIncludeSystem  --template=gcc  $(LIB_PATH)/$(SRC_PATH_LIB)/ $(TEST_PATH)/$(SRC_PATH)

# ~~~~~ coveralls ~~~~~
coveralls:
	@echo [COV] Creating cov data
	@coveralls -b . \
	-e $(TEST_PATH)/$(INCLUDE_PATH)/ \
	-e $(TEST_PATH)/$(SRC_PATH)/ \
	-e $(LIB_PATH)/$(INCLUDE_PATH)/ \
	-e $(EX_PATH)/ \
	--gcov-options '\-lp'

# ~~~~~ examples ~~~~~
EX_SRC := $(wildcard $(EX_PATH)/*.cpp)
EX_RUNNABLES = $(EX_SRC:.cpp=)

examples: build-lib $(EX_RUNNABLES)

$(EX_PATH)/%: $(EX_PATH)/%.cpp
	@echo [CXX] $<
	@$(CXX) $< -o $@ -L. -I $(LIB_PATH) -lteslyn
	@mkdir -p $(EX_PATH)/bin/
	@mv $@ $(EX_PATH)/bin/

# ~~~~~ clean ~~~~~
clean-all: clean-lib clean-test
	@echo "[INFO] Cleaning .so files"
	@$(RM) -rfv *.so
	@echo "[INFO] Cleaning $(EX_PATH)/"
	@$(RM) -rfv $(EX_PATH)/bin/

clean-lib:
	@echo "[INFO] Cleaning $(LIB_PATH)/"
	@$(RM) -rfv $(LIB_PATH)/$(BUILD_PATH)/*

clean-test:
	@echo "[INFO] Cleaning $(TEST_PATH)/"
	@$(RM) -rfv $(TEST_PATH)/$(BUILD_PATH)/*
	@$(RM) -fv $(TEST_TARGET)


