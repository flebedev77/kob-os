#!/usr/bin/tcc -run
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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

int main() {
  char buf[100];
  // int_to_str(-1234, buf);
  // byte_to_hex(253, buf);
  int val = 0;
  for (int i = 0; i < 100; i++) {
    val++;
    int32_to_hex(val, buf);
    printf("Mine: %s  Libc: %X\n", buf, val);
  }
}
