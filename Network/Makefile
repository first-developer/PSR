#
# Makefile pour generer la bibliotheque de fonctions reseau
#

CFLAGS += -I ../Network -I ../Threads -I ../IPC  -I ../Switch

all: libnet.a
	

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o libnet.a

#
# Les cibles pour la bibliotheque
#

libnet.o: libnet.c libnet.h
	$(CC) -c $(CFLAGS)  libnet.c
 
libnet.a: libnet.o
	rm -f $@
	$(CLIB) $@ $+
