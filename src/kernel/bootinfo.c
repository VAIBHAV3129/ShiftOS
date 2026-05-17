#include "bootinfo.h"
#include "limine.h"

extern volatile struct limine_framebuffer_request limine_framebuffer_request;

static struct boot_info g_boot_info;

SHIFTOS_CALL int bootinfo_init(void) {
    g_boot_info.has_framebuffer = 0;

    if (!limine_framebuffer_request.response) {
        return 0;
    }

    if (limine_framebuffer_request.response->framebuffer_count == 0) {
        return 0;
    }

    if (!limine_framebuffer_request.response->framebuffers) {
        return 0;
    }

    struct limine_framebuffer *fb = limine_framebuffer_request.response->framebuffers[0];
    if (!fb) {
        return 0;
    }

    g_boot_info.has_framebuffer = 1;
    g_boot_info.fb.address = fb->address;
    g_boot_info.fb.width = fb->width;
    g_boot_info.fb.height = fb->height;
    g_boot_info.fb.pitch = fb->pitch;
    g_boot_info.fb.bpp = fb->bpp;
    g_boot_info.fb.memory_model = fb->memory_model;
    g_boot_info.fb.red_mask_size = fb->red_mask_size;
    g_boot_info.fb.red_mask_shift = fb->red_mask_shift;
    g_boot_info.fb.green_mask_size = fb->green_mask_size;
    g_boot_info.fb.green_mask_shift = fb->green_mask_shift;
    g_boot_info.fb.blue_mask_size = fb->blue_mask_size;
    g_boot_info.fb.blue_mask_shift = fb->blue_mask_shift;

    return 1;
}

SHIFTOS_CALL const struct boot_info *bootinfo_get(void) {
    return &g_boot_info;
}
