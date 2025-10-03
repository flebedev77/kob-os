#include <stdbool.h>
#include <stdio.h>
#include <term.h>
#include "../drivers/pckbd.h"

void on_keypress(struct pckbd_state* keyboard) {
  if (keyboard->backspace) {
    cursor.x--;
    term_putchar(' ', def_screen_color());
    cursor.x--;
  }

  if (keyboard->tab) {
    printf("  ");
  }

  if (keyboard->up_arrow) cursor.y--;
  if (keyboard->down_arrow) cursor.y++;
  if (keyboard->left_arrow) cursor.x--;
  if (keyboard->right_arrow) cursor.x++;

  term_cursor_update_position(&cursor);

  if (keyboard->key_pressed == 0) return;


  printf("%c", keyboard->key_pressed);
}

void tty_init() {
  pckbd_register_onkeypress(on_keypress); 
}

void tty_run() {
  while (true) {}
}
