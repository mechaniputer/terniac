CFLAGS = --std=c89 -pedantic -Wall -g
CC = gcc

all: terniac

terniac: terniac.o ternutils.o instructions.o
	$(CC) -o $@ $^

terniac.o: terniac.c ternutils.h instructions.h

ternutils.o: ternutils.c

instructions.o: instructions.c

clean:
	rm -f terniac *.o

