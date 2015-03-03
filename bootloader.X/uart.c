#include <pic18f26j50.h>

void uart_setup() {
    TRISCbits.TRISC6 = 0; // Set PORTC bit (RC6 - TX1) as output (transmit)
    LATCbits.LATC6 = 0; // Clear PORTC bit (RC6)
    TRISCbits.TRISC7 = 1; // Set PORTC bit (RC7 - RX1) as input (receive)
    LATCbits.LATC7 = 0; // Clear PORTC bit (RC7)

    // SPBRG = Fosc/4/Baud-1
    // SPBRG = 0x04E1 (9600 baud) - Fosc = 48MHz
    // SPBRG = 0x0270 (19200 baud) - Fosc = 48MHz
    // SPBRG = 0x00CF (57600 baud) - Fosc = 48MHz
    // SPBRG = 0x0067 (115200 baud) - Fosc = 48MHz
    SPBRGH1 = 0x00; // the hex value selected from the table (higher part)
    SPBRG1 = 0x67; // the hex value selected from the table (lower part)
    BAUDCON1bits.BRG16 = 1; // 8-bit baud rate generator
    TXSTA1bits.BRGH = 1; // High baud rate select bit

    TXSTA1bits.SYNC = 0; // Set UART to asynchronous mode
    RCSTA1bits.SPEN = 1; // Enable transmit
    TXSTA1bits.TX9 = 0; // Selects 8-bit transmission mode
    TXSTA1bits.TXEN = 1; // Enable transmission
    TXSTA1bits.SENDB = 0; // Do not send break character

    RCSTA1bits.CREN = 1; // Enable receiver
}

void uart_send_uchar(unsigned char value) {
    TXREG1 = value;
    while (!TXSTA1bits.TRMT);
}

unsigned char uart_recv_uchar() {
    if (RCSTA1bits.OERR) {
        /* overrun error, reset CREN */
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }

    while (!PIR1bits.RC1IF);
    return RCREG1;
}
