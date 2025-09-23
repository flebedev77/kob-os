.arch i386
.code32

.section .data
gdtr: .word 0
      .long 0

.section .text
.global set_gdt
set_gdt:
  cli
  movw   4(%esp), %ax
  movw   %ax, gdtr
  movl   8(%esp), %eax
  movl   %eax, gdtr + 2
  lgdt   gdtr
  sti
  ret
