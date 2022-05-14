# -*- coding: utf-8-unix; tab-width: 8 -*-

.PHONY: all build clean

all: build

build: bin/smb1msx.rom

clean:
	@rm -rf bin
	@make -s -C smb1boot clean
	@make -s -C smb1main clean

bin/smb1msx.rom: smb1boot/bin/boot.rom smb1main/bin/main.rom
	@${info [Build]	$@}
	@mkdir -p $(dir $@)
	@cat $^ > $@

smb1boot/bin/boot.rom:
	@make -C smb1boot

smb1main/bin/main.rom:
	@make -C smb1main
