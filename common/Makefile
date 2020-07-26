# Makefile for 'common' module
# 
# Blake Danziger, May 2020

L = ../libcs50
LIB = common.a
OBJS = pagedir.o word.o index.o $L/libcs50.a
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $^

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
word.o: word.h
index.o: index.h

.PHONY: all clean

all: $(LIB) pagedir.o word.o index.o

# clean up after our compilation
clean:
	rm -f *~ *.o
	rm -f core
	rm -f $(LIB)
	rm -f *.dSYM
	rm -f core