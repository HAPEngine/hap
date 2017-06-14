BIN_PATH=bin
CC=gcc
CFLAGS=-I include
KRO_BINARY_NAME=kro
KRO_EXECUTABLE_PATH=$(BIN_PATH)/$(KRO_BINARY_NAME)

MKDIR=mkdir -p
RM=rm
RMFLAGS=-rf

SOURCES=$(wildcard src/*.c)
objects=$(patsubst src/%.c, src/%.o, $(SOURCES))

all: $(KRO_EXECUTABLE_PATH)
.PHONY: all

$(KRO_EXECUTABLE_PATH): $(objects)
	$(MKDIR) -p $(BIN_PATH)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(RMFLAGS) $(objects)
	$(RM) $(RMFLAGS) $(BIN_PATH)
.PHONY: clean
