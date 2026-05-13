#include "color.h"

SHIFTOS_CALL u32 color_rgb(u8 r, u8 g, u8 b) {
    return 0xFF000000u | ((u32)r << 16) | ((u32)g << 8) | (u32)b;
}

SHIFTOS_CALL u32 color_rgba(u8 r, u8 g, u8 b, u8 a) {
    return ((u32)a << 24) | ((u32)r << 16) | ((u32)g << 8) | (u32)b;
}

SHIFTOS_CALL u32 color_blend(u32 src, u32 dst) {
    u32 sa = (src >> 24) & 0xFFu;
    u32 sr = (src >> 16) & 0xFFu;
    u32 sg = (src >> 8) & 0xFFu;
    u32 sb = src & 0xFFu;

    u32 dr = (dst >> 16) & 0xFFu;
    u32 dg = (dst >> 8) & 0xFFu;
    u32 db = dst & 0xFFu;

    u32 inva = 255u - sa;

    u32 r = (sr * sa + dr * inva) / 255u;
    u32 g = (sg * sa + dg * inva) / 255u;
    u32 b = (sb * sa + db * inva) / 255u;

    return 0xFF000000u | (r << 16) | (g << 8) | b;
}
