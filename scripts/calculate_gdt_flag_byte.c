#!/usr/bin/tcc -run
#include <stdio.h>
#include <stdint.h>

// https://wiki.osdev.org/Global_Descriptor_Table
uint8_t make_flag_byte(
    uint8_t G,  //G: Granularity flag, indicates the size the Limit value is scaled by. If clear (0), the Limit is in 1 Byte blocks (byte granularity). If set (1), the Limit is in 4 KiB blocks (page granularity).
    uint8_t DB, // DB: Size flag. If clear (0), the descriptor defines a 16-bit protected mode segment. If set (1) it defines a 32-bit protected mode segment. A GDT can have both 16-bit and 32-bit selectors at once.
    uint8_t L   // L: Long-mode code flag. If set (1), the descriptor defines a 64-bit code segment. When set, DB should always be clear. For any other type of segment (other code types or any data segment), it should be clear (0).
    ) {
  uint8_t out = 0;

  out |= (L & 1) << 1;
  out |= (DB & 1) << 2;
  out |= (G & 1) << 3;

  return out;
}

int main() {
  uint8_t flag = make_flag_byte(1, 1, 0);
  uint8_t flag64 = make_flag_byte(1, 0, 1);

  printf("Flag for 4Kib block segment in 32 bit mode %02X\n", flag);
  printf("Flag for 4Kib block segment in 64 bit mode %02X\n", flag64);
  return 0;
}
