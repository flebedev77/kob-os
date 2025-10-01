#include <libk/io.h>
#include "util.h"

#define MASTER_PIC_COMMAND 	0x0020
#define MASTER_PIC_DATA   	0x0021
#define SLAVE_PIC_COMMAND   0x00A0
#define SLAVE_PIC_DATA    	0x00A1

#define PIC_EOI		0x20

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define CASCADE_irq 2

void pic_send_eoi(uint8_t irq_index) {
  if (irq_index > 8) {
    outb(SLAVE_PIC_COMMAND, PIC_EOI);
  }
  outb(MASTER_PIC_COMMAND, PIC_EOI);
}

void pic_remap(int offset1, int offset2) {
	outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(MASTER_PIC_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(SLAVE_PIC_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(MASTER_PIC_DATA, 1 << CASCADE_irq);        // ICW3: tell Master PIC that there is a slave PIC at irq2
	io_wait();
	outb(SLAVE_PIC_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(MASTER_PIC_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(SLAVE_PIC_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs.
	outb(MASTER_PIC_DATA, 0);
	outb(SLAVE_PIC_DATA, 0);
}

void pic_disable(void) {
    outb(MASTER_PIC_DATA, 0xFF);
    outb(SLAVE_PIC_DATA, 0xFF);
}

void pic_irq_set_mask(uint8_t irqline) {
    uint16_t port;
    uint8_t value;

    if(irqline < 8) {
        port = MASTER_PIC_DATA;
    } else {
        port = SLAVE_PIC_DATA;
        irqline -= 8;
    }
    value = inb(port) | (1 << irqline);
    outb(port, value);        
}

void pic_irq_clear_mask(uint8_t irqline) {
    uint16_t port;
    uint8_t value;

    if(irqline < 8) {
        port = MASTER_PIC_DATA;
    } else {
        port = SLAVE_PIC_DATA;
        irqline -= 8;
    }
    value = inb(port) & ~(1 << irqline);
    outb(port, value);        
}

void pic_init(void) {
  pic_remap(SOFTWARE_INTERRUPTS_AMOUNT, SOFTWARE_INTERRUPTS_AMOUNT + 8);
  pic_irq_set_mask(0);
}
