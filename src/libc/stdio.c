#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <term.h>
#include <types.h>

// These probably need to go into string.h
char char_to_upper(char c) {
  if (c < 'a') return c; // already capital
  return c - (ALPHABET_LEN + SPECIALCHR_LEN);
}

char char_to_lower(char c) {
  if (c > 'Z') return c; // already lowercase
  return c + (ALPHABET_LEN + SPECIALCHR_LEN);
}

int int_digits_amt(int val) {
  int i = 0;
  while(val != 0) {
    val /= 10;
    i++;
  }
  return i;
}

void int_to_str(int num, char* str) {
    int i = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative)
        str[i++] = '-';

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static const char* hexchars = "0123456789ABCDEF";
int hex_digits_amt(int num) {
  int i = 0;
  for (i = 0; num > 0; i++) {
    i++;
    num /= 16;
  }
  return i;
}

void byte_to_hex(uint8_t num, char* restrict str) {
  str[0] = hexchars[(num >> 4) & 0xF];
  str[1] = hexchars[num & 0xF];
  str[2] = 0;
}

void uint32_to_hex(uint32_t num, char* restrict str) {
  // size_t i;
  // for (i = 0; num != 0; i++) {
  //   str[i] = hexchars[num & 0xF];
  //   num /= 16;
  // }
  // str[i] = 0;

  int idx = 0;
  bool exclude_zeros = true;
  for (int i = 7; i >= 0; i--) {
    char nibble = hexchars[(num >> (i*4)) & 0xF];
    if (nibble != '0') exclude_zeros = false;
    if (exclude_zeros) continue; 
    str[idx++] = nibble;
  }
  str[idx] = 0;
}

int vprintf(const char* restrict format, va_list args) {
  screen_color_t color = def_screen_color();
  for (size_t i = 0; format[i] != '\0'; i++) {
    char current_char = format[i]; 

    if (current_char == '%') {
      char suffix = char_to_lower(format[i+1]);
      i++;
      if (suffix == '\0') {
        return EXIT_FAIL;
      } else {
        switch (suffix) {
          case 'c':
            term_putchar(va_arg(args, int), color);
            break;
          case 's':
            term_print(va_arg(args, char*), color);
            break;
          case 'd': {
            int val = va_arg(args, int);
            char buf[int_digits_amt(val)]; // TODO fix size allocation with int_digits_amt           
            int_to_str(val, buf);
            term_print(buf, color);
          } break;
          case 'x': {
            int val = va_arg(args, int);
            char buf[hex_digits_amt(val)];
            uint32_to_hex((uint32_t)val, buf);
            term_print(buf, color);
          } break;
          case 'b':
            term_print((va_arg(args, int)) ? "true" : "false", color);
          break;
          // case 'f': break;
          default:
            printf("%c not supported\n", suffix);
            break;
        }
      }
    } else {
      term_putchar(current_char, color);
    }
  }
  return EXIT_SUCCESS;
}

int printf(const char* restrict format, ...) {
  va_list args;
  va_start(args, format);
  int status = vprintf(format, args);
  va_end(args);
  return status;
}
