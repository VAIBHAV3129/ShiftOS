#include "time.h"

static volatile u64 g_ticks = 0;

SHIFTOS_CALL void time_init(void) {
    g_ticks = 0;
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
