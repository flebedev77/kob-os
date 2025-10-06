#include <libk/io.h>
#include <string.h>
#include "cpu.h"

void power_init(void) {
  
}

void power_off(void) {
  // TODO HARDWAREDAMAGE make this detect environment using cpuid
  if (strcmp(cpu_vendor, CPUID_VENDOR_QEMU) == 0) outw(0x604, 0x2000);
  else panick("Unable to poweroff this cpu\n");
}

void power_reboot(void) {

}
