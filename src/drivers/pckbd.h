#pragma once
#include <stdint.h>

// Number row
#define KEY_BACKTICK     0x29

#define KEY_ONE   0x02
#define KEY_TWO   0x03
#define KEY_THREE 0x04
#define KEY_FOUR  0x05
#define KEY_FIVE  0x06
#define KEY_SIX   0x07
#define KEY_SEVEN 0x08
#define KEY_EIGHT 0x09
#define KEY_NINE  0x0A
#define KEY_ZERO  0x0B

#define KEY_MINUS   0x0C
#define KEY_EQUALS  0x0D

// Top row
#define KEY_Q 0x10
#define KEY_W 0x11
#define KEY_E 0x12
#define KEY_R 0x13
#define KEY_T 0x14
#define KEY_Y 0x15
#define KEY_U 0x16
#define KEY_I 0x17
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_LEFT_BRACKET  0x1A
#define KEY_RIGHT_BRACKET 0x1B
#define KEY_BACKSLASH     0x2B

// Home row
#define KEY_A 0x1E 
#define KEY_S 0x1F 
#define KEY_D 0x20 
#define KEY_F 0x21 
#define KEY_G 0x22 
#define KEY_H 0x23 
#define KEY_J 0x24 
#define KEY_K 0x25 
#define KEY_L 0x26 
#define KEY_SEMICOLON 0x27 
#define KEY_QUOTE     0x28 

// Bottom row
#define KEY_Z 0x2C 
#define KEY_X 0x2D 
#define KEY_C 0x2E 
#define KEY_V 0x2F 
#define KEY_B 0x30 
#define KEY_N 0x31 
#define KEY_M 0x32 
#define KEY_COMMA 0x33 
#define KEY_DOT   0x34 
#define KEY_SLASH 0x35 

// Misc keys
#define KEY_ENTER 0x1C
#define KEY_SPACE 0x39

// Stateful characters
#define KEY_RELEASE_OFFSET 128 // Add this to key down scancode and you will get the scancode for release
#define KEY_TAB 0x0F
#define KEY_CAPSLOCK 0x3A
#define KEY_LEFTSHIFT 0x2A
#define KEY_RIGHTSHIFT 0x36
#define KEY_BACKSPACE 0x0E


struct pckbd_state {
  bool left_shift,  // Whether or not the left shift is pressed down or not
       right_shift, // Same as left shift
       capslock,    // Whether capslock is toggled
       numlock,     // Same as capslock
       pause_break, // Same as capslock
       special,     // Same as capslock
       backspace,   // When initially pressed down turns to true, will remain true until any key is released
       tab,         // Same as backspace
       caps;        // True when left shift, right shift or capslock are true
  char key_pressed; // Last key pressed
};

extern struct pckbd_state kbd_state;

void pckbd_init(void);

void pckbd_interrupt(void);
