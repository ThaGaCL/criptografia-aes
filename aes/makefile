PROGS = aes
CC = gcc
CFLAGS = -g -Wall
objects = main.o aes.o gf.o otp.o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PROGS) $(objects)


main.o: main.c aes.h
	$(CC) $(CFLAGS) -c main.c

aes.o: aes.c aes.h gf.h otp.h
	$(CC) $(CFLAGS) -c aes.c

gf.o: gf.c gf.h
	$(CC) $(CFLAGS) -c gf.c

otp.o: otp.c otp.h
	$(CC) $(CFLAGS) -c otp.c

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f $(PROGS)