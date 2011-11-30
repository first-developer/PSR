#
# Makefile for the Threads library
#
#
# Constantes pour la compilation des programmes
#

export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -g -Wall -DDEBUG

OBJS = libthreads.o

all: libthreads.a

#
# Cleaning target
#

clean: 
	rm -f core *.o libthreads.a

#
# Library target
#

libthreads.o: libthreads.c libthreads.h
	$(CC) -c $(CFLAGS) $(LIBS) libthreads.c
 
libthreads.a: $(OBJS)
	rm -f $@
	$(AR) rcs $@ $+
