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

typedef uint8_t vga_color_t;

struct term_cursor {
  size_t x, y;
};

struct term_cursor cursor = {0};

static inline vga_color_t vga_color(enum vga_color fg, enum vga_color bg) {
  return (bg << 4) | fg;
}

static inline void term_cursor_advance(struct term_cursor* cursor) {
  if (cursor == NULL)
    return;

  cursor->x++;
  if (cursor->x > VGA_WIDTH) {
    cursor->x = 0;
    cursor->y++;

    if (cursor->y > VGA_HEIGHT) {
      // TODO: scroll terminal here
      cursor->y--;
    }
  }
}
static inline void term_cursor_advanceln(struct term_cursor* cursor) {
  if (cursor == NULL) return;
  cursor->x = 0;
  cursor->y++;
}

static inline size_t term_cursorpos_to_vga_idx(struct term_cursor* cursor) {
  if (cursor == NULL) return 0;
  return (cursor->y * VGA_WIDTH) + cursor->x;
}

void term_print(const char* text, vga_color_t color) {
  for (size_t i = 0; text[i] != 0; i++) {
    if (text[i] == '\n') {
      term_cursor_advanceln(&cursor);
      continue;
    }
    vga_mem_ptr[term_cursorpos_to_vga_idx(&cursor)] = text[i] | (color << 8);
    term_cursor_advance(&cursor);    
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
  term_print("Hello world!\nHi from a new line!\n\n", vga_color(VGA_COLOR_RED, VGA_COLOR_WHITE));
  term_print("Hi from a new line and different color!\n", vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));

  while (true) {}
}
