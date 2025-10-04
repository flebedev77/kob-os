#!/usr/bin/tcc -run
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_LEN 25
#define SPECIALCHR_LEN 7 // The amount of special characters in between the upper case alphabet and lowercase alphabet
#define ASCII_NUMBERS_BEGIN 48

void int_to_str(int num, char *str) {
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

const char* hexchars = "0123456789ABCDEF";
void byte_to_hex(uint8_t num, char* str) {
  str[0] = hexchars[(num >> 4) & 0xF];
  str[1] = hexchars[num & 0xF];
  str[2] = 0;
}

void int32_to_hex(uint32_t num, char* str) {
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

bool is_symbol(char c) { // ASCII decided to sprinkle the symbols about
  if (c > 'Z' && c < 'a') return true;
  if (c > 'z') return true;
  if (c < 'A' && c > ' ') return true;
  return false;
}
// Fuck you ascii
char symbol_upper_table[255] = {0};
char symbol_lower_table[255] = {0};

char symbol_to_upper(char s) {
  if (!is_symbol(s)) return s;
  char out = symbol_upper_table[s];
  if (!is_symbol(out)) return s;
  return out;
}
char symbol_to_lower(char s) {
  if (!is_symbol(s)) return s;
  char out = symbol_lower_table[s];
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



void generate_symbol_lower_table() {
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
    symbol_lower_table[symbol_upper_table[i]] = i;
  }
}


int main() {
  generate_symbol_lower_table();
  if (1) {
    printf("Characters\n");
    char* test = "abcdefghijklmnopqrstuvwxyz";
    size_t len = strlen(test);
    char test2[len];

    for (size_t i = 0; i < len; i++) {
      test2[i] = char_to_upper(test[i]);
      printf("%c:%c ", test[i], test2[i]);
    }
    printf("To upper\n");
    for (size_t i = 0; i < len; i++) {
      printf("%c:%c ", test2[i], char_to_lower(test2[i]));
    }
    printf("To lower\nSymbols\n");
  }

  if (1) {
    char* test = "1234567890-=`[]\\,./;'";
    size_t len = strlen(test);
    char test2[len];

    for (size_t i = 0; i < len; i++) {
      test2[i] = symbol_to_upper(test[i]);
      printf("%c:%c ", test[i], test2[i]);
    }
    printf("To upper\n");
    for (size_t i = 0; i < len; i++) {
      printf("%c:%c ", test2[i], symbol_to_lower(test2[i]));
    }
    printf("To lower\n");
  }

  printf("Strcmp wrong: %d\n", strcmp("notsame", "novname"));

  
  // char buf[100];
  // // int_to_str(-1234, buf);
  // // byte_to_hex(253, buf);
  // int val = 0;
  // for (int i = 0; i < 100; i++) {
  //   val++;
  //   int32_to_hex(val, buf);
  //   printf("Mine: %s  Libc: %X\n", buf, val);
  // }
}
