#ifndef SHIFTOS_CPU_H
#define SHIFTOS_CPU_H

#include "types.h"
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

struct idt_entry {
    u16 offset_low;
    u16 selector;
    u8 ist;
    u8 attributes;
    u16 offset_mid;
    u32 offset_high;
    u32 reserved;
} __attribute__((packed));

struct idt_ptr {
    u16 limit;
    u64 base;
} __attribute__((packed));

SHIFTOS_CALL void idt_set_gate(u8 vector, u64 handler_address, u16 selector, u8 attributes);
SHIFTOS_CALL void idt_init(void);
SHIFTOS_CALL void cpu_pic_remap(void);

#endif
