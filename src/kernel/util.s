.arch i386
.code32

.section .data
gdtr: .word 0 # uint16_t limit
      .long 0 # uint32_t base

idtr: .word 0 # uint16_t limit
      .long 0 # uint32_t base

.global intid
intid: .byte 0

.section .text

.global set_gdt
set_gdt:
  movw   4(%esp), %ax
  movw   %ax, (gdtr)
  movl   8(%esp), %eax
  movl   %eax, (gdtr + 2)
  lgdt   (gdtr)
  ljmp $0x10, $flush_segments

  # flushing
flush_segments:
  movw $0x18, %ax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %ss
  ret

.global set_idt
set_idt:
  movw 4(%esp), %ax
  movw %ax, idtr
  movl 8(%esp), %eax
  movl %eax, (idtr + 2)
  lidt (idtr)
  sti
  ret

.extern interrupt_handler

.macro isr_error_stub index
  isr_stub_\index:
    movb $\index, (intid)
    jmp interrupt_handle
.endm

.macro isr_no_error_stub index
  isr_stub_\index:
    movb $\index, (intid)
    jmp interrupt_handle
.endm

interrupt_handle:
  pusha

  call interrupt_handler

  popa
  add %esp, 4

  iret

isr_no_error_stub 0
isr_no_error_stub 1
isr_no_error_stub 2
isr_no_error_stub 3
isr_no_error_stub 4
isr_no_error_stub 5
isr_no_error_stub 6
isr_no_error_stub 7
isr_error_stub    8
isr_no_error_stub 9
isr_error_stub    10
isr_error_stub    11
isr_error_stub    12
isr_error_stub    13
isr_error_stub    14
isr_no_error_stub 15
isr_no_error_stub 16
isr_error_stub    17
isr_no_error_stub 18
isr_no_error_stub 19
isr_no_error_stub 20
isr_no_error_stub 21
isr_no_error_stub 22
isr_no_error_stub 23
isr_no_error_stub 24
isr_no_error_stub 25
isr_no_error_stub 26
isr_no_error_stub 27
isr_no_error_stub 28
isr_no_error_stub 29
isr_error_stub    30
isr_no_error_stub 31

isr_no_error_stub 32
isr_no_error_stub 33
isr_no_error_stub 34
isr_no_error_stub 35
isr_no_error_stub 36
isr_no_error_stub 37
isr_no_error_stub 38
isr_no_error_stub 39
isr_no_error_stub 40
isr_no_error_stub 41
isr_no_error_stub 42
isr_no_error_stub 43
isr_no_error_stub 44
isr_no_error_stub 45
isr_no_error_stub 46
isr_no_error_stub 47
isr_no_error_stub 48
isr_no_error_stub 49
isr_no_error_stub 50

.global isr_stub_table
isr_stub_table:
        .long isr_stub_0
        .long isr_stub_1
        .long isr_stub_2
        .long isr_stub_3
        .long isr_stub_4
        .long isr_stub_5
        .long isr_stub_6
        .long isr_stub_7
        .long isr_stub_8
        .long isr_stub_9
        .long isr_stub_10
        .long isr_stub_11
        .long isr_stub_12
        .long isr_stub_13
        .long isr_stub_14
        .long isr_stub_15
        .long isr_stub_16
        .long isr_stub_17
        .long isr_stub_18
        .long isr_stub_19
        .long isr_stub_20
        .long isr_stub_21
        .long isr_stub_22
        .long isr_stub_23
        .long isr_stub_24
        .long isr_stub_25
        .long isr_stub_26
        .long isr_stub_27
        .long isr_stub_28
        .long isr_stub_29
        .long isr_stub_30
        .long isr_stub_31
        .long isr_stub_32
        .long isr_stub_33
        .long isr_stub_34
        .long isr_stub_35
        .long isr_stub_36
        .long isr_stub_37
        .long isr_stub_38
        .long isr_stub_39
        .long isr_stub_40
        .long isr_stub_41
        .long isr_stub_42
        .long isr_stub_43
        .long isr_stub_44
        .long isr_stub_45
        .long isr_stub_46
        .long isr_stub_47
        .long isr_stub_48
        .long isr_stub_49
        .long isr_stub_50
