# -*- coding: utf-8-unix; tab-width: 8 -*-

# Path to top of the libmsx installed folder.
LIBMSX_HOME ?= ./libmsx

NAME = smb1msx

CFLAGS  = -DNDEBUG --opt-code-speed
LDFLAGS =
LDLIBS  =
LIBS    =

include ${LIBMSX_HOME}/mk/ascii16.mk
include ${LIBMSX_HOME}/mk/build.mk

# -- for build compile_commands.json --
# case 1: build on WSL, use on Windows (Mingw)
CWD = $(shell wslpath -a -m ${CURDIR})
# case 2: build on Windows (Mingw), use on Windows (Mingw)
# CWD = $(shell cygpath -a -m ${CURDIR})
# default: build and use on same system
# CWD = ${CURDIR}

${OBJDIR}/%.compdb_entry: ${SRCDIR}/%.c
	@echo "    {" > $@
	@echo "        \"command\": \"${CC} -I ${LIBMSX_HOME}/include -c $<\"," >> $@
	@echo "        \"directory\": \"${CWD}\"," >> $@
	@echo "        \"file\": \"$<\"" >> $@
	@echo "    }," >> $@

COMPDB_ENTRIES = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.compdb_entry, ${SRCS_C})

compile_commands.json: ${COMPDB_ENTRIES}
	@echo "[" > $@.tmp
	@cat $^ >> $@.tmp
	@sed '$$d' < $@.tmp > $@
	@echo "    }" >> $@
	@echo "]" >> $@
	@rm $@.tmp
