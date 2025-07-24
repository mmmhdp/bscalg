CC = gcc
WARNING_AND_ERRORS = -Wall -Wextra -Wpedantic 
OPTIMIZATION =
LIBS = -lc -lm
XSANITIZE = -fsanitize=address -fsanitize=leak -static-libasan
DBUG_INF = -g 
CFLAGS = $(DBUG_INF) $(WARNING_AND_ERRORS) $(LIBS) $(XSANITIZE) $(OPTIMIZATION)
ASM_CFLAGS = $(WARNING_AND_ERRORS) $(LIBS) 

SRC_DIR = src
BUILD_DIR = build
ASM_DIR = $(BUILD_DIR)/asm

SEM_DIRS := $(wildcard $(SRC_DIR)/sem_*) $(wildcard $(SRC_DIR)/sandbox0)
SOURCES := $(foreach dir, $(SEM_DIRS), $(wildcard $(dir)/*.c))
TARGETS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))
ASM_FILES := $(patsubst $(SRC_DIR)/%.c, $(ASM_DIR)/%.s, $(SOURCES))

all: $(TARGETS) run

$(BUILD_DIR)/%: $(SRC_DIR)/%.c | $(dir $(BUILD_DIR)/%)
	$(CC) $< -o $@ $(CFLAGS)

$(BUILD_DIR)/%/:
	mkdir -p $@

run:
	#./build/sem_5/problem_ed < in.dat
	./build/sandbox0/check < in.dat

assemble: $(ASM_FILES)

$(ASM_DIR)/%.s: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -S $< -o $@ $(ASM_CFLAGS)

format:
	find $(SRC_DIR) -name "*.c" -exec clang-format -style=gnu -i {} +

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run format assemble

