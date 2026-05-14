#include "graphics.h"
#include "bootinfo.h"
#include "color.h"

static struct gfx_context g_gfx;

SHIFTOS_CALL int gfx_init(void) {
    const struct boot_info *boot = bootinfo_get();
    if (!boot || !boot->has_framebuffer) {
        return 0;
    }

    g_gfx.buffer = (u32 *)(u64)boot->fb.address;
    g_gfx.width = boot->fb.width;
    g_gfx.height = boot->fb.height;
    g_gfx.pitch = boot->fb.pitch;
    g_gfx.bpp = boot->fb.bpp;

    if (!g_gfx.buffer || g_gfx.width == 0 || g_gfx.height == 0) {
        return 0;
    }

    return 1;
}

SHIFTOS_CALL u64 gfx_width(void) {
    return g_gfx.width;
}

SHIFTOS_CALL u64 gfx_height(void) {
    return g_gfx.height;
}

SHIFTOS_CALL void gfx_put_pixel(u64 x, u64 y, u32 color) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    u64 pixels_per_row = g_gfx.pitch / 4;
    g_gfx.buffer[y * pixels_per_row + x] = color;
}

SHIFTOS_CALL void gfx_put_pixel_alpha(u64 x, u64 y, u32 color) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    u64 pixels_per_row = g_gfx.pitch / 4;
    u64 idx = y * pixels_per_row + x;
    u32 dst = g_gfx.buffer[idx];
    g_gfx.buffer[idx] = color_blend(color, dst);
}

SHIFTOS_CALL void gfx_clear(u32 color) {
    u64 pixels_per_row = g_gfx.pitch / 4;
    for (u64 y = 0; y < g_gfx.height; ++y) {
        u64 row = y * pixels_per_row;
        for (u64 x = 0; x < g_gfx.width; ++x) {
            g_gfx.buffer[row + x] = color;
        }
    }
}

SHIFTOS_CALL void gfx_fill_rect(u64 x, u64 y, u64 w, u64 h, u32 color) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    u64 max_x = x + w;
    u64 max_y = y + h;

    if (max_x > g_gfx.width) {
        max_x = g_gfx.width;
    }
    if (max_y > g_gfx.height) {
        max_y = g_gfx.height;
    }

    u64 pixels_per_row = g_gfx.pitch / 4;
    for (u64 yy = y; yy < max_y; ++yy) {
        u64 row = yy * pixels_per_row;
        for (u64 xx = x; xx < max_x; ++xx) {
            g_gfx.buffer[row + xx] = color;
        }
    }
}

SHIFTOS_CALL void gfx_fill_rect_alpha(u64 x, u64 y, u64 w, u64 h, u32 color) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    u64 max_x = x + w;
    u64 max_y = y + h;

    if (max_x > g_gfx.width) {
        max_x = g_gfx.width;
    }
    if (max_y > g_gfx.height) {
        max_y = g_gfx.height;
    }

    u64 pixels_per_row = g_gfx.pitch / 4;
    for (u64 yy = y; yy < max_y; ++yy) {
        u64 row = yy * pixels_per_row;
        for (u64 xx = x; xx < max_x; ++xx) {
            u64 idx = row + xx;
            u32 dst = g_gfx.buffer[idx];
            g_gfx.buffer[idx] = color_blend(color, dst);
        }
    }
}

SHIFTOS_CALL void gfx_draw_line(u64 x0, u64 y0, u64 x1, u64 y1, u32 color) {
    s64 dx = (s64)x1 - (s64)x0;
    s64 dy = (s64)y1 - (s64)y0;

    s64 sx = (dx >= 0) ? 1 : -1;
    s64 sy = (dy >= 0) ? 1 : -1;

    s64 adx = (dx >= 0) ? dx : -dx;
    s64 ady = (dy >= 0) ? dy : -dy;

    s64 err = (adx > ady ? adx : -ady) / 2;
    s64 x = (s64)x0;
    s64 y = (s64)y0;

    for (;;) {
        if (x >= 0 && y >= 0) {
            gfx_put_pixel((u64)x, (u64)y, color);
        }

        if (x == (s64)x1 && y == (s64)y1) {
            break;
        }

        s64 e2 = err;
        if (e2 > -adx) {
            err -= ady;
            x += sx;
        }
        if (e2 < ady) {
            err += adx;
            y += sy;
        }
    }
}

SHIFTOS_CALL void gfx_draw_rect(u64 x, u64 y, u64 w, u64 h, u32 color) {
    if (w == 0 || h == 0) {
        return;
    }

    u64 x2 = x + w - 1;
    u64 y2 = y + h - 1;

    gfx_draw_line(x, y, x2, y, color);
    gfx_draw_line(x, y2, x2, y2, color);
    gfx_draw_line(x, y, x, y2, color);
    gfx_draw_line(x2, y, x2, y2, color);
}

SHIFTOS_CALL void gfx_draw_gradient(u64 x, u64 y, u64 w, u64 h, u32 top, u32 bottom) {
    if (w == 0 || h == 0) {
        return;
    }

    u32 tr = (top >> 16) & 0xFFu;
    u32 tg = (top >> 8) & 0xFFu;
    u32 tb = top & 0xFFu;

    u32 br = (bottom >> 16) & 0xFFu;
    u32 bg = (bottom >> 8) & 0xFFu;
    u32 bb = bottom & 0xFFu;

    for (u64 yy = 0; yy < h; ++yy) {
        u32 r = tr + (u32)((br - tr) * yy / (h - 1));
        u32 g = tg + (u32)((bg - tg) * yy / (h - 1));
        u32 b = tb + (u32)((bb - tb) * yy / (h - 1));
        u32 col = 0xFF000000u | (r << 16) | (g << 8) | b;
        gfx_fill_rect(x, y + yy, w, 1, col);
    }
}

SHIFTOS_CALL void gfx_draw_checker(u64 size, u32 c1, u32 c2) {
    if (size == 0) {
        return;
    }

    u64 tiles_x = (g_gfx.width + size - 1) / size;
    u64 tiles_y = (g_gfx.height + size - 1) / size;

    for (u64 ty = 0; ty < tiles_y; ++ty) {
        for (u64 tx = 0; tx < tiles_x; ++tx) {
            u32 col = ((tx + ty) & 1) ? c1 : c2;
            gfx_fill_rect(tx * size, ty * size, size, size, col);
        }
    }
}
