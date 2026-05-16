bits 64
global irq0_handler_stub
extern pit_handler_callback

irq0_handler_stub:
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11

    call pit_handler_callback

    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax

    mov al, 0x20
    out 0x20, al

    iretq
