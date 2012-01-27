
OBJS = admin.o 
LIBS = -lpthread 
LOCAL_LIBS = -L ../Network  -L ../Threads -L ../IPC
LOCAL_LIBS += -pthread  -lthrd -lnet -lipc  
CFLAGS += -I ../Network -I ../Threads -I ../IPC  -I ../Switch -I ../Utils

#
# Cible generale
#

all: admin

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o admin

#
# Les cibles pour l'executable
#

admin: $(OBJS)
	$(CC) $(CFLAGS) -o admin $(OBJS) $(LOCAL_LIBS) $(LIBS)

admin.o: admin.c admin.h


