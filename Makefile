 ##################################################################
 #                     GNU BACKGAMMON MOBILE                      #
 ##################################################################
 #                                                                #
 #  Authors: Domenico Martella - Davide Saurino                   #
 #  E-mail: info@alcacoop.it                                      #
 #  Date:   19/12/2012                                            #
 #                                                                #
 ##################################################################
 #                                                                #
 #  Copyright (C) 2012   Alca Societa' Cooperativa                #
 #                                                                #
 #  This file is part of GNU BACKGAMMON MOBILE.                   #
 #  GNU BACKGAMMON MOBILE is free software: you can redistribute  # 
 #  it and/or modify it under the terms of the GNU General        #
 #  Public License as published by the Free Software Foundation,  #
 #  either version 3 of the License, or (at your option)          #
 #  any later version.                                            #
 #                                                                #
 #  GNU BACKGAMMON MOBILE is distributed in the hope that it      #
 #  will be useful, but WITHOUT ANY WARRANTY; without even the    #
 #  implied warranty of MERCHANTABILITY or FITNESS FOR A          #
 #  PARTICULAR PURPOSE.  See the GNU General Public License       #
 #  for more details.                                             #
 #                                                                #
 #  You should have received a copy of the GNU General            #
 #  Public License v3 along with this program.                    #
 #  If not, see <http://http://www.gnu.org/licenses/>             #
 #                                                                #
 ##################################################################


NDK_PROJECT_PATH=${pwd}
CC=gcc

CFLAGS=-DIS_LIBRARY -DHAVE_CONFIG_H -g -O2 -DLOCALEDIR=\"/usr/share/locale\" -pthread -DAC_DATADIR=\"/usr/share\" -DAC_PKGDATADIR=\"/usr/share/gnubg\" -DAC_DOCDIR=\"/usr/share/doc/gnubg/\" -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -Ilib -fPIC
INCLUDES=-Ilibgnubg -Ilibgnubg/lib -I/usr/lib/jvm/java-6-openjdk-amd64/include/


OBJS= \
    libgnubg/lib/list.o \
    libgnubg/lib/neuralnet.o \
    libgnubg/lib/mt19937ar.o \
    libgnubg/lib/isaac.o \
    libgnubg/lib/md5.o \
    libgnubg/lib/cache.o \
    libgnubg/lib/inputs.o \
		libgnubg/mtsupport.o \
		libgnubg/multithread.o \
		libgnubg/output.o \
		libgnubg/timer.o \
    libgnubg/globals.o \
    libgnubg/async.o \
    libgnubg/dice.o \
    libgnubg/bearoff.o \
    libgnubg/bearoffgammon.o \
    libgnubg/imported_functions.o \
    libgnubg/levels.o \
    libgnubg/matchequity.o \
    libgnubg/matchid.o \
    libgnubg/util.o \
    libgnubg/mec.o \
    libgnubg/positionid.o \
		libgnubg/eval.o \
		libgnubg/rollout.o \
		libgnubg/evallock.o \
		libgnubg/jniAPI.o \
    libgnubg/nativeAPI.o \
    libgnubg/test.o



LDFLAGS=-Llibgnubg/lib -lm -fPIC

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

ndk-clean:
	ndk-build clean

ndk:
	ndk-build
	make ndk-install

ndk-debug:
	ndk-build NDK_DEBUG=1
	make ndk-install

ndk-install: 
	cp -rf libs/armeabi/libgnubg* /home/dmt/Progetti/Android/workspace/gnubg-gdx/Backgammon-android/libs/armeabi/  
	cp -rf libs/armeabi-v7a/libgnubg* /home/dmt/Progetti/Android/workspace/gnubg-gdx/Backgammon-android/libs/armeabi-v7a/  
	cp -rf libs/x86/libgnubg* /home/dmt/Progetti/Android/workspace/gnubg-gdx/Backgammon-android/libs/x86/
