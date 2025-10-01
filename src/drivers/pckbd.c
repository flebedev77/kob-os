#include <stdint.h>
#include <libk/io.h>
#include <term.h>

#define PCKBD_DATA 0x60
#define PCKBD_COMMAND 0x64

void pckbd_init(void) {
  // while (inb(PCKBD_COMMAND) & 1) {
  //   inb(PCKBD_DATA);
  // }

  // outb(PCKBD_COMMAND, 0xAE);
  // outb(PCKBD_COMMAND, 0x20);
  // uint8_t status = (inb(PCKBD_DATA) | 1) & ~0x10;
  // outb(PCKBD_COMMAND, 0x60);
  // outb(PCKBD_DATA, status);
  // outb(PCKBD_DATA, 0xF4);

  // commandport.Write(0xae); // activate interrupts
  // commandport.Write(0x20); // command 0x20 = read controller command byte
  // uint8_t status = (dataport.Read() | 1) & ~0x10;
  // commandport.Write(0x60); // command 0x60 = set controller command byte
  // dataport.Write(status);
  // dataport.Write(0xf4);


}

char scancode_to_char(uint8_t scancode) {
  char out = 0;

  switch (scancode) {
    case 0x10: out = 'q'; break;
    case 0x11: out = 'w'; break;
    case 0x12: out = 'e'; break;
    case 0x13: out = 'r'; break;
    case 0x14: out = 't'; break;
    case 0x15: out = 'y'; break;
    case 0x16: out = 'u'; break;
    case 0x17: out = 'i'; break;
    case 0x18: out = 'o'; break;
    case 0x19: out = 'p'; break;
    case 0x1A: out = '['; break;
    case 0x1B: out = ']'; break;
    case 0x2B: out = '\\'; break;

    case 0x1E: out = 'a'; break;
    case 0x1F: out = 's'; break;
    case 0x20: out = 'd'; break;
    case 0x21: out = 'f'; break;
    case 0x22: out = 'g'; break;
    case 0x23: out = 'h'; break;
    case 0x24: out = 'j'; break;
    case 0x25: out = 'k'; break;
    case 0x26: out = 'l'; break;
    case 0x27: out = ';'; break;
    case 0x28: out = '\''; break;

    case 0x1C: out = '\n'; break; // enter

    case 0x2C: out = 'z'; break;
    case 0x2D: out = 'x'; break;
    case 0x2E: out = 'c'; break;
    case 0x2F: out = 'v'; break;
    case 0x30: out = 'b'; break;
    case 0x31: out = 'n'; break;
    case 0x32: out = 'm'; break;
    case 0x33: out = ','; break;
    case 0x34: out = '.'; break;
    case 0x35: out = '/'; break;
  }

  return out;
}

void pckbd_interrupt() {
  uint8_t scancode = inb(PCKBD_DATA);

  if (scancode < 0x80) {
#if 0
    printkf("%X ", scancode);

    cursor.y--;
    cursor.x -= 2;
    printkf("%c ", scancode_to_char(scancode));
    cursor.x += 2;
    cursor.y++;
#else
    printkf("%c", scancode_to_char(scancode));
#endif
  }
}
