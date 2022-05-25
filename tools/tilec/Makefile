# -*- coding: utf-8-unix; tab-width: 8 -*-

NAME = tilec

CFLAGS = -std=c11 -Wall -Wpedantic

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

TARGET = ${BINDIR}/${NAME}

SRCS = ${SRCDIR}/tilec.c

.PHONY: all build clean

all: build

build: ${TARGET}

clean:
	@rm -f ${TARGET}

${TARGET}: ${SRCS}
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ $^
