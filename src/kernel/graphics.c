#include "graphics.h"
#include "bootinfo.h"

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

SHIFTOS_CALL void gfx_put_pixel(u64 x, u64 y, u32 color) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    u64 pixels_per_row = g_gfx.pitch / 4;
    g_gfx.buffer[y * pixels_per_row + x] = color;
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
