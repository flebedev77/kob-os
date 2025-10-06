#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <term.h>
#include <stdio.h>
#include <libk/io.h>
#include <gdt.h>
#include <idt.h>
#include <pic.h>

#include "drivers/pckbd.h"
#include "drivers/power.h"
#include "drivers/cpu.h"

#include "tty/tty.h"

#include <multiboot.h>

#if defined(__linux__)
#error "Not using a correct cross compiler"
#endif

#if !defined(__i386__)
#error "Kernel only works with a 32 bit compiler"
#endif

void k_main(struct multiboot_info* mbd, unsigned int mb_magic) {
  assertk(mb_magic != MULTIBOOT_BOOTLOADER_MAGIC, "Bootloader magic number is incorrect! Should be %x and is %x. The bootloader is messed up!\n", MULTIBOOT_BOOTLOADER_MAGIC, (int)mb_magic);

  multiboot_ptr = mbd;
  cpu_init();
  stdio_init();
  power_init();

  gdt_init();
  pic_init();
  idt_init();

  pckbd_init();
  tty_init();

  // tty_run never returns, looks for the init process and runs it.
  // if the init process was not found, then will run a kernel shell
  // which will let you access basic system information, shutdown,
  // specify a new place to look for the init executable, etc.
  tty_run();

  while (true) {}
}
