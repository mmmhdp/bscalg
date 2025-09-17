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

LDFLAGS = -lc -lm

INCLUDES = -I./include
LIBS_DIR = libs
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests
TESTS_DATA_DIR = tests/data
SCRIPTS_DIR = ./scripts

SOURCES = $(SRC_DIR)/main.c \
	  $(SRC_DIR)/llst.c \
	  $(SRC_DIR)/hshtbl.c \
	  $(SRC_DIR)/cache.c

TARGETS = $(BUILD_DIR)/main

all: $(TARGETS) run

$(TARGETS): $(SOURCES)
	mkdir -p "$(BUILD_DIR)" && \
	mkdir -p "$(LIBS_DIR)" &&\
	bear -- $(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

mk-lib: mk-llst-lib mk-hshtbl-lib mk-cache-lib

mk-llst-lib: 
	gcc -c $(SRC_DIR)/llst.c $(INCLUDES) -o $(LIBS_DIR)/libllst.o && \
        ar rcs $(LIBS_DIR)/libllst.a $(LIBS_DIR)/libllst.o

mk-hshtbl-lib: 
	gcc -c $(SRC_DIR)/hshtbl.c $(INCLUDES) -o $(LIBS_DIR)/libhshtbl.o && \
        ar rcs $(LIBS_DIR)/libhshtbl.a $(LIBS_DIR)/libhshtbl.o

mk-cache-lib: 
	gcc -c $(SRC_DIR)/cache.c $(INCLUDES) -o libs/libcache.o && \
        ar rcs libs/libcache.a libs/libcache.o
	
run:
	$(SCRIPTS_DIR)/run_main_test.sh -s $(TESTS_DATA_DIR)

format:
	$(SCRIPTS_DIR)/format.sh -s $(SRC_DIR)

clean:
	rm -rf $(BUILD_DIR) $(LIBS_DIR)  && rm ./compile_commands.json

.PHONY: all clean run format mk-cache-lib
