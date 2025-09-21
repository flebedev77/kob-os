#include <stdio.h>
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
    __asm__ ( "hlt" );
  }
}
