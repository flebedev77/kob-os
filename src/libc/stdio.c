#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <term.h>
#include <types.h>

// These probably need to go into string.h
static char symbol_upper_table[255] = {0};
static char symbol_lower_table[255] = {0};

void strings_init() {
  symbol_upper_table['1'] = '!';
  symbol_upper_table['2'] = '@';
  symbol_upper_table['3'] = '#';
  symbol_upper_table['4'] = '$';
  symbol_upper_table['5'] = '%';
  symbol_upper_table['6'] = '^';
  symbol_upper_table['7'] = '&';
  symbol_upper_table['8'] = '*';
  symbol_upper_table['9'] = '(';
  symbol_upper_table['0'] = ')';

  symbol_upper_table['-'] = '_';
  symbol_upper_table['='] = '+';
  symbol_upper_table['`'] = '~';
  symbol_upper_table['['] = '{';
  symbol_upper_table[']'] = '}';
  symbol_upper_table['\\'] = '|';
  symbol_upper_table[','] = '<';
  symbol_upper_table['.'] = '>';
  symbol_upper_table['/'] = '?';
  symbol_upper_table[';'] = ':';
  symbol_upper_table['\''] = '"';

  for (uint8_t i = 0; i < 255; i++) {
    symbol_lower_table[(uint8_t)symbol_upper_table[i]] = i;
  }
}

bool is_symbol(char c) { // ASCII decided to sprinkle the symbols about
  if (c > 'Z' && c < 'a') return true;
  if (c > 'z') return true;
  if (c < 'A' && c > ' ') return true;
  return false;
}

char symbol_to_upper(char s) {
  if (!is_symbol(s)) return s;
  char out = symbol_upper_table[(uint8_t)s];
  if (!is_symbol(out)) return s;
  return out;
}
char symbol_to_lower(char s) {
  if (!is_symbol(s)) return s;
  char out = symbol_lower_table[(uint8_t)s];
  if (!is_symbol(out)) return s;
  return out;
}

char char_to_upper(char c) {
  if (is_symbol(c)) return symbol_to_upper(c);
  if (c > 'z' || c < 'a') return c; // already capital
  return c - (ALPHABET_LEN + SPECIALCHR_LEN);
}

char char_to_lower(char c) {
  if (is_symbol(c)) return symbol_to_lower(c);
  if (c > 'Z' || c < 'A') return c; // already lowercase
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

void int_to_str(int num, char* restrict str) {
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

// TODO: add decimal places specification support. Eg %02X %02f
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
            char buf[int_digits_amt(val)];
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
          // case 'f': break; TODO: add floating point numbers support
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

void stdio_init() {
  strings_init();
}
