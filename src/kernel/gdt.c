#include <libk/io.h>
#include <stdint.h>
#include "util.h"

struct gdtentry {
  uint32_t limit;
  uint32_t base;
  uint8_t access_byte;
  uint8_t flags;
};

__attribute__((aligned(8)))
static uint8_t entries[8 * 4] = {0};

void encode_gdt_entry(uint8_t* restrict target, struct gdtentry source)
{
    if (source.limit > 0xFFFFF) {panick("GDT cannot encode limits larger than 0xFFFFF");}
    
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;
    
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
    
    target[5] = source.access_byte;
    
    target[6] |= (source.flags << 4);
}

void gdt_init(void) {

  encode_gdt_entry(&entries[8 * 0], (struct gdtentry){0}); //null segment 0x0
  encode_gdt_entry(&entries[8 * 1], (struct gdtentry){0}); //unused segment 0x8
  encode_gdt_entry(&entries[8 * 2], (struct gdtentry){.base = 0, .limit = 0xFFFFF, .access_byte = 0x9A, .flags = 0xC}); //kernel code segment 0x10
  encode_gdt_entry(&entries[8 * 3], (struct gdtentry){.base = 0, .limit = 0xFFFFF, .access_byte = 0x92, .flags = 0xC}); //kernel data segment 0x18
  
  set_gdt((8 * 4)-1, (uint32_t)entries);
}
