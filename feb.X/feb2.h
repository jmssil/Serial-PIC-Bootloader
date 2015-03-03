#ifndef FEB2_H
#define	FEB2_H

#include <pic18f26j50.h>

#pragma config DEBUG = OFF
#pragma config XINST = OFF       // Disables the extended CPU configuration mode
#pragma config WDTEN = OFF       // Disable watchdog

#pragma config OSC = HSPLL       // High-speed 8MHz crystal resonator and PLL are turned on - Fosc = 48MHz
#pragma config PLLDIV = 2        // 8MHz clock frequency is divided by 2 and fed to PLL
#pragma config CPUDIV = OSC1     // Input 48MHz clock from PLL is divided by 1 - System clock is 48MHz

#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor bit - disabled
#pragma config IESO = OFF        // Two-Speed Start-up (Internal/External Oscillator Switchover) control bit - disabled
#pragma config LPT1OSC = OFF
#pragma config STVREN = OFF
#pragma config CP0 = ON         // Program memory is not code protected

#define BLUE_LED LATBbits.LATB3
#define YELLOW_LED LATBbits.LATB2
#define GREEN_LED LATBbits.LATB1
#define RED_LED LATBbits.LATB0

#define Chan_0 0b0000 // ADC channel 0
#define Chan_1 0b0001 // ADC channel 1
#define Chan_2 0b0010 // ADC channel 2
#define Chan_3 0b0011 // ADC channel 3

//#define RST_SBC LATBbits.LATB5   // Reset SBC bit - active high

#define PWM CCPR1L

#define BUZZ_port LATBbits.LATB5
//#define BUZZ T4CONbits.TMR4ON

#ifdef	__cplusplus
extern "C" {
#endif

    void feb2_setup();

#ifdef	__cplusplus
}
#endif

#endif	/* FEB2_H */

