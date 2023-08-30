CC=gcc
CFLAGS= -std=c11 -Wall -Werror -Wpedantic
LDFLAGS= -lm
PROJECT_NAME=huffman_coding

build: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_NAME) main.c

clean:
	rm -f $(PROJECT_NAME)
