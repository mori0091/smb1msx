// -*- coding: utf-8-unix -*-

#include "config.h"
#include "../bank1/boot.h"

void smb1_main(void) __banked;

void main(void) {
  boot_main();
  smb1_main();
}
