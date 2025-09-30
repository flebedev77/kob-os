#!/usr/bin/tcc -run
#include <stdio.h>
#include <stdint.h>

// https://wiki.osdev.org/Interrupt_Descriptor_Table
uint8_t make_idt_flag(
  uint8_t P, // P: Present bit. Must be set (1) for the descriptor to be valid.
  uint8_t DPL, // DPL: A 2-bit value which defines the CPU Privilege Levels which are allowed to access this interrupt via the INT instruction. Hardware interrupts ignore this mechanism.
               // The ring it is (0-3)
  uint8_t gate_type /* Gate Type: A 4-bit value which defines the type of gate this Interrupt Descriptor represents. There are five valid type values:

    0b0101 or 0x5: Task Gate, note that in this case, the Offset value is unused and should be set to zero.
    0b0110 or 0x6: 16-bit Interrupt Gate
    0b0111 or 0x7: 16-bit Trap Gate
    0b1110 or 0xE: 32-bit Interrupt Gate
    0b1111 or 0xF: 32-bit Trap Gate */
) {
  uint8_t out = 0;

  out |= gate_type & 0b1111;
  out |= 0 << 4; // Reserved. Also dosen't need to be included here, but is here as an explanation for suddenly shifting DPL by 5 instead of 4
  out |= (DPL & 0b11) << 5;
  out |= (P & 1) << 7;

  return out;
}

int main() {
  uint8_t idt32intring0 = make_idt_flag(1, 0, 0xE);
  uint8_t idt32intring1 = make_idt_flag(1, 1, 0xE);
  uint8_t idt32intring2 = make_idt_flag(1, 2, 0xE);
  uint8_t idt32intring3 = make_idt_flag(1, 3, 0xE);

  printf("IDT Gate descriptor flags for 32 bit interrupt gate in ring 0: 0x%02X\n", idt32intring0);
  printf("IDT Gate descriptor flags for 32 bit interrupt gate in ring 1: 0x%02X\n", idt32intring1);
  printf("IDT Gate descriptor flags for 32 bit interrupt gate in ring 2: 0x%02X\n", idt32intring2);
  printf("IDT Gate descriptor flags for 32 bit interrupt gate in ring 3: 0x%02X\n", idt32intring3);

  return 0;
}
