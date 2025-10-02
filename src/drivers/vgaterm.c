#include "vgaterm.h"
#include <libk/io.h>

struct term_cursor cursor = {0};

static uint16_t* vga_mem_ptr = (uint16_t*)VGA_MEMORY;

static void term_cursor_boundscheck(struct term_cursor* cursor) {
  if (cursor->x < 0) cursor->x = 0;
  if (cursor->x > VGA_WIDTH) cursor->x = 0;
  if (cursor->y < 0) cursor->y = 0;
}

uint16_t term_xy_to_vga_idx(tint_t x, tint_t y) {
  tint_t idx = (y * VGA_WIDTH) + x;
  if (idx > VGA_WIDTH * VGA_HEIGHT) {
    idx = VGA_WIDTH * VGA_HEIGHT;
  }
  return idx;
}
uint16_t term_cursorpos_to_vga_idx(struct term_cursor cursor) {
  return term_xy_to_vga_idx(cursor.x, cursor.y);
}

void term_scroll(struct term_cursor* cursor) {
  for (tint_t y = 1; y < VGA_HEIGHT; y++) {
    for (tint_t x = 0; x < VGA_WIDTH; x++) {
      vga_mem_ptr[term_xy_to_vga_idx(x, y-1)] = vga_mem_ptr[term_xy_to_vga_idx(x, y)];
    }
  }
  for (tint_t x = 0; x < VGA_WIDTH; x++) {
    vga_mem_ptr[term_xy_to_vga_idx(x, VGA_HEIGHT-1)] = 0;
  }

  if (cursor != NULL)
    cursor->y--;
}

void term_cursor_advance(struct term_cursor* cursor) {
  if (cursor == NULL)
    return;

  cursor->x++;
  if (cursor->x > VGA_WIDTH) {
    cursor->x = 0;
    cursor->y++;

    if (cursor->y >= VGA_HEIGHT-2) {
      term_scroll(cursor);
    }
  }
}

void term_cursor_advanceln(struct term_cursor* cursor) {
  if (cursor == NULL) return;
  cursor->x = 0;
  cursor->y++;

  if (cursor->y >= VGA_HEIGHT-2) {
    term_scroll(cursor);
  }
}


void term_print(const char* text, vga_color_t color) {
  term_cursor_boundscheck(&cursor);
  for (tint_t i = 0; text[i] != 0; i++) {
    if (text[i] == '\n') {
      term_cursor_advanceln(&cursor);
      continue;
    }
    vga_mem_ptr[term_cursorpos_to_vga_idx(cursor)] = text[i] | (color << 8);
    term_cursor_advance(&cursor);    
  }
  term_cursor_boundscheck(&cursor);
  term_cursor_update_position(&cursor);
}

void term_putchar(int c, screen_color_t color) {
  term_cursor_boundscheck(&cursor);
  if (c == '\n') {
    term_cursor_advanceln(&cursor);
    return;
  }
  vga_mem_ptr[term_cursorpos_to_vga_idx(cursor)] = c | (color << 8);
  term_cursor_advance(&cursor);
  term_cursor_boundscheck(&cursor);
  term_cursor_update_position(&cursor);
}

void term_clear() {
  for (tint_t y = 0; y < VGA_HEIGHT; y++) {
    for (tint_t x = 0; x < VGA_WIDTH; x++) {
      vga_mem_ptr[x + y * VGA_WIDTH] = 0;
    }
  }
}

void term_cursor_update_position(struct term_cursor* cursor) {
  if (cursor == NULL) return;

  uint16_t cursor_pos = term_cursorpos_to_vga_idx(*cursor);
  outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (cursor_pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((cursor_pos >> 8) & 0xFF));
}

void term_cursor_hide() {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}
void term_cursor_show(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
