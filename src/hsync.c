// -*- coding: utf-8-unix -*-

#include "smb1.h"

#define HSYNC_LINE (13)

volatile uint16_t scroll_x;

static volatile bool interrupted;

inline void vsync_handler(void) {
  // vdp_set_hscroll(0);
  {
    VDP_SET_CONTROL_REGISTER(26, 0);
    VDP_SET_CONTROL_REGISTER(27, 0);
  }
  interrupted = true;
}

inline void vsync_handler_epilogue(void) {
  vsync_busy = false;
  JIFFY++;
}

inline void hsync_handler(void) {
  // ---- HSYNC ----
  // vdp_set_hscroll(...);
  {
    VDP_SET_CONTROL_REGISTER(26, RG26SA);
    VDP_SET_CONTROL_REGISTER(27, RG27SA);
  }
  {
    volatile uint16_t x = scroll_x;
    RG26SA = (x >> 3) & 0x3f;
    RG27SA = (~x + 1) & 0x07;
    if (RG27SA) {
      RG26SA++;
    }
  }
  interrupted = false;
}

static void interrupt_handler(void) {
  // Checking if HSYNC is caught.
  VDP_SET_STATUS_REGISTER_POINTER(1);
  uint8_t hsync = (VDP_GET_STATUS_REGISTER_VALUE() & 1);
  // Don't forget reset the status register pointer to 0
  VDP_SET_STATUS_REGISTER_POINTER(0);
  if (hsync) {
    hsync_handler();
    // ---- override BIOS VSYNC routine
    if (VDP_GET_STATUS_REGISTER_VALUE() & 0x80) {
      // Catching up on delayed VSYNC.
      // Note that we do not call `vsync_handler()` here.
      vsync_handler_epilogue();
      __asm__("ei");
      sound_player();
      return;
    }
    __asm__("ei");
    return;
  }
  // ---- override BIOS VSYNC routine
  if (VDP_GET_STATUS_REGISTER_VALUE() & 0x80) {
    vsync_handler();
    vsync_handler_epilogue();
    __asm__("ei");
    sound_player();
  }
}

void setup_interrupt(void) {
  interrupted = false;
  set_hscroll(0);
  vdp_set_control(19, (RG19SA = HSYNC_LINE));
  vdp_set_control(0, (RG0SAV |= 0x10));
  set_interrupt_handler(interrupt_handler);
}

void await_hsync(void) {
  await_vsync();
  while (interrupted) {
    // spin lock
  }
}
