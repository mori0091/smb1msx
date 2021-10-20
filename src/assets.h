// -*- coding: utf-8-unix -*-

#ifndef ASSETS_H
#define ASSETS_H

#pragma once

#include <stdlib.h>
#include <msx.h>

/* defined sprite pattern table */
extern const size_t smb1spt_size;
extern const char smb1spt[];

/* defined sprite color table */
extern const size_t smb1sct_size;
extern const char smb1sct[];

/* defined tileset */
extern const size_t smb1tile_size;
extern const char smb1tile[];

/* stage map */
extern const size_t smb1map_size;
extern const char smb1map[];

/* default color palette */
extern const palette_t color_palette[16];

void assets_setup(void);

#endif
