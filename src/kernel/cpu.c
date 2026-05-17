#include "cpu.h"

extern void irq0_handler_stub(void);

static struct idt_entry g_idt[256];
static struct idt_ptr g_idt_ptr;

static inline void outb(u16 port, u8 val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    __asm__ volatile("outb %b0, $0x80" : : "a"(0));
}

SHIFTOS_CALL void cpu_pic_remap(void) {
    u8 a1 = inb(0x21);
    u8 a2 = inb(0xA1);

    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();
    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, (u8)(a1 & 0xFE));
    outb(0xA1, a2);
}

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
