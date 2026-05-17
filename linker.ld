ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);    /* Read and Execute */
    rodata PT_LOAD FLAGS(4);  /* Read Only */
    data PT_LOAD FLAGS(6);    /* Read and Write */
}

SECTIONS
{
    . = 0xffffffff80000000;
    
    __kernel_start = .;

    .text : {
        __text_start = .;
        *(.text*)
        __text_end = .;
    } :text

    .rodata : {
        __rodata_start = .;
        *(.rodata*)
        __rodata_end = .;
    } :rodata

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

    __kernel_end = .;

    /DISCARD/ : {
        *(.note*)
        *(.comment)
    }
}
