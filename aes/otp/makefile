PROGS = otp
CC = gcc
CFLAGS = -g -Wall 
objects = main.o otp.o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PROGS) $(objects)


main.o: main.c otp.h
	$(CC) $(CFLAGS) -c main.c

otp.o: otp.c otp.h
	$(CC) $(CFLAGS) -c otp.c

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f $(PROGS)