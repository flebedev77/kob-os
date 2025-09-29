#!/usr/bin/tcc -run
#include <stdio.h>
#include <stdint.h>

// https://wiki.osdev.org/Global_Descriptor_Table
uint8_t make_access_byte(
    uint8_t P, // P: Present bit. Allows an entry to refer to a valid segment. Must be set (1) for any valid segment.
    uint8_t DPL, // DPL: Descriptor privilege level field. Contains the CPU Privilege level of the segment. 0 = highest privilege (kernel), 3 = lowest privilege (user applications).
    uint8_t S, // S: Descriptor type bit. If clear (0) the descriptor defines a system segment (eg. a Task State Segment). If set (1) it defines a code or data segment.
    uint8_t E, // E: Executable bit. If clear (0) the descriptor defines a data segment. If set (1) it defines a code segment which can be executed from.
    uint8_t DC, /* DC: Direction bit/Conforming bit.
    For data selectors: Direction bit. If clear (0) the segment grows up. If set (1) the segment grows down, ie. the Offset has to be greater than the Limit.
    For code selectors: Conforming bit.
        If clear (0) code in this segment can only be executed from the ring set in DPL.
        If set (1) code in this segment can be executed from an equal or lower privilege level. For example, code in ring 3 can far-jump to conforming code in a ring 2 segment. The DPL field represent the highest privilege level that is allowed to execute the segment. For example, code in ring 0 cannot far-jump to a conforming code segment where DPL is 2, while code in ring 2 and 3 can. Note that the privilege level remains the same, ie. a far-jump from ring 3 to a segment with a DPL of 2 remains in ring 3 after the jump. */

    uint8_t RW, /* RW: Readable bit/Writable bit.
    For code segments: Readable bit. If clear (0), read access for this segment is not allowed. If set (1) read access is allowed. Write access is never allowed for code segments.
    For data segments: Writeable bit. If clear (0), write access for this segment is not allowed. If set (1) write access is allowed. Read access is always allowed for data segments. */

    uint8_t A // A: Accessed bit. The CPU will set it when the segment is accessed unless set to 1 in advance. This means that in case the GDT descriptor is stored in read only pages and this bit is set to 0, the CPU trying to set this bit will trigger a page fault. Best left set to 1 unless otherwise needed.
    ) {
  uint8_t out = 0;

  out |= A & 1;
  out |= (RW & 1) << 1;
  out |= (DC & 1) << 2;
  out |= (E & 1) << 3;
  out |= (S & 1) << 4;
  out |= (DPL & 0b11) << 6; // DPL is 2 bit
  out |= (P & 1) << 7;

  return out;
}

int main() {
  // present bit means we valid, ring 0, not a task segment so 1, executable, executed only from ring 0, read and writeable, has not been accessed yet
  uint8_t kernel_code = make_access_byte(1, 0, 1, 1, 0, 1, 0);
  // only changed the executable flag
  uint8_t kernel_data = make_access_byte(1, 0, 1, 0, 0, 1, 0);

  printf("Kernel code descriptor access byte %02X\n", kernel_code);
  printf("Kernel data descriptor access byte %02X\n", kernel_data);

  return 0;
}
