CC=g++
CFLAGS=-std=c++11 -I/usr/local/include/ -lbroker

all: master

master:	master.o
	$(CC) $(CFLAGS) -g master.o -o master
	

master.o: master.cpp
	$(CC) $(CFLAGS) -c master.cpp

clean:
	rm *o master
