#
# Makefile for the Threads library
#
#
# Constantes pour la compilation des programmes
#

export CC = gcc
export LD = gcc
export CLIB = ar cq -lpthread
export CFLAGS = -g -Wall -DDEBUG

OBJS = libthrd.o

all: libthrd.a

#
# Cleaning target
#

clean: 
	rm -f core *.o libthrd.a

#
# Library target
#

libthrd.o: libthrd.c libthrd.h
	$(CC) -c $(CFLAGS) $(LIBS) libthrd.c
 
libthrd.a: $(OBJS)
	rm -f $@
	$(AR) rcs $@ $+
