#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

#include "../feb.X/feb2.h"
#include "uart.h"
#include "../../software/states.h"

/* guest program start/end addresses */
#define PROG_START 0x1000
#define PROG_END 0xfbffu

/* point to guest program interrupt vectors */
asm("PSECT intcode");
asm("GOTO " ___mkstr(PROG_START) " + 0x8");
asm("PSECT intcodelo");
asm("GOTO " ___mkstr(PROG_START) " + 0x18");

/* function declarations */
uint8_t uart_get_value();
void erase_flash();
int8_t write_data(uint8_t addr_h, uint8_t addr_l, uint8_t size, uint8_t *data);

void main(void) {
    uint8_t c;
    uint8_t size;
    uint8_t addr_h;
    uint8_t addr_l;
    uint8_t type;
    uint8_t data[256];
    uint8_t checksum;
    uint24_t sum;
    int8_t status;

    feb2_setup();
    uart_setup();

    BLUE_LED = 0;
    YELLOW_LED = 0;
    GREEN_LED = 0;
    RED_LED = 0;

    while (1) {
        /* waiting for record */
        BLUE_LED = 1;
        c = uart_recv_uchar();
        BLUE_LED = 0;
        switch (c) {
            case STATE_ERASE_FLASH:
                /* erase flash */
                YELLOW_LED = 1;
                erase_flash();
                YELLOW_LED = 0;
                uart_send_uchar('0');
                break;
            case ':':
                /* get record line */
                GREEN_LED = 1;
                size = uart_get_value(); /* get size (byte count) */
                addr_h = uart_get_value(); /* get address high byte */
                addr_l = uart_get_value(); /* get address low byte */
                type = uart_get_value(); /* get record type (0x00 is data, 0x01 is EOF) */
                for (uint8_t i = 0; i < size; i++) {
                    data[i] = uart_get_value(); /* get data */
                }
                checksum = uart_get_value(); /* get checksum */
                GREEN_LED = 0;

                RED_LED = 1;
                /* sum of all bytes should be zero */
                sum = size + addr_h + addr_l + type + checksum;
                for (uint8_t i = 0; i < size; i++) {
                    sum += data[i];
                }
                if (sum & 0xff) {
                    /* checksum error */
                    uart_send_uchar('1');
                } else {
                    /* checksum OK */
                    if (type == 0x01) { /* EOF */
                        uart_send_uchar('0');
                    } else {
                        status = write_data(addr_h, addr_l, size, data);
                        if (status) {
                            uart_send_uchar('1');
                        } else {
                            uart_send_uchar('0');
                        }
                    }
                }
                RED_LED = 0;
                break;
            case STATE_BOOT:
                /* execute guest program */
                uart_send_uchar('0');
                asm("goto " ___mkstr(PROG_START));
                break;
            default:
                /* unrecognized character */
                BLUE_LED = 1;
                YELLOW_LED = 1;
                GREEN_LED = 1;
                RED_LED = 1;
                Delay10KTCYx(0);
                BLUE_LED = 0;
                YELLOW_LED = 0;
                GREEN_LED = 0;
                RED_LED = 0;
                break;
        }
    }
}

/* extract value from UART chars */
uint8_t uart_get_value() {
    uint8_t byte[3];
    byte[0] = uart_recv_uchar();
    byte[1] = uart_recv_uchar();
    byte[2] = '\0';
    return (uint8_t) strtol(byte, NULL, 16);
}

/* erase flash memory */
void erase_flash() {

    /* enable write cycles */
    EECON1bits.WREN = 1;

    /* disable interrupts */
    INTCONbits.GIE = 0;

    /* erase */
    for (uint16_t address = PROG_START;
            address <= PROG_END + 1 - FLASH_ERASE_BLOCK; /* beware of overflow */
            address += FLASH_ERASE_BLOCK) {
        TBLPTRU = 0x00;
        TBLPTRH = (uint8_t) (address >> 8);
        TBLPTRL = 0x00;

        EECON1bits.FREE = 1;

        /* required sequence for flash update */
        EECON2 = 0x55;
        EECON2 = 0xAA;

        /* initiates erase cycle */
        EECON1bits.WR = 1;
    }

    /* enable interrupts */
    INTCONbits.GIE = 1;

    /* disable write cycles */
    EECON1bits.WREN = 0;
}

/* write data on flash memory */
int8_t write_data(uint8_t addr_h, uint8_t addr_l, uint8_t size, uint8_t *data) {
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

    /* write */
    for (uint8_t i = 0; i < size; i += 2) {
        TABLAT = data[i]; /* LSB of word to be written */
        asm("TBLWT*+"); /* increment */
        TABLAT = data[i + 1]; /* MSB of word to be written */
        asm("TBLWT*"); /* do not increment */

        /* required sequence */
        EECON2 = 0x55;
        EECON2 = 0xAA;

        /* initiates write cycle */
        EECON1bits.WR = 1;

        asm("TBLWT*+"); /* increment */
    }

    /* enable interrupts */
    INTCONbits.GIE = 1;

    /* disable write cycles */
    EECON1bits.WREN = 0;

    /* block (64 bytes) write mode */
    EECON1bits.WPROG = 0;

    /* reset start address */
    TBLPTRU = 0x00;
    TBLPTRH = addr_h;
    TBLPTRL = addr_l;

    /* verify data written */
    for (uint8_t i = 0; i < size; i += 2) {
        asm("TBLRD*+"); /* increment */
        if (data[i] != TABLAT) {
            return (int8_t) -1;
        }
        asm("TBLRD*+"); /* increment */
        if (data[i + 1] != TABLAT) {
            return (int8_t) -1;
        }
    }
    return 0;
}
