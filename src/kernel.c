#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

#if defined(__linux__)
#error "Not using a correct cross compiler"
#endif

#if !defined(__i386__)
#error "Kernel only works with a 32 bit compiler"
#endif

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

uint16_t* vga_mem_ptr = (uint16_t*)VGA_MEMORY;

// static inline uint16_t term_vga_char(

void term_print(const char* text) {
  for (size_t i = 0; text[i] != 0; i++) {
    vga_mem_ptr[i] = text[i] | (VGA_COLOR_RED << 8);
  }
}

void term_clear() {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      vga_mem_ptr[x + y * VGA_WIDTH] = 0;
    }
  }
}

void k_main(void) {
  term_clear();
  term_print("Hello world!");

  while (true) {}
}
