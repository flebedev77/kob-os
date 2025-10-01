#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <term.h>
#include <stdio.h>
#include <libk/io.h>
#include <gdt.h>
#include <idt.h>
#include <pic.h>

#include "drivers/pckbd.h"

#include <multiboot.h>

#if defined(__linux__)
#error "Not using a correct cross compiler"
#endif

#if !defined(__i386__)
#error "Kernel only works with a 32 bit compiler"
#endif

// https://wiki.osdev.org/Detecting_Memory_(x86)
void print_mb(struct multiboot_info* mbd) {
  bool is_mmap_valid = (mbd->flags >> 6) & 0b1;
  bool is_contiguous_mem_valid = mbd->flags & 0b1;

  printkf("------- Memory info -------\n");
  printkf("Memory map detected:        %b\n", is_mmap_valid);
  printkf("Contiguous memory detected: %b\n", is_contiguous_mem_valid);
  printkf("---------------------------\n");

  if (is_contiguous_mem_valid) {
    printkf("Contiguous mem lower %d Kib\n", mbd->mem_lower);
    printkf("Contiguous mem upper %d Kib\n", mbd->mem_upper);
    printkf("Contiguous mem size  %d Kib\n", (mbd->mem_upper-mbd->mem_lower));
    printkf("---------------------------\n");
  }

  if (is_mmap_valid && false) {
    int idx = 0;
    for(uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
      multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i);

      printkf("Memory map %d\n", idx);
      printkf(" Start address: 0x%x%x\n", mmmt->addr_high, mmmt->addr_low);
      printkf(" Length:        0x%x%x\n", mmmt->len_high, mmmt->len_low);
      printkf(" Size:          0x%x\n", mmmt->size);
      char* type_str;
      switch(mmmt->type) {
        case 1: type_str = "Usable RAM"; break;
        case 2: type_str = "Reserved unusable"; break;
        case 3: type_str = "ACPI reclaimable RAM"; break;
        case 4: type_str = "ACPI NVS RAM"; break;
        case 5: type_str = "Bad memory"; break;
        default: type_str = "Unknown"; break;
      }
      printkf(" Type:          %d (%s)\n", mmmt->type, type_str);
      printkf("---------------------------\n");
      idx++;
    }
  }
}

void k_main(struct multiboot_info* mbd, unsigned int mb_magic) {
  term_clear();

  // print_mb(mbd);

  gdt_init();
  pic_init();
  idt_init();

  pckbd_init();

  assertk(mb_magic != MULTIBOOT_BOOTLOADER_MAGIC, "Bootloader magic number is incorrect! Should be %x and is %x. The bootloader is messed up!\n", MULTIBOOT_BOOTLOADER_MAGIC, (int)mb_magic);


  cursor.x = 3;
  cursor.y = 2;
  term_cursor_update_position(&cursor);
  term_cursor_hide();
  // term_cursor_show(0, 15);

  // term_print("Hello!\n", def_screen_color());
  // term_print("Hello world!\nHi from a new line!\n\n", screen_color(VGA_COLOR_RED, VGA_COLOR_BLUE));
  // vga_color_t col = 0; 
  // term_print("Hi from a new line and different color!\n", col++);
  // term_print("Hi from a new line and different color!\n", col++);
  // term_print("Hi from a new line and different color!\n", col++);
  //
  // char* test = "h\n";
  // test[0] = char_to_lower('B');
  // term_print(test, def_screen_color());
  //
  // cursor.x = 5;
  // printkf("Hello world %s! Number example %d in hex %x\n", "bob", 123, 123);
  //
  // printkf("Multiboot magic number: %x\n", mb_magic);

  while (true) {}
}
