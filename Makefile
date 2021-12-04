# -*- coding: utf-8-unix; tab-width: 8 -*-

# Path to top of the libmsx installed folder.
LIBMSX_HOME ?= ./libmsx

NAME = smb1msx

CFLAGS  = -DNDEBUG --opt-code-speed
LDFLAGS =
LDLIBS  =
LIBS    =

include ${LIBMSX_HOME}/mk/32k.4000.mk
include ${LIBMSX_HOME}/mk/build.mk
