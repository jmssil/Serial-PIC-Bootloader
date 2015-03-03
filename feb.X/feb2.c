#include <pic18f26j50.h>

void feb2_setup() {
    /*** Configure system clock ****************************************************************************/
    /*** Crystal oscillator ports **************************************************************************/

    TRISAbits.TRISA6 = 1; // Set PORTA bit (RA6) as output
    LATAbits.LATA6 = 0; // Clear PORTA bit (RA6)
    TRISAbits.TRISA7 = 1; // Set PORTA bit (RA7) as output
    LATAbits.LATA7 = 0; // Clear PORTA bit (RA7)

    /*** Oscillator control setup **************************************************************************/

    OSCTUNEbits.INTSRC = 0;     // 31 kHz device clock derived directly from INTRC internal oscillator
    OSCTUNEbits.PLLEN = 1;     // 96 MHz PLL is enabled
    OSCTUNEbits.TUN = 0b111011; // Fine tune oscilator frequency drift

    OSCCONbits.IDLEN = 0;       // Device enters Sleep mode on SLEEP instruction
    OSCCONbits.IRCF = 0b111;    // Post-scaled value of internal clock oscillator - 8MHz
    OSCCONbits.OSTS = 0;
    OSCCONbits.SCS = 0b00;      // Primary clock source

    /*** Reference clock setup - required only if a clock output is necessary for an external device *******/

    REFOCONbits.ROON = 0;       // Reference clock signal available on REFO pin - port RB2 (board pin# 8) - YELLOW LED
    REFOCONbits.ROSSLP = 1;     // Reference oscillator continues to run in Sleep
    REFOCONbits.ROSEL = 0;      // System clock (FOSC) is used as the base clock
    REFOCONbits.RODIV = 0b1010; // Selects the clock divider option - 1024

    /*** USB clock setup ***********************************************************************************/

    UCFGbits.FSEN = 1; // Selects no clock division for USB module - USB clock = System clock Fosc = 48MHz

    /*** Configure Peripheral Pin Select (PPS) module - Maps peripheral IO pins to physical RPn chip IO pins ***/
    /*** SFR - Special function register ***********************************************************************/

    EECON2 = 0x55; // Unlock control register - allows writes to the control registers
    EECON2 = 0xAA;

    RPOR13 = 14;   // CCP1/P1A - ECCP1 Compare or PWM Output Channel A assigned to output RP13 - Lighting control
    //RPOR8 = 18;   // CCP2/P2A - ECCP2 Compare or PWM Output Channel A assigned to output RP8 - Audio signalling

    EECON2 = 0x55; // Lock control register - prevents writes to the control register
    EECON2 = 0xAA;

    /*** Configure timer2 that drives CCP1 module ***********************************************************/

    T2CONbits.T2OUTPS = 0b0000; // Timer2 Output Postscale Select bits - Not used
    T2CONbits.TMR2ON = 1;      // Timer2 On bit
    T2CONbits.T2CKPS = 0b00;    // Timer2 Clock Prescale Select bits -  Set to 1

    PR2 = 249; // Timer2 Period Register - Set to 249 (48 kHz) for 10-bit PWM resolution
    // Duty-cycle of 100% corresponds to 250 on 8-bit register CCPR1L

    TCLKCONbits.T3CCP2 = 0b0; // ECCP1 uses timer2 (lighting PWM) and ECCP2 uses timer4 (audio PWM)
    TCLKCONbits.T3CCP1 = 0b1;
    TCLKCONbits.T1RUN = 0;    // System clock comes from an oscillator other than T1OSC/T1CKI

    /*** Configure CCP1 module as PWM for lighting control *****************************************/

    CCP1CONbits.P1M = 0b00;     // Single output: PxA, PxB, PxC and PxD are controlled by steering
    CCP1CONbits.DC1B = 0b00;    // Two LSB of 10-bit duty-cycle word
    CCP1CONbits.CCP1M = 0b1100; // PWM mode - PxA and PxC are active-high - PxB and PxD are active-high

    TRISCbits.TRISC2 = 0; // Set PORTC bit (RC2) as output for light control PWM
    LATCbits.LATC2 = 0; // Clear PORTC bit (RC2)

    /*** Configure timer4 that dives CCP2 module ***************************************************/

    //T4CONbits.T4OUTPS = 0b0000; // Timer4 Output Postscale Select bits - Not used
    //T4CONbits.TMR4ON = 0;      // Timer4 On bit
    //T4CONbits.T4CKPS = 0b11;    // Timer4 Clock Prescale Select bits - Set to 16

    //PR4 = 250;    // Defines buzzer frequency
    //CCPR2L = 150; // Defines buzzer duty cycle

    /*** Configure CCP2 module as PWM for audio signalling *****************************************/

    //CCP2CONbits.P2M = 0b00;     // Single output: PxA, PxB, PxC and PxD are controlled by steering
    //CCP2CONbits.DC2B = 0b00;    // Two LSB of 10-bit duty-cycle word
    //CCP2CONbits.CCP2M = 0b1100; // PWM mode - PxA and PxC are active-high - PxB and PxD are active-high

    //TRISBbits.TRISB5 = 0; // Set PORTB bit (RB5 - Speaker drive) as output
    //LATBbits.LATB5 = 0; // Clear PORTB bit (RB5 - Speaker driver)

    /*** Configure on-board LED ports *****************************************************************/

    TRISBbits.TRISB3 = 0; // Set PORTB bit (RB3) as output - BLUE LED
    LATBbits.LATB3 = 0; // Clear PORTB bit (RB3)
    TRISBbits.TRISB2 = 0; // Set PORTB bit (RB2) as output - YELLOW LED
    LATBbits.LATB2 = 0; // Clear PORTB bit (RB2)
    TRISBbits.TRISB1 = 0; // Set PORTB bit (RB1) as output - GREEN LED
    LATBbits.LATB1 = 0; // Clear PORTB bit (RB1)
    TRISBbits.TRISB0 = 0; // Set PORTB bit (RB0) as output - RED LED
    LATBbits.LATB0 = 0; // Clear PORTB bit (RB0)

    /*** Configure audio signalling port **************************************************************/

    TRISBbits.TRISB5 = 0; // Set PORTB bit (RB5 - Speaker driver) as output
    LATBbits.LATB5 = 0; // Clear PORTB bit (RB5 - Speaker driver)

    /*** Configure GPIO ports *************************************************************************/

    TRISCbits.TRISC4 = 1; // Set PORTC bit (RC4 - GPIO1) as input
    LATCbits.LATC4 = 0; // Clear PORTC bit (RC4 - GPIO1)
    TRISCbits.TRISC5 = 1; // Set PORTC bit (RC5 - GPIO2) as intput
    LATCbits.LATC5 = 0; // Clear PORTC bit (RC5 - GPIO2)
    TRISCbits.TRISC1 = 1; // Set PORTC bit (RC1 - GPIO3) as input
    LATCbits.LATC1 = 0; // Clear PORTC bit (RC1 - GPIO3)
    TRISCbits.TRISC0 = 1; // Set PORTC bit (RC0 - GPIO4) as input
    LATCbits.LATC0 = 0; // Clear PORTC bit (RC0 - GPIO4)

    /*** Configure unused ports ***********************************************************************/

    TRISAbits.TRISA5 = 1; // Set PORTA bit (RA5) as output - pin7 - N/C
    LATAbits.LATA5 = 0; // Clear PORTA bit (RA5)
    TRISBbits.TRISB4 = 1; // Set PORTA bit (RB4) as output - pin25 - N/C
    LATBbits.LATB4 = 0; // Clear PORTA bit (RB4)

    /***** Setup ADC **********************************************************************************/

    ADCON0bits.VCFG0 = 0; // Vref+ = AVdd - Input range is full-scale 3.3V
    ADCON0bits.VCFG1 = 0; // Vref- = AVss - Input range is full-scale 3.3V

    ADCON1bits.ADFM = 1; // Result format 1 - Right justified
    ADCON1bits.ADCAL = 0; // Normal A/D conversion operation
    ADCON1bits.ACQT = 0b001; // Acquisition time set to 2TAD
    ADCON1bits.ADCS = 0b110; // Clock conversion set to FOSC/64

    ANCON1bits.VBGEN = 1; // Enable bandgap module
    ADCON0bits.ADON = 1; // Enable ADC module

    /***** Setup analog I/O ports *********************************************************************/

    ANCON0 = 0b11111111; // Set all A/D channels as digital I/O
    ANCON1 = 0b00011111; // Set all A/D channels as digital I/O

    TRISAbits.TRISA0 = 1; // Set PORTA bit (RA0 - board pin# 0) as input
    ANCON0bits.PCFG0 = 0; // Configure AN0 as an analog input channel
    TRISAbits.TRISA1 = 1; // Set PORTA bit (RA1 - board pin# 1) as input
    ANCON0bits.PCFG1 = 0; // Configure AN1 as an analog input channel
    TRISAbits.TRISA2 = 1; // Set PORTA bit (RA2 - board pin# 2) as input
    ANCON0bits.PCFG2 = 0; // Configure AN2 as an analog input channel
    TRISAbits.TRISA3 = 1; // Set PORTA bit (RA3 - board pin# 3) as input
    ANCON0bits.PCFG3 = 0; // Configure AN3 as an analog input channel
}
