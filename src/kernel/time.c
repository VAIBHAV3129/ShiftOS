#include "time.h"
#include "cpu.h"

static volatile u64 g_ticks = 0;

static inline void outb(u16 port, u8 val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

SHIFTOS_CALL void pit_init(u32 frequency) {
    if (frequency == 0) {
        return;
    }

    u32 divisor = 1193182u / frequency;
    if (divisor == 0) {
        divisor = 1;
    } else if (divisor > 0xFFFFu) {
        divisor = 0xFFFFu;
    }

    outb(0x43, 0x36);
    outb(0x40, (u8)(divisor & 0xFF));
    outb(0x40, (u8)((divisor >> 8) & 0xFF));
}

SHIFTOS_CALL void pit_handler_callback(void) {
    g_ticks += 1;
}

SHIFTOS_CALL void time_init(void) {
    g_ticks = 0;
    cpu_cli();
    cpu_pic_remap();
    idt_init();
    pit_init(100);
    cpu_sti();
}

SHIFTOS_CALL u64 time_ticks(void) {
    return g_ticks;
}

SHIFTOS_CALL void time_wait(u64 ticks) {
    u64 start = g_ticks;
    while ((g_ticks - start) < ticks) {
        __asm__ volatile("pause");
    }
}
