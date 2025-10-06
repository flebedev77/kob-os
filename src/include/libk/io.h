#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <multiboot.h>

void printkf(const char* restrict format, ...);
void panick(const char* restrict format, ...);
void assertk(bool condition, const char* restrict format, ...);

inline void outb(uint16_t port, uint8_t value) {
  __asm__ volatile( "outb %0, %1"
      :
      : "a"(value), "Nd"(port));
}

inline uint8_t inb(uint16_t port) {
  volatile uint8_t value;
  __asm__ volatile( "inb %1, %0"
      : "=a"(value)
      : "Nd"(port));
  return value;
}

inline void outw(uint16_t port, uint16_t value) {
  __asm__ volatile( "outw %0, %1"
      :
      : "a"(value), "Nd"(port));
}

inline uint16_t inw(uint16_t port) {
  volatile uint16_t value;
  __asm__ volatile( "inw %1, %0"
      : "=a"(value)
      : "Nd"(port));
  return value;
}

inline void io_wait(void) {
    outb(0x80, 0);
}

extern struct multiboot_info* multiboot_ptr;

void print_mb(struct multiboot_info* mbd, bool verbose);
