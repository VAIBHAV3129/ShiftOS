#ifndef SHIFTOS_LIMINE_H
#define SHIFTOS_LIMINE_H

#include "types.h"

struct limine_uuid {
    u64 a;
    u64 b;
};

struct limine_base_revision_response {
    u64 revision;
};

struct limine_base_revision_request {
    struct limine_uuid id;
    u64 revision;
    struct limine_base_revision_response *response;
};

struct limine_framebuffer {
    u64 address;
    u64 width;
    u64 height;
    u64 pitch;
    u16 bpp;
    u8  memory_model;
    u8  red_mask_size;
    u8  red_mask_shift;
    u8  green_mask_size;
    u8  green_mask_shift;
    u8  blue_mask_size;
    u8  blue_mask_shift;
    u8  unused[7];
    u64 edid_size;
    void *edid;
};

struct limine_framebuffer_response {
    u64 framebuffer_count;
    struct limine_framebuffer **framebuffers;
};

struct limine_framebuffer_request {
    struct limine_uuid id;
    u64 revision;
    struct limine_framebuffer_response *response;
};

extern volatile struct limine_base_revision_request limine_base_revision_request;
extern volatile struct limine_framebuffer_request limine_framebuffer_request;

#endif
