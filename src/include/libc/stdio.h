#pragma once
#include <stdarg.h>

void stdio_init();

char char_to_upper(char c);
char char_to_lower(char c);
int printf(const char* restrict format, ...);
int vprintf(const char* restrict format, va_list args);

void int_to_str(int val, char* restrict str);
