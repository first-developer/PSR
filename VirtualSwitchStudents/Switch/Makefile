#
# Makefile pour generer le commutateur virtuel
#

OBJS = commutateur.o gestionConnexions.o algorithmeCommutateur.o gestionAdmin.o commandesAdmin.o
LIBS = -lpthread
LOCAL_LIBS = -L ../Network # -L ../Threads -L ../IPC
LOCAL_LIBS += -lnet # -lthrd -lipc
CFLAGS += -I ../Network -I ../Threads -I ../IPC 

#
# Cible generale
#

all: commutateur

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o commutateur

#
# Les cibles pour l'executable
#

commutateur: $(OBJS)
	$(CC) $(CFLAGS) -o commutateur $(OBJS) $(LOCAL_LIBS) $(LIBS)

commutateur.o: commutateur.c commutateur.h

gestionConnexions.o: gestionConnexions.c commutateur.h gestionConnexions.h

algorithmeCommutateur.o: algorithmeCommutateur.c commutateur.h algorithmeCommutateur.h

gestionAdmin.o: gestionAdmin.c commutateur.h gestionAdmin.h commandesAdmin.h

commandesAdmin.o: commandesAdmin.c commutateur.h commandesAdmin.h
