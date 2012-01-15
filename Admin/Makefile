
# Makefile pour generer le client d'administration


OBJS = admin.o

LOCAL_LIBS = -L ../Network  -L ../Threads -L ../IPC
LOCAL_LIBS += -lnet  -lthrd -lipc
CFLAGS += -I ../Network -I ../Threads -I ../IPC  -I ../Switch -I ../Logger

#
# Cible generale
#

all: admin

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o commutateur

#
# Les cibles pour l'executable
#

admin: $(OBJS)
	$(CC) $(CFLAGS) -o admin $(OBJS) $(LOCAL_LIBS)

admin.o: admin.c admin.h 
	$(CC) -c $(CFLAGS) $(LOCAL_LIBS) admin.c
