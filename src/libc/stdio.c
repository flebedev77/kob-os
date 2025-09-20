#include <stdarg.h>
#include <stddef.h>
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

int printf(const char* format, ...) {
  va_list args;
  va_start(args, format);

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
            term_putchar(va_arg(args, int), def_screen_color());
            break;
          case 's':
            term_print(va_arg(args, char*), def_screen_color());
            break;
          case 'd': {
            int val = va_arg(args, int);
            char buf[int_digits_amt(val)]; // TODO fix size allocation with int_digits_amt           
            int_to_str(val, buf);
            term_print(buf, def_screen_color());
          } break;
          // case 'f': break;
          // case 'x': break;
          default:
            printf("%c not supported\n", suffix);
            break;
        }
      }
    } else {
      term_putchar(current_char, def_screen_color());
    }
  }

  return EXIT_SUCCESS;
}
