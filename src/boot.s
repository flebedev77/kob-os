.set ALIGN,    1<<0               /* Tell grub to align our kernel in memory */
.set MEMINFO,  1<<1               /* Ask grub to provide memory map */
.set FLAGS,    (ALIGN | MEMINFO)  /* Final flags to put into multiboot header */
.set MAGIC,    0x1BADB002         /* Multiboot magic number */
.set CHECKSUM, -(MAGIC + FLAGS)   /* Checksum of above, to prove we are multiboot */


.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


.section .bss
.align 16 # Follow 16 byte alignment
stack_bottom:
.skip 16384 # 16 KiB stack which grows from top to bottom
stack_top:

.section .text 
.global _start

.extern k_main

_start:
  # Setup stack
  mov $stack_top, %esp

  call k_main
  
  hlt
loop:
  jmp loop
