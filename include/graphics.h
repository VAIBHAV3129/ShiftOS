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
SHIFTOS_CALL u64 gfx_width(void);
SHIFTOS_CALL u64 gfx_height(void);

SHIFTOS_CALL void gfx_clear(u32 color);
SHIFTOS_CALL void gfx_put_pixel(u64 x, u64 y, u32 color);
SHIFTOS_CALL void gfx_put_pixel_alpha(u64 x, u64 y, u32 color);
SHIFTOS_CALL void gfx_fill_rect(u64 x, u64 y, u64 w, u64 h, u32 color);
SHIFTOS_CALL void gfx_fill_rect_alpha(u64 x, u64 y, u64 w, u64 h, u32 color);
SHIFTOS_CALL void gfx_draw_line(u64 x0, u64 y0, u64 x1, u64 y1, u32 color);
SHIFTOS_CALL void gfx_draw_rect(u64 x, u64 y, u64 w, u64 h, u32 color);
SHIFTOS_CALL void gfx_draw_gradient(u64 x, u64 y, u64 w, u64 h, u32 top, u32 bottom);
SHIFTOS_CALL void gfx_draw_checker(u64 size, u32 c1, u32 c2);

SHIFTOS_CALL void gfx_draw_circle(u64 cx, u64 cy, u64 r, u32 color);
SHIFTOS_CALL void gfx_fill_circle(u64 cx, u64 cy, u64 r, u32 color);

#endif
