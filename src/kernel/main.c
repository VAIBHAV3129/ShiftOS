#include "types.h"
#include "compiler.h"
#include "boot.h"
#include "panic.h"
#include "limine.h"
#include "bootinfo.h"
#include "graphics.h"
#include "color.h"
#include "time.h"

extern u8 __kernel_start;
extern u8 __kernel_end;
extern u8 __text_start;
extern u8 __text_end;
extern u8 __rodata_start;
extern u8 __rodata_end;
extern u8 __data_start;
extern u8 __data_end;
extern u8 __bss_start;
extern u8 __bss_end;

static void render_scene(u64 progress, u8 pulse, u64 ticks) {
    u64 w = gfx_width();
    u64 h = gfx_height();

    gfx_draw_checker(64, color_rgb(10, 16, 26), color_rgb(16, 24, 34));
    gfx_draw_gradient(0, 0, w, h / 2, color_rgb(10, 50, 90), color_rgb(6, 18, 40));

    const char *title = "ShiftOS";
    u64 title_w = 0;
    u64 title_h = 0;
    gfx_measure_text(title, &title_w, &title_h);

    u64 title_x = (w > title_w) ? (w - title_w) / 2 : 0;
    u64 title_y = h / 5;

    u8 glow_a = 40 + (pulse * 160) / 255;
    u32 glow = color_rgba(0, 255, 255, glow_a);
    u32 text = color_rgb(0, 220, 220);

    gfx_draw_text_glow(title_x, title_y, title, text, glow);

    u64 bar_w = (w > 400) ? 400 : (w - 40);
    u64 bar_h = 16;
    u64 bar_x = (w - bar_w) / 2;
    u64 bar_y = title_y + title_h + 20;

    gfx_fill_rect(bar_x, bar_y, bar_w, bar_h, color_rgb(18, 28, 40));
    gfx_draw_rect(bar_x, bar_y, bar_w, bar_h, color_rgb(0, 180, 200));

    u64 fill_w = (bar_w - 4) * progress / 100;
    gfx_fill_rect(bar_x + 2, bar_y + 2, fill_w, bar_h - 4, color_rgb(0, 220, 220));
    gfx_fill_rect_alpha(bar_x + 2, bar_y + 2, fill_w, bar_h / 2, color_rgba(255, 255, 255, 40));

    u64 win_y = bar_y + 40;
    gfx_draw_window(80, win_y, 360, 200, "Welcome", color_rgb(0, 110, 140), color_rgb(20, 32, 48));
    gfx_draw_window(500, win_y + 40, 320, 180, "System", color_rgb(0, 140, 160), color_rgb(18, 26, 40));

    if (h > 20) {
        gfx_draw_textf(24, h - 20, color_rgb(200, 230, 255), "ticks: ", ticks);
    }
}

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

    time_init();

    u8 pulse = 0;
    int dir = 1;

    for (;;) {
        for (u64 progress = 0; progress <= 100; ++progress) {
            render_scene(progress, pulse, time_ticks());

            if (dir > 0) {
                if (pulse >= 250) {
                    dir = -1;
                } else {
                    pulse += 5;
                }
            } else {
                if (pulse <= 5) {
                    dir = 1;
                } else {
                    pulse -= 5;
                }
            }

            time_wait(2);
        }
    }
}
