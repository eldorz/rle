CC=g++
CFLAGS=-Wall -Werror -std=c++11 -O2

all : rlencode rldecode

debug : CFLAGS=-Wall -Werror -gdwarf-3 -std=c++11
debug : all

profile : CFLAGS=-Wall -Werror -pg -std=c++11
profile : all

rlencode : rlencode.o common.o
	$(CC) $(CFLAGS) -o rlencode rlencode.o common.o

rlencode.o : rlencode.cpp rlencode.h common.h
	$(CC) $(CFLAGS) -c rlencode.cpp

rldecode : rldecode.o common.o
	$(CC) $(CFLAGS) -o rldecode rldecode.o common.o

rldecode.o : rldecode.cpp rldecode.h common.h
	$(CC) $(CFLAGS) -c rldecode.cpp

common.o : common.cpp common.h
	$(CC) $(CFLAGS) -c common.cpp

clean :
	rm rlencode rldecode *.o
