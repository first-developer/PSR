#
# Makefile du commutateur virtuel
#

#
# Constantes pour la compilation des programmes
#

export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -g -Wall -DDEBUG

#
# Constantes liees au projet
#

DIRS=Network Threads IPC Switch Admin

#
# La cible generale
#

all: 
	for dir in $(DIRS) ; do \
		( cd $$dir ; make ); \
	done   


#
# La cible de nettoyage
#

clean: 
	for dir in $(DIRS) ; do \
	  ( cd $$dir ; make clean ); \
	done

