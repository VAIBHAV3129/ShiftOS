#ifndef SHIFTOS_BOOT_H
#define SHIFTOS_BOOT_H
#include "types.h"
extern u8 __kernel_start;
extern u8 __kernel_end;
extern u8 __text_start;
extern u8 __text_end;
extern u8 __rodata_start;
extern u8 __rodata_end;
extern u8 __data_start;
extern u8 __data_end;
extern u8 __bss_start;
extern u8 __bss_end;
#endif
