#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <libk/io.h>
#include <term.h>
#include "pckbd.h"

#define PCKBD_DATA 0x60
#define PCKBD_COMMAND 0x64
#define PCKBD_MAX_RETRIES 5

#define PCKBD_ACK_BYTE 0xFA

// #define DEBUG         true
// #define DEBUG_RELEASE true

struct pckbd_state kbd_state = {0};

void pckbd_init(void) {
  kbd_state = (struct pckbd_state){0};
}

static char scancode_to_char(uint8_t scancode) {
  char out = 0;

  switch (scancode) {
    case KEY_Q: out = 'q'; break;
    case KEY_W: out = 'w'; break;
    case KEY_E: out = 'e'; break;
    case KEY_R: out = 'r'; break;
    case KEY_T: out = 't'; break;
    case KEY_Y: out = 'y'; break;
    case KEY_U: out = 'u'; break;
    case KEY_I: out = 'i'; break;
    case KEY_O: out = 'o'; break;
    case KEY_P: out = 'p'; break;
    case KEY_LEFT_BRACKET: out = '['; break;
    case KEY_RIGHT_BRACKET: out = ']'; break;
    case KEY_BACKSLASH: out = '\\'; break;

    case KEY_A: out = 'a'; break;
    case KEY_S: out = 's'; break;
    case KEY_D: out = 'd'; break;
    case KEY_F: out = 'f'; break;
    case KEY_G: out = 'g'; break;
    case KEY_H: out = 'h'; break;
    case KEY_J: out = 'j'; break;
    case KEY_K: out = 'k'; break;
    case KEY_L: out = 'l'; break;
    case KEY_SEMICOLON: out = ';'; break;
    case KEY_QUOTE: out = '\''; break;

    case KEY_ENTER: out = '\n'; break; // enter
    case KEY_SPACE: out = ' '; break;  // space;

    case KEY_Z: out = 'z'; break;
    case KEY_X: out = 'x'; break;
    case KEY_C: out = 'c'; break;
    case KEY_V: out = 'v'; break;
    case KEY_B: out = 'b'; break;
    case KEY_N: out = 'n'; break;
    case KEY_M: out = 'm'; break;
    case KEY_COMMA: out = ','; break;
    case KEY_DOT: out = '.'; break;
    case KEY_SLASH: out = '/'; break;

    case KEY_ONE:   out = '1'; break;
    case KEY_TWO:   out = '2'; break;
    case KEY_THREE: out = '3'; break;
    case KEY_FOUR:  out = '4'; break;
    case KEY_FIVE:  out = '5'; break;
    case KEY_SIX:   out = '6'; break;
    case KEY_SEVEN: out = '7'; break;
    case KEY_EIGHT: out = '8'; break;
    case KEY_NINE:  out = '9'; break;
    case KEY_ZERO:  out = '0'; break;

    case KEY_MINUS:   out = '-'; break;
    case KEY_EQUALS:  out = '='; break;
    case KEY_BACKTICK:out = '`'; break;


    default: out = 0; break;
  }

  if (kbd_state.caps) out = char_to_upper(out);

  return out;
}

/* light
   Scroll Lock	0x01
   Num Lock	    0x02
   Caps Lock	  0x04
   return 1 on failure
*/
// static bool set_keyboard_light(uint8_t light, bool state) {
//   uint8_t command_byte = 0xED; 
//   outb(PCKBD_COMMAND, command_byte);
//
//   // for(uint8_t i = 0; i < PCKBD_MAX_RETRIES; i++) {
//     uint8_t read_byte = inb(PCKBD_COMMAND);
//     printkf("Set light: %X\n", read_byte);
//   //   if (read_byte != 0xFE) {
//   //     break;
//   //   }
//   // }
//
//   outb(PCKBD_DATA, 0x04);
//   return 0;
// }


static void handle_special_keys(uint8_t scancode) {
  kbd_state.backspace = false;
  kbd_state.tab = false;

  kbd_state.left_arrow = false;
  kbd_state.right_arrow = false;
  kbd_state.up_arrow = false;
  kbd_state.down_arrow = false;

  switch (scancode) {
    case KEY_CAPSLOCK:
      kbd_state.capslock = !kbd_state.capslock;
      break;

    case KEY_LEFTSHIFT:
      kbd_state.left_shift = true;
      break;
    case KEY_LEFTSHIFT + KEY_RELEASE_OFFSET:
      kbd_state.left_shift = false;
      break;

    case KEY_RIGHTSHIFT:
      kbd_state.right_shift = true;
      break;
    case KEY_RIGHTSHIFT + KEY_RELEASE_OFFSET:
      kbd_state.right_shift = false;
      break;

    case KEY_BACKSPACE:
      kbd_state.backspace = true;
      break;

    case KEY_TAB:
      kbd_state.tab = true;
      break;
  }

  if (kbd_state.last_scancode == KEY_EXTENDED_BYTE) {
    switch (scancode) {
      case KEY_LEFTARROW:
        kbd_state.left_arrow = true;
        break;
      case KEY_RIGHTARROW:
        kbd_state.right_arrow = true;
        break;
      case KEY_UPARROW:
        kbd_state.up_arrow = true;
        break;
      case KEY_DOWNARROW:
        kbd_state.down_arrow = true;
        break;
    }
  }

  kbd_state.caps = kbd_state.capslock || kbd_state.left_shift || kbd_state.right_shift;
}

void pckbd_interrupt(void) {
  uint8_t scancode = inb(PCKBD_DATA);

  handle_special_keys(scancode);

  if (kbd_state.backspace) {
    cursor.x--;
    term_putchar(' ', def_screen_color());
    cursor.x--;
  }

  if (kbd_state.tab) {
    printkf("  ");
  }

  if (kbd_state.up_arrow) cursor.y--;
  if (kbd_state.down_arrow) cursor.y++;
  if (kbd_state.left_arrow) cursor.x--;
  if (kbd_state.right_arrow) cursor.x++;
  

#if !DEBUG_RELEASE
  if (scancode < 0x80) { // Only the press down, not release
#endif
    char scancode_char = scancode_to_char(scancode);
    kbd_state.key_pressed = scancode_char;


#if !DEBUG
    if (scancode_char == 0) return;
#endif

#if DEBUG
    printkf("%X ", scancode);

    cursor.y--;
    cursor.x -= 2;
    printkf("%c ", kbd_state.key_pressed);
    cursor.x += 2;
    cursor.y++;
#else
    printkf("%c", kbd_state.key_pressed);
#endif
    
#if !DEBUG_RELEASE
  }
#endif

  kbd_state.last_scancode = scancode;

  term_cursor_update_position(&cursor);
  term_cursor_show(0, 15);
}
