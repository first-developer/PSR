#
# Makefile pour generer la bibliotheque de fonctions reseau
#

INC_DIRS =  -I ../Utils

export CC = gcc
export CLIB = ar cq
export CFLAGS = -g -Wall -DDEBUG

all: 	libnet.a

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o libnet.a

#
# Les cibles pour la bibliotheque
#
libnet.o: libnet.c libnet.h
	$(CC) -c $(CFLAGS) $(INC_DIRS) $(LIBS) libnet.c
libnet.a: libnet.o
	rm -f $@
	$(CLIB) $@ $+