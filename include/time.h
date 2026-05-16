#ifndef SHIFTOS_TIME_H
#define SHIFTOS_TIME_H

#include "types.h"
#include "compiler.h"

SHIFTOS_CALL void time_init(void);
SHIFTOS_CALL u64 time_ticks(void);
SHIFTOS_CALL void time_wait(u64 ticks);
SHIFTOS_CALL void pit_init(u32 frequency);
SHIFTOS_CALL void pit_handler_callback(void);

#endif
