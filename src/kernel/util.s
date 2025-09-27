.arch i386
.code32

.section .data
gdtr: .word 0
      .long 0

.section .text
.global set_gdt

set_gdt:
  # cli
  movw   4(%esp), %ax
  movw   %ax, gdtr
  movl   8(%esp), %eax
  movl   %eax, gdtr + 2
  lgdt   gdtr
  # ret
  ljmp $0x10, $flush_segments

  # flushing
flush_segments:
  movw $0x18, %ax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %ss
  # sti
  ret

.extern exception_handler

.macro isr_error_stub index
  isr_stub_\index:
    call exception_handler
    iret
.endm

.macro isr_no_error_stub index
  isr_stub_\index:
    call exception_handler
    iret
.endm

keyboard_handler:

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
# %macro isr_err_stub 1
# isr_stub_%+%1:
#     call exception_handler
#     iret 
# %endmacro
# ; if writing for 64-bit, use iretq instead
# %macro isr_no_err_stub 1
# isr_stub_%+%1:
#     call exception_handler
#     iret
# %endmacro

