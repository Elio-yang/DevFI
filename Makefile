CC = gcc

CFLAGS = -O2 -Wall

SRC_FILES = $(wildcard *.c)

BIN_FILES = $(SRC_FILES:.c=.bin)

all: $(BIN_FILES)

%.bin: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN_FILES)