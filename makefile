PROGS = aescomp
CC = gcc
CFLAGS = -g -Wall 
objects = main.o aes.o otp.o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PROGS) $(objects) 

main.o: main.c aes/aes.h otp/otp.h
	$(CC) $(CFLAGS) -c main.c

aes.o: aes/aes.c aes/aes.h
	$(CC) $(CFLAGS) -c aes/aes.c

otp.o: otp/otp.c otp/otp.h
	$(CC) $(CFLAGS) -c otp/otp.c

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f $(PROGS)