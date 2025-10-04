#include <stdbool.h>
#include <stdio.h>
#include <term.h>
#include <string.h>
#include <libk/io.h>
#include "../drivers/pckbd.h"

#define TTY_MAX_COMMAND_LEN 1024

static char* tty_prompt = "> ";
static uint32_t tty_prompt_len = 0;

static char tty_command[TTY_MAX_COMMAND_LEN] = {0};
static uint32_t tty_command_len = 0;

void tty_drawprompt() {
  printf(tty_prompt);
}
void tty_clearcommand() {
  for (uint32_t i = 0; i < tty_command_len; i++) {
    tty_command[i] = 0;
  }
  tty_command_len = 0;
}

void tty_helpcommand() {
  printf("TTY help:\n");
  printf("  help - shows this menu\n");
  printf("  memory - shows memory information\n");
  printf("  memory -v - shows more memory information\n");
}

void tty_memorycommand(bool verbose) {
  print_mb(multiboot_ptr, verbose);
}

void tty_runcommand() {
  if (strcmp(tty_command, "help") == 0) tty_helpcommand();
  else if (strcmp(tty_command, "memory") == 0) tty_memorycommand(false);
  else if (strcmp(tty_command, "memory -v") == 0) tty_memorycommand(true);
  else printf("Unknown command: %s\n", tty_command);
}

void tty_cursormovex(char direction) {
  if (direction == 0 || direction < -1 || direction > 1) return;

  if (direction == -1 && (uint32_t)cursor.x > tty_prompt_len) cursor.x--;
  if (direction == 1) cursor.x++;
}

void on_keypress(struct pckbd_state* keyboard) {
  if (keyboard->backspace && (uint32_t)cursor.x > tty_prompt_len) {
    cursor.x--;
    term_putchar(' ', def_screen_color());
    cursor.x--;

    tty_command[tty_command_len] = 0;
    tty_command_len--;
  }

  if (keyboard->tab) {
    printf("  ");
  }

  // if (keyboard->up_arrow) cursor.y--;
  // if (keyboard->down_arrow) cursor.y++;
  if (keyboard->left_arrow) tty_cursormovex(-1);
  if (keyboard->right_arrow) tty_cursormovex(1);

  term_cursor_update_position(&cursor);

  if (keyboard->key_pressed == 0) return;

  printf("%c", keyboard->key_pressed);

  if (keyboard->key_pressed == '\n') {
    tty_runcommand();
    tty_clearcommand();
    tty_drawprompt();
  } else if (tty_command_len < TTY_MAX_COMMAND_LEN+1) {
    tty_command[tty_command_len] = keyboard->key_pressed;
    tty_command_len++;
  }
}

void tty_init() {
  tty_prompt_len = (uint32_t)strlen(tty_prompt);
  tty_command_len = 0;

  term_cursor_show(0, 15);
  pckbd_register_onkeypress(on_keypress); 

  tty_drawprompt();
}

void tty_run() {
  while (true) {}
}
