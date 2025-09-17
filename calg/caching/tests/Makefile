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

LDFLAGS = -L../libs -lc -lm -lcache -lllst -lhshtbl

INCLUDES = -I../include/
LIBS_DIR = ../libs
SRC_DIR = ./
BUILD_DIR = ../build/tests
SCRIPTS_DIR = ../scripts

SOURCES = $(SRC_DIR)/test_llst.c \
	  $(SRC_DIR)/test_hshtbl.c \
	  $(SRC_DIR)/test_cache.c

TARGETS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))

all: $(TARGETS) run-tests

$(BUILD_DIR)/%: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $@

run-tests:
	../scripts/run_tests.sh

format:
	find $(SRC_DIR) -name "*.c" -exec clang-format -style=gnu -i {} +

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run format
