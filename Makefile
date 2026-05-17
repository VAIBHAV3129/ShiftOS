ARCH := x86_64
CC ?= $(shell if command -v x86_64-elf-gcc >/dev/null 2>&1; then echo x86_64-elf-gcc; else echo gcc; fi)
LD ?= $(shell if command -v x86_64-elf-ld >/dev/null 2>&1; then echo x86_64-elf-ld; else echo ld; fi)
NASM ?= $(shell command -v nasm)
QEMU := qemu-system-x86_64

CFLAGS := -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -mno-red-zone -m64 -nostdlib -Wall -Wextra -O0 -g -mcmodel=kernel
LDFLAGS := -nostdlib -T linker.ld -z max-page-size=0x1000 -z noexecstack --no-warn-rwx-segments

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BOOT_DIR := src/boot
LIMINE_DIR := limine

KERNEL_ELF := $(BUILD_DIR)/shiftos.elf
KERNEL_MAP := $(BUILD_DIR)/shiftos.map
LIMINE_CFG := $(BOOT_DIR)/limine.conf

ISO_DIR := $(BUILD_DIR)/iso
ISO_BOOT_DIR := $(ISO_DIR)/boot
ISO_LIMINE_DIR := $(ISO_DIR)/limine
ISO_IMAGE := $(BUILD_DIR)/shiftos.iso

C_SOURCES := src/kernel/main.c \
             src/kernel/panic.c \
             src/kernel/limine.c \
             src/kernel/bootinfo.c \
             src/kernel/graphics.c \
             src/kernel/color.c \
             src/kernel/font.c \
             src/kernel/time.c \
             src/kernel/cpu.c

ASM_SOURCES := src/arch/x86_64/boot.asm \
               src/arch/x86_64/interrupt.asm

C_OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
ASM_OBJS := $(patsubst src/%.asm,$(OBJ_DIR)/%.o,$(ASM_SOURCES))

all: $(KERNEL_ELF)

$(KERNEL_ELF): $(C_OBJS) $(ASM_OBJS) linker.ld
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -Map $(KERNEL_MAP) -o $@ $(ASM_OBJS) $(C_OBJS)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(OBJ_DIR)/%.o: src/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

iso: $(KERNEL_ELF) $(LIMINE_CFG)
	@mkdir -p $(ISO_BOOT_DIR)
	@mkdir -p $(ISO_LIMINE_DIR)

	cp $(KERNEL_ELF) $(ISO_BOOT_DIR)/shiftos.elf

	# copy config to all valid locations
	cp $(LIMINE_CFG) $(ISO_DIR)/limine.conf
	cp $(LIMINE_CFG) $(ISO_BOOT_DIR)/limine.conf
	cp $(LIMINE_CFG) $(ISO_LIMINE_DIR)/limine.conf

	cp $(LIMINE_DIR)/limine-bios-cd.bin $(ISO_LIMINE_DIR)/
	cp $(LIMINE_DIR)/limine-bios.sys $(ISO_LIMINE_DIR)/
	cp $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_LIMINE_DIR)/

iso-image: iso
	xorriso -as mkisofs -R -J \
		-b limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine/limine-uefi-cd.bin -efi-boot-part \
		--efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(ISO_IMAGE)

run: iso-image
	$(QEMU) -cdrom $(ISO_IMAGE) -m 512M -serial stdio

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean iso iso-image run
