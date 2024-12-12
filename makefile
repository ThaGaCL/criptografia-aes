PROGS = aescomp
CC = gcc
CFLAGS = -g -Wall 
objects = main.o aes.o otp.o gf.o

all: $(objects)
	$(CC) $(CFLAGS) -o $(PROGS) $(objects) 

main.o: main.c aes/aes.h aes/otp/otp.h
	$(CC) $(CFLAGS) -c main.c

aes.o: aes/aes.c aes/aes.h aes/gf.h aes/otp/otp.h
	$(CC) $(CFLAGS) -c aes/aes.c

gf.o: aes/gf.c aes/gf.h
	$(CC) $(CFLAGS) -c aes/gf.c

otp.o: aes/otp/otp.c aes/otp/otp.h
	$(CC) $(CFLAGS) -c aes/otp/otp.c

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f $(PROGS)