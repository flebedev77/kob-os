#pragma once
#include <stdint.h>
#include <stdbool.h>

#define SOFTWARE_INTERRUPTS_AMOUNT 32
#define HARDWARE_INTERRUPTS_AMOUNT 15

extern void set_idt(uint16_t limit, uint32_t base);
extern void set_gdt(uint16_t limit, uint32_t base);
extern uint32_t isr_stub_table[];
extern uint8_t intid;

extern uint32_t get_cpu_support(void);
extern uint32_t get_cpu_model(void);
extern char* get_cpu_vendor(void);
