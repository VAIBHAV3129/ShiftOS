ARCH := x86_64
CC := x86_64-elf-gcc
LD := x86_64-elf-ld
NASM := nasm

CFLAGS := -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -mno-red-zone -m64 -nostdlib -Wall -Wextra -O2 -g
LDFLAGS := -nostdlib -T linker.ld -z max-page-size=0x1000

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BOOT_DIR := src/boot
LIMINE_DIR := limine

KERNEL_ELF := $(BUILD_DIR)/shiftos.elf
KERNEL_MAP := $(BUILD_DIR)/shiftos.map
LIMINE_CFG := $(BOOT_DIR)/limine.cfg

C_SOURCES := src/kernel/main.c \
             src/kernel/panic.c \
             src/kernel/limine.c \
             src/kernel/bootinfo.c \
             src/kernel/graphics.c \
             src/kernel/color.c

ASM_SOURCES := src/arch/x86_64/boot.asm

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

iso: $(KERNEL_ELF)
	@mkdir -p $(BUILD_DIR)/iso/boot
	cp $(KERNEL_ELF) $(BUILD_DIR)/iso/boot/shiftos.elf
	cp $(LIMINE_CFG) $(BUILD_DIR)/iso/limine.cfg
	@echo "ISO staging complete. Add Limine binaries in ./limine before making a bootable ISO."

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean iso
