#include "types.h"
#include "compiler.h"
#include "boot.h"
#include "panic.h"
#include "limine.h"
#include "bootinfo.h"
#include "graphics.h"

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

    gfx_clear(0x00102030);
    gfx_fill_rect(40, 40, 240, 120, 0x00FF7A18);
    gfx_fill_rect(320, 80, 200, 160, 0x003BA7FF);
    gfx_fill_rect(600, 140, 280, 180, 0x00953BFF);

    gfx_draw_rect(30, 30, 260, 140, 0x00FFFFFF);
    gfx_draw_rect(310, 70, 220, 180, 0x00FFFFFF);
    gfx_draw_rect(590, 130, 300, 200, 0x00FFFFFF);

    gfx_draw_line(0, 0, 400, 300, 0x00FFD700);
    gfx_draw_line(0, 300, 400, 0, 0x00FFD700);

    kpanic("kmain idle");
}
