#pragma once
#include <stdbool.h>
#include <stdint.h>

void printkf(const char* restrict format, ...);
void panick(const char* restrict format, ...);
void assertk(bool condition, const char* restrict format, ...);

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
