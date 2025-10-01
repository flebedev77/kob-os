#pragma once

#define GDT_KERNEL_CODE_SELECTOR 0x10 // Assuming kernel code is index 2
#define GDT_KERNEL_DATA_SELECTOR 0x18 // Assuing kernel data is index 3

void gdt_init(void);
