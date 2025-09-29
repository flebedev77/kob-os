#include <idt.h>
#include <gdt.h>
#include <libk/io.h>
#include <stdbool.h>

#define IDT_MAX_DESCRIPTORS 32

__attribute__((aligned(0x10))) 
static struct idt_entry idt[256] = {0};
static struct idtr idtr = {0};

void exception_handler() {
  printkf("Exception");
  // panick("Unhandled/unknown exception happened");
  // __asm__ volatile ("cli; hlt");
}

void idt_set_descriptor(uint8_t index, uint32_t isr, uint8_t flags) {
    struct idt_entry* descriptor = &idt[index];

    descriptor->isr_low        = isr & 0xFFFF;
    descriptor->kernel_cs      = GDT_KERNEL_CODE_SELECTOR;
    descriptor->attributes     = flags;
    descriptor->isr_high       = (isr >> 16) & 0xFFFF;
    descriptor->reserved       = 0;
}

extern uint32_t isr_stub_table[];

void idt_init() {
  for (int index = 0; index < 32; index++) {
    idt_set_descriptor(index, isr_stub_table[index], 0x8E);
  }

  idtr.base = (uint32_t)&idt;
  idtr.limit = sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;

  __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
  __asm__ volatile ("sti"); // set the interrupt flag
}
