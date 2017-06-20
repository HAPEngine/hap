PROJECT_NAME := kro

BIN_PATH = bin
LIB_PATH = lib
SRC_PATH = src

lib_src_path = $(SRC_PATH)/lib
bin_src_path = $(SRC_PATH)/cmds

PREFIX := /usr/local

LDLIBS=-lkro -ldl
LDFLAGS=-L$(LIB_PATH) $(LDLIBS)

CC := gcc
CFLAGS=-Wall -Wextra -pedantic -Iinclude

PROJECT_BINARY_NAME := $(PROJECT_NAME)
PROJECT_EXECUTABLE_PATH = $(BIN_PATH)/$(PROJECT_BINARY_NAME)
PROJECT_LIBRARY_PATH = $(LIB_PATH)/lib$(PROJECT_BINARY_NAME).so

CMD_SOURCES := $(wildcard $(bin_src_path)/*.c)
cmds_objects = $(CMD_SOURCES:.c=.o)
CMD_BIN_PATHS = $(patsubst $(bin_src_path)/%.c,$(BIN_PATH)/%,$(CMD_SOURCES))

MODULE_SOURCES = $(wildcard $(lib_src_path)/*.c)
module_objects = $(CMD_SOURCES:.c=.o)
MODULE_LIB_PATHS = $(patsubst $(lib_src_path)/%.c,$(LIB_PATH)/lib%.so,$(MODULE_SOURCES))

SOURCES := $(wildcard $(SRC_PATH)/*.c)
objects = $(SOURCES:.c=.o)

MKDIRFLAGS := -p
RMFLAGS := -rf

MKDIR := mkdir $(MKDIRFLAGS)
RM := rm $(RMFLAGS)

all: $(CMD_BIN_PATHS) $(MODULE_LIB_PATHS)
.PHONY: all

bin/kro: $(bin_src_path)/kro.o $(PROJECT_LIBRARY_PATH)
	$(MKDIR) $(BIN_PATH)
	$(CC) $(CFLAGS) $(LDFLAGS) $(objects) $< -o $@

$(BIN_PATH)/%: $(bin_src_path)/%.o $(PROJECT_LIBRARY_PATH)
	$(MKDIR) $(BIN_PATH)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(PROJECT_LIBRARY_PATH): $(objects)
	$(MKDIR) $(LIB_PATH)
	$(CC) -rdynamic -shared $(CFLAGS) $^ -o $@

$(LIB_PATH)/libkro_%.so: $(lib_src_path)/kro_%.c
	$(MKDIR) $(LIB_PATH)
	$(CC) -rdynamic -shared $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

install:
	$(MKDIR) -p $(PREFIX)/bin
	cp $(CMD_BIN_PATHS) $(PREFIX)/bin
	$(MKDIR) -p $(PREFIX)/lib
	cp $(PROJECT_LIBRARY_PATH) $(PREFIX)/lib
.PHONY: install

clean:
	$(RM) $(objects) $(cmds_objects)
	$(RM) $(BIN_PATH)
	$(RM) $(LIB_PATH)
.PHONY: clean
