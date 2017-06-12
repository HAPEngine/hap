BIN_PATH=bin
CC=gcc
CFLAGS=

MKDIR=mkdir -p

SOURCES=$(wildcard src/*.c)

objects=$(patsubst src/%.c, src/%.o, $(SOURCES))

bin/kro: $(objects)
	$(MKDIR) $(BIN_PATH)
	$(CC) $(CFLAGS) -o $@ $<
