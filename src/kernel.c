#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <term.h>
#include <stdio.h>

#if defined(__linux__)
#error "Not using a correct cross compiler"
#endif

#if !defined(__i386__)
#error "Kernel only works with a 32 bit compiler"
#endif


void k_main(void) {
  term_clear();
  term_print("Hello!\n", def_vga_color());
  term_print("Hello world!\nHi from a new line!\n\n", vga_color(VGA_COLOR_RED, VGA_COLOR_BLUE));
  vga_color_t col = 0; 
  term_print("Hi from a new line and different color!\n", col++);
  term_print("Hi from a new line and different color!\n", col++);
  term_print("Hi from a new line and different color!\n", col++);
  printf("Hello world %s!\n", "bob");

  while (true) {}
}
