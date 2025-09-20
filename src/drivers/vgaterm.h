#pragma once
#include <stdint.h>
#include <stddef.h>

#define VGA
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

#define SCREEN_WIDTH  VGA_WIDTH
#define SCREEN_HEIGHT VGA_HEIGHT

typedef uint8_t vga_color_t;
typedef vga_color_t screen_color_t;
typedef uint16_t tuint_t;

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

struct term_cursor {
  tuint_t x, y;
};


extern struct term_cursor cursor;

inline vga_color_t vga_color(enum vga_color fg, enum vga_color bg) {
  return (bg << 4) | fg;
}
inline vga_color_t def_vga_color() {
  return vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

inline screen_color_t screen_color(enum vga_color fg, enum vga_color bg) {
  return (screen_color_t)vga_color(fg, bg);
}
inline screen_color_t def_screen_color() {
  return (screen_color_t)def_vga_color();
}

tuint_t term_xy_to_vga_idx(tuint_t x, tuint_t y);
tuint_t term_cursorpos_to_vga_idx(struct term_cursor* cursor);

void term_scroll(struct term_cursor* cursor);

void term_cursor_advance(struct term_cursor* cursor);
void term_cursor_advanceln(struct term_cursor* cursor);


void term_print(const char* text, screen_color_t color);
void term_putchar(int c, screen_color_t color);

void term_clear();
