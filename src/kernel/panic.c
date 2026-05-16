#include "panic.h"
#include "cpu.h"
#include "graphics.h"
#include "color.h"

SHIFTOS_NORETURN SHIFTOS_CALL
void kpanic(const char *msg) {
    u64 w = gfx_width();
    u64 h = gfx_height();

    gfx_clear(color_rgb(180, 0, 0));

    const char *title = "CRITICAL KERNEL PANIC";
    u64 title_w = 0;
    u64 title_h = 0;
    gfx_measure_text(title, &title_w, &title_h);

    u64 title_x = (w > title_w) ? (w - title_w) / 2 : 0;
    u64 title_y = (h > 32) ? (h / 2 - 24) : 0;
    gfx_draw_text(title_x, title_y, title, color_rgb(255, 255, 255));

    if (msg) {
        u64 msg_w = 0;
        u64 msg_h = 0;
        gfx_measure_text(msg, &msg_w, &msg_h);
        u64 msg_x = (w > msg_w) ? (w - msg_w) / 2 : 0;
        u64 msg_y = title_y + title_h + 12;
        gfx_draw_text(msg_x, msg_y, msg, color_rgb(255, 255, 255));
    }

    cpu_cli();
    for (;;) {
        cpu_hlt();
    }
}
