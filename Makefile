ARCH := x86_64
CC := x86_64-elf-gcc
LD := x86_64-elf-ld
NASM := nasm

CFLAGS := -ffreestanding -fno-stack-protector -mno-red-zone -m64 -nostdlib -Wall -Wextra
LDFLAGS := -nostdlib

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

KERNEL_ELF := $(BUILD_DIR)/shiftos.elf

C_SOURCES := src/kernel/main.c
ASM_SOURCES := src/arch/x86_64/boot.asm

C_OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
ASM_OBJS := $(patsubst src/%.asm,$(OBJ_DIR)/%.o,$(ASM_SOURCES))

all: $(KERNEL_ELF)

$(KERNEL_ELF): $(C_OBJS) $(ASM_OBJS) linker.ld
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(ASM_OBJS) $(C_OBJS)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(OBJ_DIR)/%.o: src/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
