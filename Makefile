ASM=nasm
CC=gcc
ASM_FLAGS=-f elf32
INCLUDE=$(SRC)/include
CFLAGS=-I $(INCLUDE) -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
ASM_LINK_FILE=link.ld
GRUB_BL=stage2_eltorito
SRC=src
BUILD=build
ISO=$(BUILD)/iso
BOOT=$(ISO)/boot
GRUB=$(BOOT)/grub
OBJECTS_C=$(BUILD)/stdio.o $(BUILD)/main.o $(BUILD)/memory.o $(BUILD)/loadidt.o $(BUILD)/loadgdt.o $(BUILD)/loadisr.o $(BUILD)/io_util.o $(BUILD)/irq.o $(BUILD)/pic.o $(BUILD)/intr_num.o $(BUILD)/keyboard_handler.o
OBJECTS_ASM=$(BUILD)/kernel.o $(BUILD)/header.o $(BUILD)/io.o $(BUILD)/idt.o $(BUILD)/gdt.o $(BUILD)/isr.o 

all: always $(BUILD)/os.iso 

run: $(BUILD)/os.iso
	bochs -f bochsrc.txt -q

$(BUILD)/%.o: $(SRC)/asm/%.asm
	$(ASM) $(ASM_FLAGS) $< -o $@

$(BUILD)/%.o: $(SRC)/c/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BOOT)/kernel.elf: $(OBJECTS_C) $(OBJECTS_ASM)
	ld -T $(SRC)/$(ASM_LINK_FILE) -melf_i386 $(OBJECTS_C) $(OBJECTS_ASM) -o $@

$(BUILD)/os.iso: $(BOOT)/kernel.elf 
	genisoimage -R -b boot/grub/$(GRUB_BL) -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o $@ $(ISO)

always:
	mkdir -p $(BUILD)

clean:
	rm -f $(BUILD)/*.o
	rm -f $(BOOT)/kernel.elf
	rm -f $(BUILD)/os.iso
