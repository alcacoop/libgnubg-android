CC=gcc

CFLAGS=-DIS_LIBRARY -DHAVE_CONFIG_H -g -O2 -DLOCALEDIR=\"/usr/share/locale\" -pthread -DAC_DATADIR=\"/usr/share\" -DAC_PKGDATADIR=\"/usr/share/gnubg\" -DAC_DOCDIR=\"/usr/share/doc/gnubg/\" -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -Ilib
INCLUDES=-I. -I./lib


OBJS= \
    lib/list.o \
    lib/neuralnet.o \
    lib/mt19937ar.o \
    lib/isaac.o \
    lib/md5.o \
    lib/cache.o \
    lib/inputs.o \
		globals.o \
		async.o \
		dice.o \
		eval.o \
		bearoff.o \
		bearoffgammon.o \
		imported_functions.o \
		levels.o \
		matchequity.o \
		matchid.o \
		util.o \
		mec.o \
		positionid.o \
		rollout.o \
		test.o


LDFLAGS=-L./lib lib/libevent.a -lglib-2.0 -lm 

all: src
	$(CC) -o test $(OBJS) $(LDFLAGS)

%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<

src: $(OBJS) 

clean: 
	rm -rf $(OBJS) test

