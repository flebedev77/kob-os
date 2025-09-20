#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <term.h>
#include <stdio.h>
#include <libk/io.h>

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
  printkf("Mmap detected:              %b\n", is_mmap_valid);
  printkf("Contiguous memory detected: %b\n", is_contiguous_mem_valid);
  printkf("---------------------------\n");
}

void k_main(struct multiboot_info* mbd, unsigned int mb_magic) {
  term_clear();

  assertk(mb_magic != MULTIBOOT_BOOTLOADER_MAGIC, "Bootloader magic number is incorrect! Should be %x and is %x. The bootloader is messed up!\n", MULTIBOOT_BOOTLOADER_MAGIC, (int)mb_magic);

  print_mb(mbd);

  term_print("Hello!\n", def_screen_color());
  term_print("Hello world!\nHi from a new line!\n\n", screen_color(VGA_COLOR_RED, VGA_COLOR_BLUE));
  vga_color_t col = 0; 
  term_print("Hi from a new line and different color!\n", col++);
  term_print("Hi from a new line and different color!\n", col++);
  term_print("Hi from a new line and different color!\n", col++);

  char* test = "h\n";
  test[0] = char_to_lower('B');
  term_print(test, def_screen_color());

  cursor.x = 5;
  printkf("Hello world %s! Number example %d in hex %x\n", "bob", 123, 123);

  printkf("Multiboot magic number: %x\n", mb_magic);

  while (true) {}
}
