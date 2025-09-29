#include <idt.h>
#include <gdt.h>
#include <libk/io.h>
#include <stdbool.h>
#include "util.h"

#define IDT_MAX_DESCRIPTORS 32

__attribute__((aligned(8)))
static uint8_t entries[8 * 256] = {0};

struct idtentry {
  uint32_t isr;
  uint8_t attributes;
};

void exception_handler() {
  panick("Exception");
  __asm__ volatile ("cli; hlt");
}
void encode_idt_entry(uint8_t* restrict target, struct idtentry source) {
  // isr low
  target[0] = source.isr & 0xFF;
  target[1] = (source.isr >> 8) & 0xFF;

  // kernel code segment
  target[2] = GDT_KERNEL_CODE_SELECTOR & 0xFF;
  target[3] = (GDT_KERNEL_CODE_SELECTOR >> 8) & 0xFF;

  // reserved set to 0
  target[4] = 0;

  // attributes
  target[5] = source.attributes;

  //isr high
  target[6] = (source.isr >> 16) & 0xFF;
  target[7] = (source.isr >> 24) & 0xFF;
}

void idt_init() {
  for (int index = 0; index < 32; index++) {
    encode_idt_entry(&entries[8 * index], (struct idtentry){ 
        .isr = isr_stub_table[index],
        .attributes = 0x8E
    });
  }

  set_idt((8 * IDT_MAX_DESCRIPTORS) - 1, (uint32_t)entries);
}
