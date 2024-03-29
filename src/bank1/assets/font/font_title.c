// -*- coding: utf-8-unix -*-

#pragma codeseg BANK1

#include <stdint.h>

const uint8_t font_title[] = {
  // '▉' ('a') right box
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  // '■' ('b') small right box
  0x000, // 00000000
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x000, // 00000000
  // '▉' ('c') vertical box
  0x07e, // 01111110
  0x07e, // 01111110
  0x07e, // 01111110
  0x07e, // 01111110
  0x07e, // 01111110
  0x07e, // 01111110
  0x07e, // 01111110
  0x07e, // 01111110
  // '▉' ('d') left box
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  // '◢' ('e') top-left
  0x00f, // 00001111
  0x01f, // 00011111
  0x01f, // 00011111
  0x03f, // 00111111
  0x03f, // 00111111
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  // '◣' ('f') top-right
  0x0f0, // 11110000
  0x0f8, // 11111000
  0x0f8, // 11111000
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  // '◥' ('g') bottom-left
  0x07f, // 01111111
  0x07f, // 01111111
  0x07f, // 01111111
  0x03f, // 00111111
  0x03f, // 00111111
  0x01f, // 00011111
  0x01f, // 00011111
  0x00f, // 00001111
  // '◤' ('h') bottom-right
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x0f8, // 11111000
  0x0f8, // 11111000
  0x0f0, // 11110000
  // '┗' ('i') small bottom-left
  0x07f, // 01111111
  0x07f, // 01111111
  0x03f, // 00111111
  0x03f, // 00111111
  0x01f, // 00011111
  0x007, // 00000111
  0x001, // 00000001
  0x000, // 00000000
  // '┓' ('j') small top-right
  0x000, // 00000000
  0x080, // 10000000
  0x0e0, // 11100000
  0x0f8, // 11111000
  0x0fc, // 11111100
  0x0fc, // 11111100
  0x0fe, // 11111110
  0x0fe, // 11111110
  // '|<' ('k') center-right
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fe, // 11111110
  0x0fc, // 11111100
  0x0f8, // 11111000
  0x0fc, // 11111100
  0x0fe, // 11111110
  0x0fe, // 11111110
  // 'v' ('l') top-center
  0x081, // 10000001
  0x0c3, // 11000011
  0x0c3, // 11000011
  0x0e7, // 11100111
  0x0e7, // 11100111
  0x0ff, // 11111111
  0x0ff, // 11111111
  0x0ff, // 11111111
};
