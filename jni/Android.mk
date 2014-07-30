# This file is the top android makefile for all sub-modules.

LOCAL_PATH := $(call my-dir)

GLIB_TOP := $(LOCAL_PATH)/..
LIBGNUBG_TOP := $(LOCAL_PATH)/../libgnubg

include $(CLEAR_VARS)

include $(GLIB_TOP)/glib/Android.mk
include $(GLIB_TOP)/gthread/Android.mk
include $(GLIB_TOP)/libgnubg/Android.mk
