#ifndef SHIFTOS_BOOTINFO_H
#define SHIFTOS_BOOTINFO_H

#include "types.h"
#include "compiler.h"

struct framebuffer_info {
    u64 address;
    u64 width;
    u64 height;
    u64 pitch;
    u16 bpp;
    u8  memory_model;
    u8  red_mask_size;
    u8  red_mask_shift;
    u8  green_mask_size;
    u8  green_mask_shift;
    u8  blue_mask_size;
    u8  blue_mask_shift;
};

struct boot_info {
    int has_framebuffer;
    struct framebuffer_info fb;
};

SHIFTOS_CALL int bootinfo_init(void);
SHIFTOS_CALL const struct boot_info *bootinfo_get(void);

#endif
