CC=g++
CFLAGS=-Wall -Werror -std=c++11 -O2

all : rlencode rldecode

debug : CFLAGS=-Wall -Werror -gdwarf-3 -std=c++11
debug : all

rlencode : rlencode.o
	$(CC) $(CFLAGS) -o rlencode rlencode.o

rlencode.o : rlencode.cpp rlencode.h
	$(CC) $(CFLAGS) -c rlencode.cpp

rldecode : rldecode.o
	$(CC) $(CFLAGS) -o rldecode rldecode.o

rldecode.o : rldecode.cpp rldecode.h
	$(CC) $(CFLAGS) -c rldecode.cpp

clean :
	rm rlencode rldecode *.o
