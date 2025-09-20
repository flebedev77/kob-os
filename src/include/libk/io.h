#pragma once
#include <stdbool.h>

void printkf(const char* format, ...);
void panick(const char* format, ...);
void assertk(bool condition, const char* format, ...);
