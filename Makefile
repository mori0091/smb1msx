# -*- coding: utf-8-unix; tab-width: 8 -*-

# Path to top of the libmsx installed folder.
LIBMSX_HOME ?= ./libmsx

NAME = smb1msx

CFLAGS  = -DNDEBUG --opt-code-speed -I./include
LDFLAGS =
LDLIBS  =
LIBS    =

include ${LIBMSX_HOME}/mk/ascii16.mk
include ${LIBMSX_HOME}/mk/build.mk

run: build
	openmsx.exe -cart ${BINDIR}/${NAME}.rom -machine C-BIOS_MSX2+
