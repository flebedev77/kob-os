#include <idt.h>
#include <gdt.h>
#include <libk/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <term.h>
#include "util.h"
#include "pic.h"
#include "../drivers/pckbd.h"

struct idtentry {
  uint32_t isr;
  uint8_t attributes;
};

__attribute__((aligned(8)))
static uint8_t entries[8 * 256] = {0};

char* exception_table[] = {
  "Divide error",
  "Debug",
  "NMI",
  "Breakpoint",
  "Overflow",
  "Bounds check",
  "Invalid opcode",
  "Co-processor not avaliable",
  "Double fault",
  "Co-processor segment overrun",
  "Invalid TSS",
  "Segment not present",
  "Stack",
  "General protection",
  "Page fault"
};

void exception_handler(uint8_t arg) {
  panick("%s exception, with arg %d\n", exception_table[intid], arg);
  __asm__ volatile ("cli; hlt");
}

void interrupt_handler(uint8_t arg) {
  if (intid == SOFTWARE_INTERRUPTS_AMOUNT + 7 || intid == SOFTWARE_INTERRUPTS_AMOUNT + HARDWARE_INTERRUPTS_AMOUNT) {
    printkf("Fake PIC hardware interrupt occured\n");
  }
  if (intid == SOFTWARE_INTERRUPTS_AMOUNT + 1) { // pckbd interrupt
    pckbd_interrupt();
  } else {
    // printkf("Interrupt occured with id %d and arg %d\n", intid, arg);
  }

  if (intid >= SOFTWARE_INTERRUPTS_AMOUNT && intid <= SOFTWARE_INTERRUPTS_AMOUNT + HARDWARE_INTERRUPTS_AMOUNT) {
    //is a hardware interrupt
    pic_send_eoi(intid - SOFTWARE_INTERRUPTS_AMOUNT);
  } else if (intid < SOFTWARE_INTERRUPTS_AMOUNT) {
    exception_handler(arg);
  }
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

void idt_init(void) {
  for (int index = 0; index <= IDT_MAX_DESCRIPTORS; index++) { 
    encode_idt_entry(&entries[8 * index], (struct idtentry){ 
        .isr = isr_stub_table[index],
        .attributes = 0x8E
    });
  }

  set_idt((8 * IDT_MAX_DESCRIPTORS) - 1, (uint32_t)entries);
}
