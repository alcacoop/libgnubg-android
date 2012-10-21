CC=gcc

CFLAGS=-DHAVE_CONFIG_H -g -O2 -DLOCALEDIR=\"/usr/share/locale\" -pthread -DAC_DATADIR=\"/usr/share\" -DAC_PKGDATADIR=\"/usr/share/gnubg\" -DAC_DOCDIR=\"/usr/share/doc/gnubg/\" -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDES=-I. -I./lib


OBJS=analysis.o \
		 bearoff.o \
		 bearoffgammon.o \
		 boardpos.o \
		 copying.o \
		 credits.o \
		 dbprovider.o \
		 dice.o \
		 drawboard.o \
		 evallock.o \
		 eval.o \
		 export.o \
		 external.o \
		 external_l.o \
		 external_y.o \
		 file.o \
		 format.o \
		 formatgs.o \
		 gnubg.o \
		 gnubgmodule.o \
		 html.o \
		 htmlimages.o \
		 import.o \
		 latex.o \
		 matchequity.o \
		 matchid.o \
		 mec.o \
		 multithread.o \
		 openurl.o \
		 osr.o \
		 play.o \
		 positionid.o \
		 progress.o \
		 relational.o \
		 render.o \
		 renderprefs.o \
		 rollout.o \
		 set.o \
		 sgf.o \
		 sgf_l.o \
		 sgf_y.o \
		 show.o \
		 simpleboard.o \
		 sound.o \
		 speed.o \
		 text.o \
		 timer.o \
		 util.o 

LDFLAGS=-L./lib lib/libevent.a -lglib-2.0 -lm 


all: libs src
	$(CC) -o gnubg $(OBJS) $(LDFLAGS)

%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<

src: $(OBJS) 

libs:
	make -C lib 

clean: 
	make -C lib clean
	rm -rf $(OBJS) gnubg

