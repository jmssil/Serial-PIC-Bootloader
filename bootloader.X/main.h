#ifndef MAIN_H
#define	MAIN_H

#ifdef __18F26J50
#include <pic18f26j50.h>
#endif

#pragma config DEBUG = OFF
#pragma config XINST = OFF     // Disables the extended CPU configuration mode
#pragma config WDTEN = OFF     // Disable watchdog

#pragma config OSC = INTOSCPLL // Internal 8MHz clock and PLL are turned on
#pragma config PLLDIV = 2      // 8MHz internal oscillator is divided by 2 and fed to PLL
#pragma config CPUDIV = OSC1   // Input 48MHz clock from PLL is divided by 2 - System clock is 24MHz

#pragma config FCMEN = OFF     // Fail-Safe Clock Monitor bit - disabled
#pragma config IESO = OFF      // Two-Speed Start-up (Internal/External Oscillator Switch over) control bit - disabled
#pragma config LPT1OSC = OFF
#pragma config STVREN = OFF
#pragma config CP0 = OFF       // Program memory is not code protected

#define LED1 LATAbits.LATA7
#define LED2 LATAbits.LATA6
#define LED3 LATCbits.LATC0
#define LED4 LATCbits.LATC1

#define ON 1
#define OFF 0

#ifdef	__cplusplus
extern "C" {
#endif

    /* function prototypes */

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */
