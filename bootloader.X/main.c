#include <stdlib.h>
#include <xc.h>

#include "main.h"
#include "setup.h"
#include "uart.h"

/* guest program start address */
#define PROG_START 0x1000

/* point to guest program interrupt vectors */
#asm
PSECT intcode
GOTO PROG_START + 0x8
PSECT intcodelo
GOTO PROG_START + 0x18
#endasm

/* function declarations */
unsigned char uart_get_value();
int write_data(unsigned char addr_h, unsigned char addr_l, unsigned char size, unsigned char *data);

void main(void) {
    unsigned char c;
    unsigned char size;
    unsigned char addr_h;
    unsigned char addr_l;
    unsigned char type;
    unsigned char data[256];
    unsigned char checksum;
    unsigned int sum;

    setup();

    while (1) {
        /* waiting for record */
        LED1 = ON;
        LED2 = OFF;
        c = uart_recv_uchar();
        switch (c) {
            case ':':
                /* process record line */
                LED1 = OFF;
                LED2 = ON;
                /* get size (byte count) */
                size = uart_get_value();
                /* get address high byte */
                addr_h = uart_get_value();
                /* get address low byte */
                addr_l = uart_get_value();
                /* get record type (0x00 is data, 0x01 is EOF) */
                type = uart_get_value();
                /* get data */
                for (unsigned int i = 0; i < size; i++) {
                    data[i] = uart_get_value();
                }
                /* get checksum */
                checksum = uart_get_value();
                /* sum of all bytes should be zero */
                sum = size + addr_h + addr_l + type + checksum;
                for (unsigned int i = 0; i < size; i++) {
                    sum += data[i];
                }
                if (sum & 0xff) {
                    /* checksum error */
                    uart_send_uchar('1');
                } else {
                    /* checksum OK */
                    if (type == 0x01) { /* EOF */
                        uart_send_uchar('0');
                        goto end;
                    } else {
                        write_data(addr_h, addr_l, size, data);
                        uart_send_uchar('0');
                    }
                }
                break;
            case 'x':
                /* execute guest program */
                uart_send_uchar('0');
                goto end;
            default:
				/* unrecognized character */
                LED1 = OFF;
                LED2 = OFF;
                Delay10KTCYx(0);
                break;
        }
    }
end:
    /* jump to guest program */
    asm("goto " ___mkstr(PROG_START));
}

/* extract value from UART chars */
unsigned char uart_get_value() {
    char byte[3];
    byte[0] = uart_recv_uchar();
    byte[1] = uart_recv_uchar();
    byte[2] = '\0';
    return (unsigned char) strtol(byte, NULL, 16);
}

/* write data on flash memory */
int write_data(unsigned char addr_h, unsigned char addr_l, unsigned char size, unsigned char *data) {
    /* set start address */
    TBLPTRU = 0x00;
    TBLPTRH = addr_h;
    TBLPTRL = addr_l;

    /* word (2 bytes) write mode */
    EECON1bits.WPROG = 1;

    /* enable write cycles */
    EECON1bits.WREN = 1;

    /* disable interrupts */
    INTCONbits.GIE = 0;

    for (unsigned int i = 0; i < size; i += 2) {
        TABLAT = data[i];
        asm("TBLWT*+"); /* increment */
        TABLAT = data[i + 1];
        asm("TBLWT*"); /* do not increment */

        /* required sequence */
        EECON2 = 0x55;
        EECON2 = 0xAA;

        /* write */
        EECON1bits.WR = 1;

        asm("TBLWT*+"); /* increment */
    }

    /* enable interrupts */
    INTCONbits.GIE = 1;

    /* disable write cycles */
    EECON1bits.WREN = 0;

    /* block (64 bytes) write mode */
    EECON1bits.WPROG = 0;
}
