#
# Makefile for the IPC library
#

#
# Constantes pour la compilation des programmes
#

OBJS = libipc.o

all: libipc.a

#
# Cleaning target
#

clean: 
	rm -f core *.o libipc.a

#
# Library target
#

libipc.o: libipc.c libipc.h
	$(CC) -c $(CFLAGS) $(LIBS) libipc.c
 
libipc.a: $(OBJS)
	rm -f $@
	$(AR) rcs $@ $+
