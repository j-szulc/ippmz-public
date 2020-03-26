CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

.PHONY: all clean

all: main

main: main.o cli.o entities.o bst.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c cli.h entities.h status.h
	$(CC) $(CFLAGS) -c $<

cli.o: cli.c cli.h bst.h entities.h status.h
	$(CC) $(CFLAGS) -c $<

entities.o: entities.c entities.h bst.h status.h
	$(CC) $(CFLAGS) -c $<

bst.o: bst.c bst.h entities.h status.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
