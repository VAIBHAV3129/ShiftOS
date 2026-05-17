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
    if (g_gfx.stride == 0) {
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
    for (u64 y = 0; y < g_gfx.height; ++y) {
        u64 row = y * g_gfx.stride;
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

    for (u64 yy = y; yy < max_y; ++yy) {
        u64 row = yy * g_gfx.stride;
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

    for (u64 yy = y; yy < max_y; ++yy) {
        u64 row = yy * g_gfx.stride;
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

    if (h == 1) {
        gfx_fill_rect(x, y, w, 1, top);
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

static void gfx_hline(u64 x, u64 y, u64 w, u32 color) {
    if (w == 0) {
        return;
    }
    gfx_fill_rect(x, y, w, 1, color);
}

static void gfx_circle_points(u64 cx, u64 cy, s64 x, s64 y, u32 color) {
    if (x < 0 || y < 0) {
        return;
    }

    gfx_put_pixel(cx + (u64)x, cy + (u64)y, color);
    gfx_put_pixel(cx + (u64)y, cy + (u64)x, color);
    if ((s64)cx - x >= 0) gfx_put_pixel(cx - (u64)x, cy + (u64)y, color);
    if ((s64)cx - y >= 0) gfx_put_pixel(cx - (u64)y, cy + (u64)x, color);
    if ((s64)cy - y >= 0) gfx_put_pixel(cx + (u64)x, cy - (u64)y, color);
    if ((s64)cy - x >= 0) gfx_put_pixel(cx + (u64)y, cy - (u64)x, color);
    if ((s64)cx - x >= 0 && (s64)cy - y >= 0) gfx_put_pixel(cx - (u64)x, cy - (u64)y, color);
    if ((s64)cx - y >= 0 && (s64)cy - x >= 0) gfx_put_pixel(cx - (u64)y, cy - (u64)x, color);
}

SHIFTOS_CALL void gfx_draw_circle(u64 cx, u64 cy, u64 r, u32 color) {
    if (r == 0) {
        gfx_put_pixel(cx, cy, color);
        return;
    }

    s64 x = (s64)r;
    s64 y = 0;
    s64 err = 0;

    while (x >= y) {
        gfx_circle_points(cx, cy, x, y, color);
        y++;
        err += 2 * y + 1;
        if (err > 0) {
            x--;
            err -= 2 * x + 1;
        }
    }
}

SHIFTOS_CALL void gfx_fill_circle(u64 cx, u64 cy, u64 r, u32 color) {
    if (r == 0) {
        gfx_put_pixel(cx, cy, color);
        return;
    }

    s64 x = (s64)r;
    s64 y = 0;
    s64 err = 0;

    while (x >= y) {
        if ((s64)cy + y >= 0) gfx_hline(cx - (u64)x, cy + (u64)y, (u64)(x * 2 + 1), color);
        if ((s64)cy + x >= 0) gfx_hline(cx - (u64)y, cy + (u64)x, (u64)(y * 2 + 1), color);
        if ((s64)cy - y >= 0) gfx_hline(cx - (u64)x, cy - (u64)y, (u64)(x * 2 + 1), color);
        if ((s64)cy - x >= 0) gfx_hline(cx - (u64)y, cy - (u64)x, (u64)(y * 2 + 1), color);

        y++;
        err += 2 * y + 1;
        if (err > 0) {
            x--;
            err -= 2 * x + 1;
        }
    }
}

static void gfx_draw_char_at(u64 x, u64 y, char c, u32 color, int alpha) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }
    if (x + 7 >= g_gfx.width || y + 7 >= g_gfx.height) {
        return;
    }

    const u8 *glyph = font_get_glyph(c);
    if (!glyph) {
        return;
    }

    for (u64 row = 0; row < 8; ++row) {
        u8 bits = glyph[row];
        for (u64 col = 0; col < 8; ++col) {
            if (bits & (1u << (7 - col))) {
                if (alpha) {
                    gfx_put_pixel_alpha(x + col, y + row, color);
                } else {
                    gfx_put_pixel(x + col, y + row, color);
                }
            }
        }
    }
}

SHIFTOS_CALL void gfx_draw_text(u64 x, u64 y, const char *text, u32 color) {
    if (!text || x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    u64 cx = x;
    for (const char *p = text; *p; ++p) {
        if (cx + 7 >= g_gfx.width || y + 7 >= g_gfx.height) {
            break;
        }
        gfx_draw_char_at(cx, y, *p, color, 0);
        cx += 9;
    }
}

SHIFTOS_CALL void gfx_draw_text_glow(u64 x, u64 y, const char *text, u32 color, u32 glow) {
    if (!text || x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    const s64 offsets[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (u64 i = 0; i < 8; ++i) {
        s64 ox = offsets[i][0];
        s64 oy = offsets[i][1];
        if ((s64)x + ox < 0 || (s64)y + oy < 0) {
            continue;
        }

        u64 cx = x;
        for (const char *p = text; *p; ++p) {
            u64 px = cx + (u64)ox;
            u64 py = y + (u64)oy;
            if (px + 7 >= g_gfx.width || py + 7 >= g_gfx.height) {
                break;
            }
            gfx_draw_char_at(px, py, *p, glow, 1);
            cx += 9;
        }
    }

    gfx_draw_text(x, y, text, color);
}

SHIFTOS_CALL void gfx_measure_text(const char *text, u64 *out_w, u64 *out_h) {
    if (out_w) {
        *out_w = 0;
    }
    if (out_h) {
        *out_h = 8;
    }

    if (!text) {
        return;
    }

    u64 len = 0;
    for (const char *p = text; *p; ++p) {
        len++;
    }

    if (out_w) {
        *out_w = len ? (len * 9 - 1) : 0;
    }
}

SHIFTOS_CALL void gfx_draw_window(u64 x, u64 y, u64 w, u64 h, const char *title, u32 accent, u32 body) {
    if (w < 32 || h < 32) {
        return;
    }

    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    u64 title_h = 20;

    gfx_fill_rect(x, y, w, h, body);
    gfx_fill_rect(x, y, w, title_h, accent);
    gfx_draw_rect(x, y, w, h, color_rgb(0, 180, 200));

    if (title) {
        u64 text_w = 0;
        u64 text_h = 0;
        gfx_measure_text(title, &text_w, &text_h);

        u64 tx = x + 8;
        if (tx + text_w >= g_gfx.width && text_w < g_gfx.width) {
            tx = g_gfx.width - text_w - 1;
        }

        u64 ty = y + (title_h - text_h) / 2;
        if (ty + 7 >= g_gfx.height) {
            return;
        }

        gfx_draw_text(tx, ty, title, color_rgb(230, 255, 255));
    }
}

SHIFTOS_CALL void gfx_draw_textf(u64 x, u64 y, u32 color, const char *label, u64 value) {
    if (x >= g_gfx.width || y >= g_gfx.height) {
        return;
    }

    char buf[32];
    u64 idx = 0;

    if (label) {
        for (const char *p = label; *p && idx < sizeof(buf) - 1; ++p) {
            buf[idx++] = *p;
        }
    }

    const char digits[] = "0123456789";
    char num[20];
    u64 n = value;

    if (n == 0) {
        num[0] = '0';
        num[1] = '\0';
    } else {
        u64 len = 0;
        while (n > 0 && len < sizeof(num) - 1) {
            num[len++] = digits[n % 10];
            n /= 10;
        }
        num[len] = '\0';

        for (u64 i = 0; i < len / 2; ++i) {
            char tmp = num[i];
            num[i] = num[len - 1 - i];
            num[len - 1 - i] = tmp;
        }
    }

    for (const char *p = num; *p && idx < sizeof(buf) - 1; ++p) {
        buf[idx++] = *p;
    }

    buf[idx] = '\0';

    gfx_draw_text(x, y, buf, color);
}
