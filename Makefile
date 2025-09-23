AS=i686-elf-as
CC=i686-elf-gcc

INCLUDES=-Isrc/include -Isrc/include/libc
CFLAGS=-ffreestanding -std=gnu99 -Wall -Wextra -O2 $(INCLUDES)
LINKFLAGS=-ffreestanding -O2 -nostdlib -lgcc

BUILD_DIR=build

TARGET=$(BUILD_DIR)/oskrn
ISO_TARGET=$(BUILD_DIR)/os.iso
OBJS=$(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/drivers/vgaterm.o \
		 $(BUILD_DIR)/libc/stdio.o $(BUILD_DIR)/libk/io.o $(BUILD_DIR)/kernel/gdt.o \
		 $(BUILD_DIR)/kernel/util.o

all: $(OBJS) $(TARGET) run

.PHONY: setup clean $(ISO_TARGET)

setup:
	mkdir -p $(BUILD_DIR)/drivers
	mkdir -p $(BUILD_DIR)/libc
	mkdir -p $(BUILD_DIR)/libk
	mkdir -p $(BUILD_DIR)/kernel

$(TARGET): $(OBJS)
	$(CC) -T linker.ld -o $@ $(OBJS) $(LINKFLAGS)
$(BUILD_DIR)/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: src/%.s
	$(AS) $< -o $@

$(ISO_TARGET): $(TARGET)
	mkdir -p $(BUILD_DIR)/iso/boot/grub
	cp $(TARGET) $(BUILD_DIR)/iso/boot/kernel
	echo "set timeout=0"            > $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "set default=0"            >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "menuentry \"OS\" {"       >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "	multiboot /boot/kernel" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "}"                        >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_TARGET) $(BUILD_DIR)/iso
	rm -r $(BUILD_DIR)/iso

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

clean:
	rm $(OBJS)
	rm $(TARGET)
	rm $(ISO_TARGET)
	rm $(BUILD_DIR)/iso -r
