#include "types.h"

__attribute__((sysv_abi))
void kmain(void) {
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
