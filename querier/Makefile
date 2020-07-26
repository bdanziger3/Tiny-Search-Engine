# Makefile for 'querier' module
# 
# Blake Danziger, May 2020

L = ../libcs50
C = ../common
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
FUZZCFLAGS = -Wall -pedantic -std=gnu11 -ggdb -I$L
PROG = querier
OBJS = querier.o $C/pagedir.o $C/word.o $C/index.o $C/common.a
FUZZ = fuzzquery
LLIBS = $L/libcs50.a
CLIBS = $C/common.a
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(CLIBS) -o $@

$(FUZZ): fuzzquery.c
	$(CC) $(FUZZCFLAGS) $^ $(LLIBS) -o $@


querier.o: querier.c

../common/pagedir.o: $C/pagedir.h

../common/word.o: $C/word.h

../common/index.o: $C/index.h

../common/common.a: $C/pagedir.h $C/word.h $C/index.h
	$(MAKE) -C $C

all: $(PROG) fuzzquery


.PHONY: all clean test valgrind

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) ../crawler/data2   ../indexer/indexdata/index_2

test: $(PROGTEST)
	./testing.sh 2>&1 | tee testing.out


clean:
	rm -f *~ *.o
	rm -f core
	rm -f *core.*
	rm -f $(PROG)
	rm -f *.dSYM
	$(MAKE) -C ../common clean