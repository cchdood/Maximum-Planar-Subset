CC = g++
CFLAGS = -c

all	: bin/mps

bin/mps         : main.o
			$(CC) -o bin/mps main.o
main.o          : src/main.cpp
			$(CC) $(CFLAGS) src/main.cpp