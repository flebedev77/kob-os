#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

void printkf(const char* restrict format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
void panick(const char* restrict format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
void assertk(bool condition, const char* restrict format, ...) {
  if (condition) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    __asm__ volatile( "hlt" );
  }
}

void outb(uint16_t port, uint8_t value) {
  __asm__ volatile( "outb %0, %1"
      :
      : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
  volatile uint8_t value;
  __asm__ volatile( "inb %1, %0"
      : "=a"(value)
      : "Nd"(port));
  return value;
}
