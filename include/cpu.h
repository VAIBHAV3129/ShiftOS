#ifndef SHIFTOS_CPU_H
#define SHIFTOS_CPU_H

#include "compiler.h"

SHIFTOS_INLINE void cpu_hlt(void) {
    __asm__ __volatile__("hlt");
}

SHIFTOS_INLINE void cpu_cli(void) {
    __asm__ __volatile__("cli");
}

SHIFTOS_INLINE void cpu_sti(void) {
    __asm__ __volatile__("sti");
}

#endif
