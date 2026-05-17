#ifndef SHIFTOS_GRAPHICS_H
#define SHIFTOS_GRAPHICS_H

#include "types.h"
#include "compiler.h"

struct gfx_context {
    u32 *buffer;
    u64 width;
    u64 height;
    u64 pitch;
    u64 stride;
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
SHIFTOS_CALL void gfx_draw_rect(u64 x, u64 y, u64 w, u64 h, u32 color);
SHIFTOS_CALL void gfx_draw_gradient(u64 x, u64 y, u64 w, u64 h, u32 color_top, u32 color_bottom);
SHIFTOS_CALL void gfx_draw_checker(u64 size, u32 c1, u32 c2);

SHIFTOS_CALL void gfx_draw_text(u64 x, u64 y, const char *text, u32 color);
SHIFTOS_CALL void gfx_draw_text_glow(u64 x, u64 y, const char *text, u32 color, u32 glow_color);
SHIFTOS_CALL void gfx_measure_text(const char *text, u64 *w, u64 *h);

SHIFTOS_CALL void gfx_draw_window(u64 x, u64 y, u64 w, u64 h, const char *title, u32 accent, u32 bg);
SHIFTOS_CALL void gfx_draw_textf(u64 x, u64 y, u32 color, const char *label, u64 value);

#endif
