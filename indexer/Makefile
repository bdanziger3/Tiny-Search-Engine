# Makefile for 'indexer' module
# 
# Blake Danziger, May 2020

L = ../libcs50
C = ../common
CC = gcc
TEST = -DMYTEST
TESTCFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTDEF) -I$L -I$C
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
PROG = indexer
TEST = indextest
OBJS = indexer.o $C/pagedir.o $C/word.o $C/index.o $C/common.a
TESTOBJS = indextest.o $C/pagedir.o $C/word.o $C/index.o $C/common.a
LLIBS = $L/libcs50.a
CLIBS = $C/common.a
MAKE = make

all: $(PROG) $(TEST)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(CLIBS) -o $@

$(TEST): $(TESTOBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(CLIBS) -o $@

$(PROGTEST): $(OBJS)
	$(CC) $(TESTCFLAGS) $^ $(LLIBS) $(CLIBS) -o $@

$(TESTTEST): $(TESTOBJS)
	$(CC) $(TESTCFLAGS) $^ $(LLIBS) $(CLIBS) -o $@

indexer.o: indexer.c

indextest.o: indextest.c indexer.c

../common/pagedir.o: $C/pagedir.h

../common/word.o: $C/word.h

../common/index.o: $C/index.h

../common/common.a: $C/pagedir.h $C/word.h $C/index.h
	$(MAKE) -C $C


.PHONY: all clean test valgrind

test: $(PROGTEST) $(TESTTEST)
	./testing.sh 2>&1 | tee testing.out

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) ../crawler/data2 indexdata/index_2

	
clean:
	rm -f *~ *.o
	rm -f core
	rm -f $(PROG)
	rm -f $(TEST)
	rm -f *.dSYM
	$(MAKE) -C ../common clean