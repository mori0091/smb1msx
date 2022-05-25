# -*- coding: utf-8-unix; tab-width: 8 -*-

NAME = pbm2tile

CFLAGS = -std=c11 -Wall -Wpedantic

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

TARGET = ${BINDIR}/${NAME}

SRCS = ${SRCDIR}/pbm2tile.c

.PHONY: all build clean

all: build

build: ${TARGET}

clean:
	@rm -f ${TARGET}

${TARGET}: ${SRCS}
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ $^
