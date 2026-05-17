 #include "graphics.h"
#include "bootinfo.h"
#include "color.h"
#include "font.h"

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
    g_gfx.stride = 0;

    if (!g_gfx.buffer || g_gfx.width == 0 || g_gfx.height == 0) {
        return 0;
    }

   
    if (g_gfx.pitch == 0 || g_gfx.bpp != 32) {
        return 0;
    }

 
    if (g_gfx.pitch < g_gfx.width * 4) {
        return 0;
    }

    g_gfx.stride = g_gfx.pitch / 4;
    return 1;
}

SHIFTOS_CALL u64 gfx_width(void)  { return g_gfx.width; }
SHIFTOS_CALL u64 gfx_height(void) { return g_gfx.height; }

SHIFTOS_CALL void gfx_put_pixel(u64 x, u64 y, u32 color) {
    
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }
    g_gfx.buffer[y * g_gfx.stride + x] = color;
}

SHIFTOS_CALL void gfx_put_pixel_alpha(u64 x, u64 y, u32 color) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }
    u64 idx = y * g_gfx.stride + x;
    u32 dst = g_gfx.buffer[idx];
    g_gfx.buffer[idx] = color_blend(color, dst);
}

SHIFTOS_CALL void gfx_clear(u32 color) {
    if (!g_gfx.buffer) return;
    for (u64 y = 0; y < g_gfx.height; ++y) {
        u64 row = y * g_gfx.stride;
        for (u64 x = 0; x < g_gfx.width; ++x) {
            g_gfx.buffer[row + x] = color;
        }
    }
}

SHIFTOS_CALL void gfx_fill_rect(u64 x, u64 y, u64 w, u64 h, u32 color) {
    if (!g_gfx.buffer || x >= g_gfx.width || y >= g_gfx.height) return;
    
    
    u64 max_x = (x + w > g_gfx.width) ? g_gfx.width : x + w;
    u64 max_y = (y + h > g_gfx.height) ? g_gfx.height : y + h;

    for (u64 yy = y; yy < max_y; ++yy) {
        u64 row = yy * g_gfx.stride;
        for (u64 xx = x; xx < max_x; ++xx) {
            g_gfx.buffer[row + xx] = color;
        }
    }
}

SHIFTOS_CALL void gfx_fill_rect_alpha(u64 x, u64 y, u64 w, u64 h, u32 color) {
    if (!g_gfx.buffer || x >= g_gfx.width || y >= g_gfx.height) return;
    
    u64 max_x = (x + w > g_gfx.width) ? g_gfx.width : x + w;
    u64 max_y = (y + h > g_gfx.height) ? g_gfx.height : y + h;

    for (u64 yy = y; yy < max_y; ++yy) {
        u64 row = yy * g_gfx.stride;
        for (u64 xx = x; xx < max_x; ++xx) {
            u64 idx = row + xx;
            g_gfx.buffer[idx] = color_blend(color, g_gfx.buffer[idx]);
        }
    }
}

SHIFTOS_CALL void gfx_draw_rect(u64 x, u64 y, u64 w, u64 h, u32 color) {
    if (w == 0 || h == 0) return;
    gfx_fill_rect(x, y, w, 1, color);
    gfx_fill_rect(x, y + h - 1, w, 1, color);
    gfx_fill_rect(x, y, 1, h, color);
    gfx_fill_rect(x + w - 1, y, 1, h, color);
}

SHIFTOS_CALL void gfx_draw_gradient(u64 x, u64 y, u64 w, u64 h, u32 color_top, u32 color_bottom) {
    if (h == 0) return;
    u32 tr = (color_top >> 16) & 0xFF, tg = (color_top >> 8) & 0xFF, tb = color_top & 0xFF;
    u32 br = (color_bottom >> 16) & 0xFF, bg = (color_bottom >> 8) & 0xFF, bb = color_bottom & 0xFF;

    for (u64 yy = 0; yy < h; ++yy) {
        if ((y + yy) >= g_gfx.height) break;
        u32 r = tr + (u32)(((s32)br - (s32)tr) * (s64)yy / (s64)h);
        u32 g = tg + (u32)(((s32)bg - (s32)tg) * (s64)yy / (s64)h);
        u32 b = tb + (u32)(((s32)bb - (s32)tb) * (s64)yy / (s64)h);
        gfx_fill_rect(x, y + yy, w, 1, color_rgb(r, g, b));
    }
}

