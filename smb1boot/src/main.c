// -*- coding: utf-8-unix -*-

#include <rom_mapper.h>

#include "boot.h"

void main(void) {
  boot_main();
  rom_banked_jump(1);
}
