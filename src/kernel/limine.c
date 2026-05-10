#include "limine.h"
#include "compiler.h"

SHIFTOS_USED SHIFTOS_SECTION(".limine_reqs")
volatile struct limine_base_revision_request limine_base_revision_request = {
    .id = {0, 0},
    .revision = 0,
    .response = 0
};

SHIFTOS_USED SHIFTOS_SECTION(".limine_reqs")
volatile struct limine_framebuffer_request limine_framebuffer_request = {
    .id = {0, 0},
    .revision = 0,
    .response = 0
};
