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
#include "tty/tty.h"

#include <multiboot.h>

#if defined(__linux__)
#error "Not using a correct cross compiler"
#endif

#if !defined(__i386__)
#error "Kernel only works with a 32 bit compiler"
#endif

void k_main(struct multiboot_info* mbd, unsigned int mb_magic) {
  multiboot_ptr = mbd;
  term_clear();
  stdio_init();

  // print_mb(mbd);

  gdt_init();
  pic_init();
  idt_init();

  pckbd_init();
  tty_init();


  assertk(mb_magic != MULTIBOOT_BOOTLOADER_MAGIC, "Bootloader magic number is incorrect! Should be %x and is %x. The bootloader is messed up!\n", MULTIBOOT_BOOTLOADER_MAGIC, (int)mb_magic);


  // cursor.x = 3;
  // cursor.y = 2;
  // term_cursor_update_position(&cursor);
  // term_cursor_hide();
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

  // tty_run never returns, looks for the init process and runs it.
  // if the init process was not found, then will run a kernel shell
  // which will let you access basic system information, shutdown,
  // specify a new place to look for the init executable, etc.
  tty_run();

  while (true) {}
}
