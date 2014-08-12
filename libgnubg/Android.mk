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



LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)  

LOCAL_SRC_FILES := \
		evallock.c \
    lib/list.c \
    lib/neuralnet.c \
    lib/mt19937ar.c \
    lib/isaac.c \
    lib/md5.c \
    lib/cache.c \
    lib/inputs.c \
		mtsupport.c \
		multithread.c \
		output.c \
		timer.c \
    globals.c \
    async.c \
    dice.c \
    bearoff.c \
    bearoffgammon.c \
    imported_functions.c \
		jniAPI.c \
    levels.c \
    matchequity.c \
    matchid.c \
    nativeAPI.c \
    util.c \
    mec.c \
    positionid.c \
    eval.c \
    rollout.c \
		test.c


LOCAL_MODULE := libgnubg
LOCAL_MODULE_FILENAME := libgnubg

LOCAL_CFLAGS := \
	-DIS_ANDROID \
	-DIS_LIBRARY \
  -ffast-math -O3 -funroll-loops \
	-DHAVE_CONFIG_H -g -w \
  -DAC_DATADIR=\"/\" \
  -DAC_PKGDATADIR=\"/\" \
  -DAC_DOCDIR=\"/\" \

LOCAL_C_INCLUDES :=			\
	$(GLIB_TOP)/	\
	$(GLIB_TOP)/glib 	\
	$(GLIB_TOP)/glib/gthread 	\
	$(GLIB_TOP)/glib/android \
	$(LIBGNUBG_TOP)/lib

LOCAL_SHARED_LIBRARIES := \
	libgthread-2.0 \
	libglib-2.0

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY) 
