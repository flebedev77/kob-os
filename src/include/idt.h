#pragma once

#define IDT_MAX_DESCRIPTORS 47
// 32 software interrupts + 15 hardware interrupts

void idt_init(void);
