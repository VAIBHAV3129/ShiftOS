#include "types.h"
#include "compiler.h"
#include "boot.h"
#include "panic.h"
#include "limine.h"
#include "bootinfo.h"
#include "graphics.h"
#include "color.h"

SHIFTOS_NORETURN SHIFTOS_CALL
void kmain(void) {
    (void)&__kernel_start;
    (void)&__kernel_end;
    (void)&__text_start;
    (void)&__text_end;
    (void)&__rodata_start;
    (void)&__rodata_end;
    (void)&__data_start;
    (void)&__data_end;
    (void)&__bss_start;
    (void)&__bss_end;

    if (!bootinfo_init()) {
        kpanic("no framebuffer");
    }

    if (!gfx_init()) {
        kpanic("gfx init failed");
    }

    gfx_draw_checker(64, color_rgb(14, 20, 30), color_rgb(20, 28, 40));
    gfx_draw_gradient(0, 0, 640, 240, color_rgb(24, 90, 180), color_rgb(8, 24, 64));

    gfx_fill_rect(40, 40, 240, 120, color_rgb(255, 122, 24));
    gfx_fill_rect(320, 80, 200, 160, color_rgb(59, 167, 255));
    gfx_fill_rect(600, 140, 280, 180, color_rgb(149, 59, 255));

    gfx_draw_rect(30, 30, 260, 140, color_rgb(255, 255, 255));
    gfx_draw_rect(310, 70, 220, 180, color_rgb(255, 255, 255));
    gfx_draw_rect(590, 130, 300, 200, color_rgb(255, 255, 255));

    gfx_draw_line(0, 0, 400, 300, color_rgb(255, 215, 0));
    gfx_draw_line(0, 300, 400, 0, color_rgb(255, 215, 0));

    gfx_fill_rect_alpha(100, 260, 360, 120, color_rgba(0, 0, 0, 96));

    kpanic("kmain idle");
}
