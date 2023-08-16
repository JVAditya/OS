ASM=nasm
CC=gcc
OBJ=objcopy
ASM_FLAGS=-f elf32
INCLUDE=-I $(SRC)/include/boot -I $(SRC)/include/drivers -I $(SRC)/include/font -I $(SRC)/include/io -I $(SRC)/include/memory -I $(SRC)/include/utils
CFLAGS=$(INCLUDE) -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
OBJ_FLAGS=-O elf32-i386 -B i386 -I binary
GNU_LINK_FILE=link.ld
GRUB_BL=stage2_eltorito
SRC=src
BUILD=build
UTILS=utilities
ISO=$(BUILD)/iso
BOOT=$(ISO)/boot
GRUB=$(BOOT)/grub
OBJECTS_C=$(BUILD)/stdio.o $(BUILD)/main.o $(BUILD)/memory.o $(BUILD)/loadidt.o $(BUILD)/loadgdt.o $(BUILD)/loadisr.o $(BUILD)/io_util.o $(BUILD)/irq.o $(BUILD)/pic.o $(BUILD)/keyboard_handler.o $(BUILD)/paging.o
OBJECTS_ASM=$(BUILD)/kernel.o $(BUILD)/header.o $(BUILD)/io.o $(BUILD)/idt.o $(BUILD)/gdt.o $(BUILD)/isr.o 
OBJECTS_FONT=$(BUILD)/UbuntuMono-B-8x16.o $(BUILD)/UbuntuMono-R-8x16.o $(BUILD)/UbuntuMono-BI-8x16.o $(BUILD)/UbuntuMono-RI-8x16.o

all: always $(BUILD)/os.iso 

run: $(BUILD)/os.iso
	bochs -f bochsrc.txt -q

$(BUILD)/%.o: $(SRC)/asm/%.asm
	$(ASM) $(ASM_FLAGS) $< -o $@

$(BUILD)/%.o: $(SRC)/c/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/%.o: $(UTILS)/%.psf
	$(OBJ) $(OBJ_FLAGS) $< $@

$(BOOT)/kernel.elf: $(OBJECTS_C) $(OBJECTS_ASM) $(OBJECTS_FONT)
	ld -T $(SRC)/$(GNU_LINK_FILE) -melf_i386 $(OBJECTS_C) $(OBJECTS_ASM) $(OBJECTS_FONT) -o $@

$(BUILD)/os.iso: $(BOOT)/kernel.elf 
	# genisoimage -R -b boot/grub/$(GRUB_BL) -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o $@ $(ISO)
	grub-mkrescue -o $@ $(ISO)

always:
	mkdir -p $(BUILD)

clean:
	rm -f $(BUILD)/*.o
	rm -f $(BOOT)/kernel.elf
	rm -f $(BUILD)/os.iso
