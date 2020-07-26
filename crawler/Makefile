# Makefile for 'crawler' module
# 
# Tracey Mills, Blake Danziger, May 2020

L = ../libcs50
C = ../common
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
PROG = crawler
OBJS = crawler.o $C/pagedir.o $C/common.a
LLIBS = $L/libcs50.a
CLIBS = $C/common.a
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(CLIBS) -o $@

crawler.o: crawler.c

../common/pagedir.o: $C/pagedir.h

../common/common.a: $C/pagedir.h
	$(MAKE) -C $C


.PHONY: all clean test valgrind

all: $(PROG)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./data2/ 1

test: $(PROG)
	./testing.sh 2>&1 | tee testing.out


clean:
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f *.dSYM
	$(MAKE) -C ../common clean