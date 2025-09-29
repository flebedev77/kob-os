#!/usr/bin/tcc -run
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

uint16_t make_segment_selector(uint16_t index, uint8_t ti, uint8_t rpl) {
  uint16_t out = 0;
  uint16_t index_limit = 0b111111111111;
  if (index > index_limit) { // limit is actually 12 bit  15-3
    printf("Index too large, cannot be larger than %X\n", index_limit);
    return out;
  }

  if (ti > 1) {
    printf("TI is too large (0-1)\n");
    return out;
  }

  if (rpl > 3) {
    printf("RPL is too large (0-3)\n");
    return out;
  }
  
  out |= (index & index_limit) << 3;
  out |= (ti & 0b1) << 2;
  out |= rpl & 0b11;

  return out;
}

int main(int argc, char** argv) {
  for (uint8_t ring = 0; ring < 4; ring++) {
    for (uint16_t i = 0; i < 4; i++) {
      uint16_t segment_selector = make_segment_selector(i, 0, ring);
      printf("Segment at index %d in GDT and ring %d: 0x%X\n", i, ring, segment_selector);
    }
  }
  return 0;
}
