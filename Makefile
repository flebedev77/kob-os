AS=i686-elf-as
CC=i686-elf-gcc

INCLUDES=-Isrc/include -Isrc/include/libc
CFLAGS=-ffreestanding -std=gnu99 -Wall -Wextra -O2 $(INCLUDES)
LINKFLAGS=-ffreestanding -O2 -nostdlib -lgcc

BUILD_DIR=build

TARGET=$(BUILD_DIR)/kernel
OBJS=$(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/drivers/vgaterm.o \
		 $(BUILD_DIR)/libc/stdio.o $(BUILD_DIR)/libk/io.o

all: $(OBJS) $(TARGET) run

.PHONY: setup clean

setup:
	mkdir -p $(BUILD_DIR)/drivers
	mkdir -p $(BUILD_DIR)/libc
	mkdir -p $(BUILD_DIR)/libk

$(TARGET): $(OBJS)
	$(CC) -T linker.ld -o $@ $(OBJS) $(LINKFLAGS)
$(BUILD_DIR)/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: src/%.s
	$(AS) $< -o $@

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

clean:
	rm $(OBJS)
	rm $(TARGET)
