#pragma once
#include <stdint.h>

void pic_send_eoi(uint8_t irq_index);
void pic_irq_clear_mask(uint8_t irqline);
void pic_irq_set_mask(uint8_t irqline);
void pic_init();
