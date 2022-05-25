# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

.PHONY: all build clean

all: build

build:
	@${MAKE} -C tilec
	@${MAKE} -C pbm2tile

clean:
	@${MAKE} -C tilec clean
	@${MAKE} -C pbm2tile clean
