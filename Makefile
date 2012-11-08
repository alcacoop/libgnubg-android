CC=gcc

CFLAGS=-DIS_LIBRARY -DHAVE_CONFIG_H -g -O2 -DLOCALEDIR=\"/usr/share/locale\" -pthread -DAC_DATADIR=\"/usr/share\" -DAC_PKGDATADIR=\"/usr/share/gnubg\" -DAC_DOCDIR=\"/usr/share/doc/gnubg/\" -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -Ilib -fPIC
INCLUDES=-I. -I./lib -I/usr/lib/jvm/java-6-openjdk-amd64/include/


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
    nativeAPI.o \
    util.o \
    mec.o \
    positionid.o \
    rollout.o \
		jniAPI.o \
    test.o


LDFLAGS=-L./lib -lm -fPIC

all: src
	$(CC) -o test $(OBJS) $(LDFLAGS) -lglib-2.0

%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<

src: $(OBJS) 

clean: 
	rm -rf $(OBJS) test libgnubg64.so

jar: src
	$(CC)  -shared -o libgnubg64.so $(OBJS) glib-static/libpcre.a glib-static/libglib.a glib-static/libcharset.a $(LDFLAGS)
	jar cvf /home/dmt/Progetti/Android/workspace/gnubg-gdx/GnuBackgammon-desktop/libs/gnubg.jar libgnubg64.so

