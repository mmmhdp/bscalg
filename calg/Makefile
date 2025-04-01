CC = gcc
CFLAGS = -g -Wall -Wextra -lm -Wpedantic -O0 -fsanitize=address -fsanitize=leak -static-libasan 
SRC_DIR = src
BUILD_DIR = build

SEM_DIRS := $(wildcard $(SRC_DIR)/sem_*) $(wildcard $(SRC_DIR)/shad_*)
SOURCES := $(foreach dir, $(SEM_DIRS), $(wildcard $(dir)/*.c))
TARGETS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))

all: $(TARGETS) format run

$(BUILD_DIR)/%: $(SRC_DIR)/%.c | $(dir $(BUILD_DIR)/%)
	$(CC) $< -o $@ $(CFLAGS) 

$(BUILD_DIR)/%/:
	mkdir -p $@

run:
	./build/sem_4/problem_rg < in.dat

format:
	find $(SRC_DIR) -name "*.c" -exec clang-format -i {} +

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean format run
