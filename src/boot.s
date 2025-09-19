.set ALIGN,    1<<0
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    (ALIGN | MEMINFO)  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */


.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text 
.global _start

.extern k_main

_start:
  # Setup stack
  mov $stack_top, %esp

  call k_main

loop:
  jmp loop
