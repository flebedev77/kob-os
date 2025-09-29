#include <idt.h>
#include <gdt.h>
#include <libk/io.h>
#include <stdbool.h>
#include "util.h"

struct idtentry {
  uint32_t isr;
  uint8_t attributes;
};

__attribute__((aligned(8)))
static uint8_t entries[8 * 256] = {0};

extern uint8_t intid;

void exception_handler() {
  // panick("Exception");
  printkf("Interrupt occured with id %d\n", intid);
  __asm__ volatile ("cli; hlt");
}

void encode_idt_entry(uint8_t* restrict target, struct idtentry source) {
  target[0] = source.isr & 0xFF;
  target[1] = (source.isr >> 8) & 0xFF;
  target[6] = (source.isr >> 16) & 0xFF;
  target[7] = (source.isr >> 24) & 0xFF;

  target[2] = GDT_KERNEL_CODE_SELECTOR & 0xFF;
  target[3] = (GDT_KERNEL_CODE_SELECTOR >> 8) & 0xFF;

  target[4] = 0;
  target[5] = source.attributes;
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
