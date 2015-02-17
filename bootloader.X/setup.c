#include <pic18.h>
#ifdef __18F26J50
#include <pic18f26j50.h>
#endif

#include "main.h"
#include "uart.h"

void setup() {

    /*** Configure system clock ****************************************************************************/
    /*** Oscillator control setup **************************************************************************/

    OSCTUNEbits.INTSRC = 0; // 31 kHz device clock derived directly from INTRC internal oscillator
    OSCTUNEbits.PLLEN = ON; // 96 MHz PLL is enabled
    OSCTUNEbits.TUN = 0b111011; // Fine tune oscillator frequency drift

    OSCCONbits.IDLEN = 0; // Device enters Sleep mode on SLEEP instruction
    OSCCONbits.IRCF = 0b110; // Post-scaled value of internal clock oscillator - 4MHz
    OSCCONbits.OSTS = 0;
    OSCCONbits.SCS = 0b00; // Primary clock source

    /*** Reference clock setup - required only if a clock output is necessary for an external device *******/

    REFOCONbits.ROON = 1; // Reference clock signal available on REFO pin - port RB2 (board pin# 8)
    REFOCONbits.ROSSLP = 1; // Reference oscillator continues to run in Sleep
    REFOCONbits.ROSEL = 0; // System clock (FOSC) is used as the base clock
    REFOCONbits.RODIV = 0b1010; // Selects the clock divider option - 1024

    /*** Configure ports for on-board LEDs *****************************************************************/

    TRISAbits.TRISA6 = 0; // Set PORTA bit (RA6) as output
    LATAbits.LATA6 = OFF; // Clear PORTA bit (RA6)
    TRISAbits.TRISA7 = 0; // Set PORTA bit (RA7) as output
    LATAbits.LATA7 = OFF; // Clear PORTA bit (RA7)

    TRISCbits.TRISC0 = 0; // Set PORTC bit (RC0) as output
    LATCbits.LATC0 = OFF; // Clear PORTC bit (RC0)
    TRISCbits.TRISC1 = 0; // Set PORTC bit (RC1) as output
    LATCbits.LATC1 = OFF; // Clear PORTC bit (RC1)

    /***** Setup serial communication module UART **********************************************************/

    uart_setup();
}
