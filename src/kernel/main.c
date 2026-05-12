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

    kpanic("kmain idle");
}
