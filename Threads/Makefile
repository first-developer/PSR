#
# Makefile pour generer la bibliotheque des processus legers
#

INC_DIRS =  -I ../Utils -I ../Switch
export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -g -Wall -DDEBUG

all:	libthrd.a

clean:
	rm -f core *.o libthrd.a

libthrd.o: libthrd.c  libthrd.h
	    $(CC) -c $(CFLAGS) $(INC_DIRS) libthrd.c 
	    
libthrd.a: libthrd.o
	   rm -f $@
	   $(CLIB) $@ $+