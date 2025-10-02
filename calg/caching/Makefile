CFLAGS += -Wall -Wextra -Wpedantic 
CFLAGS += -O0
CFLAGS += -fsanitize=address -fsanitize=leak 
CFLAGS += -g 
CFLAGS += -I./$(INCLUDE_DIR)
CFLAGS += -MMD -MP

LDFLAGS  += -L./$(LIBS_DIR)

SRC_DIR     = src
BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
LIBS_DIR    = $(BUILD_DIR)/libs
INCLUDE_DIR = include
SCRIPTS_DIR = scripts
LIBS_LINK_ORDER = -lcache -lhshtbl -llist

CSRCS   = $(SRC_DIR)/main.c \
          $(SRC_DIR)/list.c \
          $(SRC_DIR)/hshtbl.c \
          $(SRC_DIR)/cache.c

SRC_FILES = $(notdir $(CSRCS))
COBJS     = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

LIST_OBJ    = $(OBJ_DIR)/list.o
CACHE_OBJ   = $(OBJ_DIR)/cache.o
HSHTBL_OBJ  = $(OBJ_DIR)/hshtbl.o
MAIN_OBJ    = $(OBJ_DIR)/main.o

LIST_LIB    = $(LIBS_DIR)/liblist.a
CACHE_LIB   = $(LIBS_DIR)/libcache.a
HSHTBL_LIB  = $(LIBS_DIR)/libhshtbl.a

TESTS_BIN_DIR = $(BUILD_DIR)/tests
TESTS_DIR = tests
TESTS_SRCS = $(wildcard $(TESTS_DIR)/*.c)
TESTS_FILES = $(notdir $(TESTS_SRCS))
TESTS_OBJ = $(TESTS_FILES:%.c=$(OBJ_DIR)/%.test.o)
TESTS_BIN = $(TESTS_FILES:%.c=$(TESTS_BIN_DIR)/%)
TESTS_PPFLAGS = -DTESTING -DVERBOSE

TARGET = $(BUILD_DIR)/main

all: $(TARGET) format

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	bear -- $(CC) $(CFLAGS) -c $< -o $@ 

$(LIST_LIB): $(LIST_OBJ) | $(LIBS_DIR)
	ar rcs $@ $^

$(CACHE_LIB): $(CACHE_OBJ) | $(LIBS_DIR)
	ar rcs $@ $^

$(HSHTBL_LIB): $(HSHTBL_OBJ) | $(LIBS_DIR)
	ar rcs $@ $^

$(TARGET): $(MAIN_OBJ) $(LIST_LIB) $(CACHE_LIB) $(HSHTBL_LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LIBS_LINK_ORDER)

$(OBJ_DIR) $(LIBS_DIR) $(TESTS_BIN_DIR): 
	mkdir -p $@

$(OBJ_DIR)/%.test.o: $(TESTS_DIR)/%.c | $(OBJ_DIR)
	bear -- $(CC) $(CFLAGS) $(TESTS_PPFLAGS) -c $< -o $@ 

$(TESTS_BIN_DIR)/%: $(OBJ_DIR)/%.test.o  $(LIST_LIB) $(CACHE_LIB) $(HSHTBL_LIB) | $(TESTS_BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LIBS_LINK_ORDER)

tests: $(TESTS_BIN) format
	$(SCRIPTS_DIR)/run_tests.sh -b $(TESTS_BIN_DIR) -a

run: $(TARGET)
	./build/main < ./tests/data/001.dat

format:
	$(SCRIPTS_DIR)/format.sh -s $(SRC_DIR)
	$(SCRIPTS_DIR)/format.sh -s $(TESTS_DIR)
	$(SCRIPTS_DIR)/format.sh -s $(INCLUDE_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: tests run clean format
.SECONDARY: $(TESTS_OBJ)

ALL_OBJ = $(wildcard $(OBJ_DIR)/*.o)
include $(ALL_OBJ:%.o=%.d)

