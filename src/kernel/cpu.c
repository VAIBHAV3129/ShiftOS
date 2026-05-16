#include "cpu.h"

extern void irq0_handler_stub(void);

static struct idt_entry g_idt[256];
static struct idt_ptr g_idt_ptr;

SHIFTOS_CALL void idt_set_gate(u8 vector, u64 handler_address, u16 selector, u8 attributes) {
    struct idt_entry *entry = &g_idt[vector];
    entry->offset_low = (u16)(handler_address & 0xFFFF);
    entry->selector = selector;
    entry->ist = 0;
    entry->attributes = attributes;
    entry->offset_mid = (u16)((handler_address >> 16) & 0xFFFF);
    entry->offset_high = (u32)((handler_address >> 32) & 0xFFFFFFFF);
    entry->reserved = 0;
}

SHIFTOS_CALL void idt_init(void) {
    for (u64 i = 0; i < 256; ++i) {
        g_idt[i].offset_low = 0;
        g_idt[i].selector = 0;
        g_idt[i].ist = 0;
        g_idt[i].attributes = 0;
        g_idt[i].offset_mid = 0;
        g_idt[i].offset_high = 0;
        g_idt[i].reserved = 0;
    }

    idt_set_gate(32, (u64)irq0_handler_stub, 0x08, 0x8E);

    g_idt_ptr.limit = (u16)(sizeof(g_idt) - 1);
    g_idt_ptr.base = (u64)&g_idt;

    __asm__ volatile("lidt %0" : : "m"(g_idt_ptr));
}
