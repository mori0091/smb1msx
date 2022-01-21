# -*- coding: utf-8-unix; tab-width: 8 -*-

SRC = smb1_sprite_tileset_1.png
PBM = smb1_sprite_tileset_1_1.pbm \
      smb1_sprite_tileset_1_2.pbm

.phony: all build clean

all: build

build: ${PBM}

clean:
	@rm -f ${PBM}

${PBM}: ${SRC}
	python3 make_sprite4.py $<
