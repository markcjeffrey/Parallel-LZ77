
CC=g++
CFLAGS=-O3 -fopenmp

all: plz77 ansv2

.PHONY: clean

clean: 
	rm -rf PLZ77 *~ *.o

ANSV.o: ANSV.cpp ANSV.h
	$(CC) $(CFLAGS) -c ANSV.cpp
	
RMQ.o: RMQ.cpp RMQ.h
	$(CC) $(CFLAGS) -c RMQ.cpp
	
PrefixSum.o: PrefixSum.cpp PrefixSum.h
	$(CC) $(CFLAGS) -c PrefixSum.cpp
	
PLZ77.o: PLZ77.cpp
	$(CC) $(CFLAGS)  -c PLZ77.cpp
	
rangeMin.o: rangeMin.cpp rangeMin.h
	$(CC) $(CFLAGS) -c rangeMin.cpp

ansv2.o: ansv2.cpp ansv2.h
	$(CC) $(CFLAGS) -c ansv2.cpp
	
ansvTest.o: ansvTest.cpp 
	$(CC) $(CFLAGS) -c ansvTest.cpp
	
ansv2: ansvTest.o ansv2.o
	$(CC) $(CFLAGS) -o $@ ansvTest.o ansv2.o
	
plz77: PLZ77.o ANSV.o RMQ.o PrefixSum.o rangeMin.o ansv2.o
	$(CC) $(CFLAGS) -o $@ PLZ77.o ANSV.o RMQ.o PrefixSum.o rangeMin.o ansv2.o