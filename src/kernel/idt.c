#include <idt.h>
#include <libk/io.h>
#include <stdbool.h>

#define IDT_MAX_DESCRIPTORS 32

__attribute__((aligned(16))) 
static struct idt_entry idt[256];
static struct idtr idtr;

void exception_handler() {
  panick("Unhandled/unknown exception happened");
  __asm__ volatile ("cli; hlt");
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    struct idt_entry* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x10;
    descriptor->attributes     = flags;
    descriptor->isr_high       = ((uint32_t)isr >> 16) & 0xFFFF;
    descriptor->reserved       = 0;
}

extern void* isr_stub_table[];

void idt_init() {
  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;

  for (uint8_t vector = 0; vector < IDT_MAX_DESCRIPTORS; vector++) {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
  }

  __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
  __asm__ volatile ("sti"); // set the interrupt flag
}
