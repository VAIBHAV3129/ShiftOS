ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 0xffffffff80000000;
    
    __kernel_start = .;

    . = ALIGN(0x1000);
    .text : {
        __text_start = .;
        *(.text*)
        __text_end = .;
    } :text

    . = ALIGN(0x1000);
    .rodata : {
        __rodata_start = .;
        *(.rodata*)
        __rodata_end = .;
    } :rodata

    . = ALIGN(0x1000);
    .data : {
        __data_start = .;
        *(.data*)
        __data_end = .;
    } :data

    .limine_reqs : {
        KEEP(*(.limine_reqs))
    } :data

    .bss : {
        __bss_start = .;
        *(COMMON)
        *(.bss*)
        __bss_end = .;
    } :data

    . = ALIGN(0x1000);
    __kernel_end = .;

    /DISCARD/ : {
        *(.note*)
        *(.comment)
    }
}
