PROJECT_NAME=kro
BIN_PATH=bin
LIB_PATH=lib
SRC_PATH=src

PREFIX?=/usr/local

LDLIBS=-lkro -ldl
LDFLAGS=-L$(LIB_PATH) $(LDLIBS)

CC?=gcc
CFLAGS=-Wall -Wextra \
	-pedantic -Iinclude \
        -Wl,--no-as-needed \
	-ggdb  # Debug flag

PROJECT_BINARY_NAME?=$(PROJECT_NAME)
PROJECT_EXECUTABLE_PATH=$(BIN_PATH)/$(PROJECT_BINARY_NAME)
PROJECT_LIBRARY_PATH=$(LIB_PATH)/lib$(PROJECT_BINARY_NAME).so

MKDIR?=mkdir
MKDIRFLAGS?=-p
RM?=rm
RMFLAGS?=-rf

CMD_SOURCES?=$(wildcard $(SRC_PATH)/cmds/*.c)
cmds_objects=$(CMD_SOURCES:.c=.o)
CMD_BIN_PATHS=$(patsubst $(SRC_PATH)/cmds/%.c,$(BIN_PATH)/%,$(CMD_SOURCES))

SOURCES?=$(wildcard $(SRC_PATH)/*.c)
objects=$(SOURCES:.c=.o)

all: $(CMD_BIN_PATHS)
.PHONY: all

bin/%: $(SRC_PATH)/cmds/%.o $(PROJECT_LIBRARY_PATH)
	$(MKDIR) $(MKDIRFLAGS) $(BIN_PATH)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(PROJECT_LIBRARY_PATH): $(objects)
	$(MKDIR) $(MKDIRFLAGS) $(LIB_PATH)
	$(CC) -shared $(CFLAGS) $(objects) -o $@

%.o: %.c
	$(CC) -fPIC $(CFLAGS) -c -o $@ $^

install:
	$(MKDIR) -p $(PREFIX)/bin
	cp $(CMD_BIN_PATHS) $(PREFIX)/bin
	$(MKDIR) -p $(PREFIX)/lib
	cp $(PROJECT_LIBRARY_PATH) $(PREFIX)/lib
.PHONY: install

clean:
	$(RM) $(RMFLAGS) $(objects) $(cmds_objects)
	$(RM) $(RMFLAGS) $(BIN_PATH)
	$(RM) $(RMFLAGS) $(LIB_PATH)
.PHONY: clean