SHIFTOS_CALL void gfx_draw_checker(u64 size, u32 c1, u32 c2) {
    if (size == 0) return;
    for (u64 y = 0; y < g_gfx.height; y += size) {
        for (u64 x = 0; x < g_gfx.width; x += size) {
            u32 col = (((x / size) + (y / size)) & 1) ? c1 : c2;
            gfx_fill_rect(x, y, size, size, col);
        }
    }
}

static void gfx_draw_char_at(u64 x, u64 y, char c, u32 color, int use_alpha) {

    if (c < 32 || c > 126) return; 
    const u8 *glyph = font_get_glyph(c);
    if (!glyph) return;

    for (int row = 0; row < 8; ++row) {
        u8 b = glyph[row];
        for (int col = 0; col < 8; ++col) {
            if (b & (1 << (7 - col))) {
                if (use_alpha) {
                    gfx_put_pixel_alpha(x + col, y + row, color);
                } else {
                    gfx_put_pixel(x + col, y + row, color);
                }
            }
        }
    }
}

SHIFTOS_CALL void gfx_measure_text(const char *text, u64 *w, u64 *h) {
    if (!text) {
        if (w) *w = 0;
        if (h) *h = 0;
        return;
    }
    u64 len = 0;
    while (text[len]) len++;
    if (w) *w = len * 9; 
    if (h) *h = 8;
}

SHIFTOS_CALL void gfx_draw_text(u64 x, u64 y, const char *text, u32 color) {
    if (!text) return;
    for (const char *p = text; *p; ++p) {
        gfx_draw_char_at(x, y, *p, color, 0);
        x += 9;
    }
}

SHIFTOS_CALL void gfx_draw_text_glow(u64 x, u64 y, const char *text, u32 color, u32 glow_color) {
    if (!text) return;
    for (int oy = -1; oy <= 1; ++oy) {
        for (int ox = -1; ox <= 1; ++ox) {
            if (ox == 0 && oy == 0) continue;
            u64 cx = x + ox;
            for (const char *p = text; *p; ++p) {
                gfx_draw_char_at(cx, y + oy, *p, glow_color, 1);
                cx += 9;
            }
        }
    }
    gfx_draw_text(x, y, text, color);
}

SHIFTOS_CALL void gfx_draw_window(u64 x, u64 y, u64 w, u64 h, const char *title, u32 accent, u32 bg) {
    u64 title_h = 24;
    if (w < 16 || h < title_h) return;

    gfx_fill_rect(x, y, w, title_h, accent);
    gfx_fill_rect(x, y + title_h, w, h - title_h, bg);
    gfx_draw_rect(x, y, w, h, accent);

    if (title) {
        u64 text_w = 0, text_h = 0;
        gfx_measure_text(title, &text_w, &text_h);
        u64 tx = x + 8;
        u64 ty = y + (title_h - text_h) / 2;
        gfx_draw_text(tx, ty, title, color_rgb(255, 255, 255));
    }
}

SHIFTOS_CALL void gfx_draw_textf(u64 x, u64 y, u32 color, const char *label, u64 value) {
    char buf[64];
    u64 idx = 0;

    if (label) {
        for (const char *p = label; *p && idx < sizeof(buf) - 24; ++p) {
            buf[idx++] = *p;
        }
    }

    char digits[24];
    u64 d_idx = 0;
    u64 v = value;
    if (v == 0) {
        digits[d_idx++] = '0';
    } else {
        while (v > 0 && d_idx < sizeof(digits)) {
            digits[d_idx++] = '0' + (v % 10);
            v /= 10;
        }
    }
    while (d_idx > 0 && idx < sizeof(buf) - 1) {
        buf[idx++] = digits[--d_idx];
    }
    buf[idx] = '\0';

    gfx_draw_text(x, y, buf, color);
}
