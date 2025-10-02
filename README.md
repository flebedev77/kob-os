# KOB OS
i386 32 bit operating system. With basic bare bones functionality to demonstrate the code required for an operating system to function.


# Compiling
Run
`make setup`
to create the proper build directory structure. Then you can run
`make`
it will build and attempt to launch the os through qemu.

You can also have the makefile generate an iso file, which you can clone to a usb and boot on real hardware.
`make build/os.iso`

This may damage your hardware, I am not liable for any damages.

# Features
 - 80 by 25 terminal display
 - Interrupt driven PS2 keyboard driver
