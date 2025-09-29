#pragma once

extern void set_idt(uint16_t limit, uint32_t base);
extern void set_gdt(uint16_t limit, uint32_t base);
extern uint32_t isr_stub_table[];
