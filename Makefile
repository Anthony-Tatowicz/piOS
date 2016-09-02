# Use arm gcc
CC=arm-none-eabi-gcc
# Flags for compiler
CFLAGS= -mcpu=arm1176jzf-s -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra
# Assembly flags
AS=arm-none-eabi-gcc
# Assembly flags
ASFLAGS=-fpic -ffreestanding
# Custom linker
LD=src/linker.ld
# linker flags
LDFLAGS= -ffreestanding -O2 -nostdlib


all: pios

pios: boot.o kernel.o
	$(CC) -T $(LD) -o build/pios.elf $(LDFLAGS) $?
	arm-none-eabi-objcopy pios.elf -O build/binary pios.bin

boot.o: boot.S
 	$(AS) $(ASFLAGS) $? -o $@

kernel.o: kernel.c kernel.h
	$(CC) $(CFLSGS) $? -o $@

shell.o: shell.c shell.h
	${CC} ${CSFLAGS} $? -o $@

clean:
	@rm build/*.o
