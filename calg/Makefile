CC = gcc
CFLAGS = -g -Wall -Werror -lm -Wpedantic -O0

SRC_DIR = src
BUILD_DIR = build

SEM_DIRS := $(wildcard $(SRC_DIR)/sem_*)
SOURCES := $(foreach dir,$(SEM_DIRS),$(wildcard $(dir)/*.c))
TARGETS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))

all: $(TARGETS)

$(BUILD_DIR)/%: $(SRC_DIR)/%.c | $(dir $(BUILD_DIR)/%)
	$(CC) $< -o $@ $(CFLAGS) 

$(BUILD_DIR)/%/:
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
