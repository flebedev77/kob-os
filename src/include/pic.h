#pragma once
#include <stdint.h>

void pic_send_eoi(uint8_t irq_index);
void pic_init();
