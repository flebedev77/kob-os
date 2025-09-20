#include "vgaterm.h"

struct term_cursor cursor = {0};

static uint16_t* vga_mem_ptr = (uint16_t*)VGA_MEMORY;

// TODO: fix these astronomical tuint_t values when they have tiny max values anyways
tuint_t term_xy_to_vga_idx(tuint_t x, tuint_t y) {
  tuint_t idx = (y * VGA_WIDTH) + x;
  if (idx > VGA_WIDTH * VGA_HEIGHT) {
    idx = VGA_WIDTH * VGA_HEIGHT;
  }
  return idx;
}
tuint_t term_cursorpos_to_vga_idx(struct term_cursor* cursor) {
  if (cursor == NULL) return 0;
  return term_xy_to_vga_idx(cursor->x, cursor->y);
}

void term_scroll(struct term_cursor* cursor) {
  for (tuint_t y = 0; y < VGA_HEIGHT-1; y++) {
    for (tuint_t x = 0; x < VGA_WIDTH; x++) {
      vga_mem_ptr[term_xy_to_vga_idx(x, y)] = vga_mem_ptr[term_xy_to_vga_idx(x, y+1)];
    }
  }
  for (tuint_t x = 0; x < VGA_WIDTH; x++) {
    vga_mem_ptr[term_xy_to_vga_idx(x, VGA_HEIGHT)] = 0;
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

    if (cursor->y > VGA_HEIGHT) {
      term_scroll(cursor);
    }
  }
}

void term_cursor_advanceln(struct term_cursor* cursor) {
  if (cursor == NULL) return;
  cursor->x = 0;
  cursor->y++;

  if (cursor->y > VGA_HEIGHT) {
    term_scroll(cursor);
  }
}


void term_print(const char* text, vga_color_t color) {
  for (tuint_t i = 0; text[i] != 0; i++) {
    if (text[i] == '\n') {
      term_cursor_advanceln(&cursor);
      continue;
    }
    vga_mem_ptr[term_cursorpos_to_vga_idx(&cursor)] = text[i] | (color << 8);
    term_cursor_advance(&cursor);    
  }
}

void term_putchar(int c, screen_color_t color) {
  if (c == '\n') {
    term_cursor_advanceln(&cursor);
    return;
  }
  vga_mem_ptr[term_cursorpos_to_vga_idx(&cursor)] = c | (color << 8);
  term_cursor_advance(&cursor);
}

void term_clear() {
  for (tuint_t y = 0; y < VGA_HEIGHT; y++) {
    for (tuint_t x = 0; x < VGA_WIDTH; x++) {
      vga_mem_ptr[x + y * VGA_WIDTH] = 0;
    }
  }
}
