AS=i686-elf-as
CC=i686-elf-gcc

TARGET=kernel
BUILD_DIR=build
OBJS=$(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o

all: $(OBJS) $(TARGET) run

$(TARGET): $(OBJS)
	$(CC) -m32 -T linker.ld -o $@ $(OBJS) -ffreestanding -O2 -nostdlib -lgcc

build/%.o: src/%.c
	$(CC) -c -m32 -ffreestanding -std=gnu99 -Wall -Wextra -O2 $< -o $@

build/%.o: src/%.s
	$(AS) $< -o $@

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

clean:
	rm $(OBJS)
	rm $(TARGET)
