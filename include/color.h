#ifndef SHIFTOS_COLOR_H
#define SHIFTOS_COLOR_H
#include "types.h"
#include "compiler.h"
SHIFTOS_CALL u32 color_rgb(u8 r, u8 g, u8 b);
SHIFTOS_CALL u32 color_rgba(u8 r, u8 g, u8 b, u8 a);
SHIFTOS_CALL u32 color_blend(u32 src, u32 dst);
#endif
