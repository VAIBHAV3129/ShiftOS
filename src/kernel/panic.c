#include "panic.h"
#include "cpu.h"

SHIFTOS_NORETURN SHIFTOS_CALL
void kpanic(const char *msg) {
    (void)msg;
    cpu_cli();

    for (;;) {
        cpu_hlt();
    }
}
