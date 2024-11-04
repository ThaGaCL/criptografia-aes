PROGS = aes
CC = gcc
CFLAGS = -g -Wall
objects = main.o aes.o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PROGS) $(objects)


main.o: main.c aes.h
	$(CC) $(CFLAGS) -c main.c

aes.o: aes.c aes.h
	$(CC) $(CFLAGS) -c aes.c

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f $(PROGS)