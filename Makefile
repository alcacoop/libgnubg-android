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
INCLUDES=-I. -I./lib -I/usr/lib/jvm/java-6-openjdk-amd64/include/


OBJS= \
    lib/list.o \
    lib/neuralnet.o \
    lib/mt19937ar.o \
    lib/isaac.o \
    lib/md5.o \
    lib/cache.o \
    lib/inputs.o \
		mtsupport.o \
		multithread.o \
		output.o \
		timer.o \
    globals.o \
    async.o \
    dice.o \
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
		eval.o \
		rollout.o \
		evallock.o \
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

ndk:
	ndk-build
	cp -rf libs/armeabi/* /home/dmt/Progetti/Android/workspace/gnubg-gdx/Backgammon-android/libs/armeabi/  
	cp -rf libs/armeabi-v7a/* /home/dmt/Progetti/Android/workspace/gnubg-gdx/Backgammon-android/libs/armeabi-v7a/  
	cp -rf libs/x86/* /home/dmt/Progetti/Android/workspace/gnubg-gdx/Backgammon-android/libs/x86/
