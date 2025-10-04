#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <multiboot.h>

struct multiboot_info* multiboot_ptr;

void printkf(const char* restrict format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
void panick(const char* restrict format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
void assertk(bool condition, const char* restrict format, ...) {
  if (condition) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    __asm__ volatile( "hlt" );
  }
}

// https://wiki.osdev.org/Detecting_Memory_(x86)
void print_mb(struct multiboot_info* mbd, bool verbose) {
  bool is_mmap_valid = (mbd->flags >> 6) & 0b1;
  bool is_contiguous_mem_valid = mbd->flags & 0b1;

  printkf("------- Memory info -------\n");
  printkf("Memory map detected:        %b\n", is_mmap_valid);
  printkf("Contiguous memory detected: %b\n", is_contiguous_mem_valid);
  printkf("---------------------------\n");

  if (is_contiguous_mem_valid) {
    printkf("Contiguous mem lower %d Kib\n", mbd->mem_lower);
    printkf("Contiguous mem upper %d Kib\n", mbd->mem_upper);
    printkf("Contiguous mem size  %d Kib\n", (mbd->mem_upper-mbd->mem_lower));
    printkf("---------------------------\n");
  }

  if (is_mmap_valid && verbose) {
    int idx = 0;
    for(uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
      multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i);

      printkf("Memory map %d\n", idx);
      printkf(" Start address: 0x%x%x\n", mmmt->addr_high, mmmt->addr_low);
      printkf(" Length:        0x%x%x\n", mmmt->len_high, mmmt->len_low);
      printkf(" Size:          0x%x\n", mmmt->size);
      char* type_str;
      switch(mmmt->type) {
        case 1: type_str = "Usable RAM"; break;
        case 2: type_str = "Reserved unusable"; break;
        case 3: type_str = "ACPI reclaimable RAM"; break;
        case 4: type_str = "ACPI NVS RAM"; break;
        case 5: type_str = "Bad memory"; break;
        default: type_str = "Unknown"; break;
      }
      printkf(" Type:          %d (%s)\n", mmmt->type, type_str);
      printkf("---------------------------\n");
      idx++;
    }
  }
}
