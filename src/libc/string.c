#include <stddef.h>

size_t strlen(const char* restrict str) {
  size_t i;
  for (i = 0; str[i] != 0; i++) {}
  return i;
}

int strcmp(const char* restrict a, const char* restrict b) {
  size_t size_a = strlen(a);
  size_t size_b = strlen(b);

  if (size_a < size_b) return -1;
  if (size_a > size_b) return 1;

  for (size_t i = 0; i < size_a; i++) {
    if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
  }
  return 0;
}

char* strcpy(char* restrict dst, char* restrict src) {
  size_t size = strlen(src);
  for (size_t i = 0; i < size; i++) {
    dst[i] = src[i];
  }
  return dst;
}
