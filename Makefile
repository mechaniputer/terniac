CC=gcc

all: terniac

terniac:
	$(CC) -o terniac terniac.c

clean:
	rm -f terniac

