#include "limine.h"
#include "compiler.h"

SHIFTOS_USED SHIFTOS_SECTION(".limine_reqs")
volatile struct limine_base_revision_request limine_base_revision_request = {
    .id = {
        .a = 0xf759957357787220,
        .b = 0x151486b70936130d
    },
    .revision = 0,
    .response = 0
};

SHIFTOS_USED SHIFTOS_SECTION(".limine_reqs")
volatile struct limine_framebuffer_request limine_framebuffer_request = {
    .id = {
        .a = 0x97127d0ddde8a74b,
        .b = 0x6eac34d561d34eb4
    },
    .revision = 0,
    .response = 0
};
