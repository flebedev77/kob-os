#pragma once
#include <stdarg.h>

void stdio_init();

char char_to_upper(char c);
char char_to_lower(char c);
int printf(const char* format, ...);
int vprintf(const char* format, va_list args);

void int_to_str(int val, char* str);
