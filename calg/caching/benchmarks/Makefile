CC = gcc
WARNING_AND_ERRORS = -Wall -Wextra -Wpedantic
OPTIMIZATION = -O2
XSANITIZE = -fsanitize=address -fsanitize=leak 
DBUG_INF = -g 
CFLAGS = $(DBUG_INF) \
	 $(WARNING_AND_ERRORS) \
	 $(XSANITIZE) \
	 $(OPTIMIZATION) \
	 $(INCLUDES)

LDFLAGS = -L../libs -lc -lm -lcache

INCLUDES = -I../include
SRC_DIR = ./
BUILD_DIR = ./build
TESTS_DIR = ../tests
TESTS_DATA_DIR = ../tests/data
SCRIPTS_DIR = ./../scripts

SOURCES = $(SRC_DIR)/bnch.c
TARGETS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))

all: $(TARGETS) run

$(TARGETS): $(SOURCES)
	mkdir -p "$(BUILD_DIR)" && bear -- $(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

run:
	$(BUILD_DIR)/bnch

format:
	$(SCRIPTS_DIR)/format.sh -s $(SRC_DIR)

clean:
	rm -rf $(BUILD_DIR) && rm ./compile_commands.json

.PHONY: all clean run format

