# -*- coding: utf-8-unix; tab-width: 8 -*-

.PHONY: all build clean

BINDIR = ./bin

TARGETS = ${TILEC}

TILEC = ${BINDIR}/tilec

all: build

build: ${TARGETS}

clean:
	@rm -f ${TARGETS}
	@rm -rf ${BINDIR}

${TILEC}: ./src/tilec.c
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CC} -o $@ $^
