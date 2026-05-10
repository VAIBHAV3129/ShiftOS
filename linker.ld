ENTRY(_start)

SECTIONS
{
    . = 0x100000;
    __kernel_start = .;

    .text : {
        __text_start = .;
        *(.text*)
        __text_end = .;
    }

    .rodata : {
        __rodata_start = .;
        *(.rodata*)
        __rodata_end = .;
    }

    .data : {
        __data_start = .;
        *(.data*)
        __data_end = .;
    }

    .bss : {
        __bss_start = .;
        *(COMMON)
        *(.bss*)
        __bss_end = .;
    }

    .limine_reqs : {
        KEEP(*(.limine_reqs))
    }

    __kernel_end = .;

    /DISCARD/ : {
        *(.note*)
        *(.comment)
    }
}
