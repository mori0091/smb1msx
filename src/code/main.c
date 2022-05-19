// -*- coding: utf-8-unix -*-

#include <config.h>

#include "boot.h"
#include "smb1.h"

void main(void) {
  boot_main();
  smb1_main();
}
