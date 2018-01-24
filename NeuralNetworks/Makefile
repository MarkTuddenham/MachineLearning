.PHONY: build run clean
.DEFAULT_GOAL: build

cc = g++
cc_flags = -g -std=c++11
bin_folder := bin
src_folder := src

out := $(bin_folder)/main


# build
build: $(out)

run: build
	./$(out)

# clean
clean:
	# rm -rf tmp/
	ls -A | grep -vE "Makefile|$(source_folder)|$(bin_folder)" | xargs -0 rm

# output file
$(out): $(src_folder)/*
		$(cc) $(cc_flags) $(src_folder)/*.cpp -o $(out)
