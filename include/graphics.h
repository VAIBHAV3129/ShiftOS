#ifndef SHIFTOS_GRAPHICS_H
#define SHIFTOS_GRAPHICS_H

#include "types.h"
#include "compiler.h"

struct gfx_context {
    u32 *buffer;
    u64 width;
    u64 height;
    u64 pitch;
    u16 bpp;
};

SHIFTOS_CALL int gfx_init(void);
SHIFTOS_CALL void gfx_clear(u32 color);
SHIFTOS_CALL void gfx_put_pixel(u64 x, u64 y, u32 color);

#endif
