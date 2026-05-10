#ifndef SHIFTOS_PANIC_H
#define SHIFTOS_PANIC_H

#include "compiler.h"

SHIFTOS_NORETURN SHIFTOS_CALL
void kpanic(const char *msg);

#endif
