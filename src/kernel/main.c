#include "types.h"
#include "compiler.h"

SHIFTOS_NORETURN SHIFTOS_CALL
void kmain(void) {
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
